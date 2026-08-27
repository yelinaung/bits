# Mark and Sweep

The garbage collector in [ms/](ms/) decides what to free in three passes. All
the code named below lives in that folder.

---

## The question

A collector answers one question: which objects can the running program still
reach? Everything else is garbage.

Two different things can reach an object. A stack frame reaches one directly,
through a local variable. An object reaches another object, through a field.
`mark` handles the first case, `trace` handles the second, and `sweep` acts on
the combined answer.

---

## The data

```text
vm_t
 |- objects : stack_t*   the census. Every object ever allocated.
 |                       The VM owns these and only the VM frees them.
 |- frames  : stack_t*   the call stack. One frame_t per live call.
              frame_t
               |- references : stack_t*   what this frame can see.
                                          Borrowed pointers into the census.
```

`vm_track_object` pushes onto the census. `frame_reference_object` pushes onto
a frame's references. An object appears in the census once and in as many
frames as hold it.

---

## The colors

The algorithm sorts objects into three colors, using one bool and one stack:

```text
white  is_marked == false, not on the gray stack   presumed garbage
gray   is_marked == true,  still on the gray stack reachable, contents unexamined
black  is_marked == true,  popped off              reachable, contents examined
```

Nothing stores "black." An object turns black when it leaves `gray_objects`.

One invariant makes the algorithm correct: **no black object points at a white
one.** Once the gray stack empties and every root is black, every remaining
white object is unreachable.

---

## mark: paint the roots

```c
for each frame in vm->frames
  for each obj in frame->references
    obj->is_marked = true;
```

Roots are the objects a frame holds directly, the local variables of a live
call.

`mark` goes exactly one level deep. An array held by a frame gets marked. Its
elements do not. `test_trace_vector` asserts that half-finished state: after
`mark`, the vector carries the bit and `x`, `y`, and `z` do not.

---

## trace: close over the references

```c
// seed: every marked object starts gray
for each obj in vm->objects
  if (obj->is_marked) stack_push(gray_objects, obj);

// drain
while (gray_objects->count > 0) {
  obj = stack_pop(gray_objects);        // obj turns black
  trace_blacken_object(gray_objects, obj);
}
```

The seed loop reads `vm->objects`, not the frames, because `mark` already
recorded its answer in the `is_marked` bits.

`trace_blacken_object` finds what an object points at and hands each target to
`trace_mark_object`. The fields it reads depend on `kind`. Integers, floats,
and strings point at nothing and turn black on the spot. A `VECTOR3` offers
`x`, `y`, and `z`. An `ARRAY` offers its elements.

`trace_mark_object` walks the graph in three lines:

```c
if (obj == NULL || obj->is_marked) return;   // already gray or black
obj->is_marked = true;
stack_push(gray_objects, obj);                // white -> gray
```

The early return terminates the walk on a cycle. An array containing itself
gets popped once, tries to push itself, finds the bit already set, and
returns. `test_trace_unreachable_cycle` covers exactly that.

The `while` loop drains an explicit worklist instead of recursing. A recursive
walk overflows the C stack on a long chain. A worklist has no such limit.

---

## sweep: free the white, reset the black

```c
for each obj in vm->objects
  if (obj->is_marked) obj->is_marked = false;    // survivor
  else { snek_object_free(obj); data[i] = NULL; }
stack_remove_nulls(vm->objects);
```

`sweep` walks the census because the census is the only list that still holds
the garbage. Nothing else points at an unreachable object, by definition.

Clearing `is_marked` on the survivors matters as much as the freeing. The bit
is scratch space for one collection. Leave it set and the next `mark` treats
every survivor as a root, so nothing is ever collected again.

`stack_remove_nulls` runs after the loop, never inside it. Compacting
mid-iteration shifts elements down into indices the loop already passed, so
the loop skips objects.

---

## A worked collection

`test_trace_nested` builds an array of two arrays, each holding strings, and
gives one frame a reference to the outer array.

```text
frame.references = [alldevs]
vm->objects      = [bootdevs, lane, hunter, terminaldevs, prime, teej,
                    dax, adam, alldevs]        9 objects, all white
```

```text
mark:    alldevs -> marked                     8 white, 1 gray

trace:   seed gray = [alldevs]
         pop alldevs      -> push bootdevs, terminaldevs
         pop terminaldevs -> push prime, teej, dax, adam
         pop adam .. pop prime      strings, nothing to push
         pop bootdevs     -> push lane, hunter
         pop hunter, pop lane
         gray empty                            all 9 black

sweep:   9 marked, so clear 9 bits and free nothing.
```

Drop the frame and collect again. `mark` finds no frames, so nothing turns
gray. `trace` seeds an empty stack and returns. `sweep` finds nine white
objects and frees all nine.

A reference cycle dies for the same reason. Nothing outside the cycle points
into it, so `mark` never reaches it, `trace` never colors it, and `sweep`
frees it. Reference counting cannot release a cycle at all.

---

## Why three passes

You cannot decide an object's fate on first sight. Reaching `bootdevs` early
says nothing about `alldevs`, and freeing during the walk would destroy
objects whose path you have not yet found. The collector must finish computing
reachability before it destroys anything, and that boundary is the line
between `trace` and `sweep`.

`mark` and `trace` could be one function, since `mark` could push roots
straight onto the gray stack. Splitting them makes each one testable alone,
which is what lets `test_trace_vector` assert the state where the vector is
marked and its components are not.

---

## Who owns what

The collector works only because ownership is unambiguous.

- `vm->objects` owns every object. `sweep` and `vm_free` are the only callers
  of `snek_object_free`.
- `frame->references` borrows. `frame_free` releases the container and never
  the objects inside it.
- Object fields borrow. An array does not own its elements, so a cycle can
  form, which is the reason a tracing collector is needed at all.

Every one of those stacks holds `void *`, so the compiler checks none of it.
Calling `stack_free` where `snek_object_free` belongs compiles cleanly and
corrupts the heap.
