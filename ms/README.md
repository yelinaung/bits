The Two Stacks

Both vm->frames and vm->objects are the same C type, stack_t *. They mean opposite things.

```
vm_t
 ├── objects  ──►  [obj, obj, obj, obj, ...]     the heap census — VM OWNS these
 └── frames   ──►  [frame, frame, frame]         the call stack
                      │
                      └── frame_t
                           └── references ──► [obj, obj]   BORROWED — points back
                                                            into vm->objects

vm_t
 ├── frames   : stack_t*   ── a stack OF frame_t*
 │                              each element is a frame_t*
 │                              frame_t
 │                                └── references : stack_t*  ── a stack OF snek_object_t*
 │
 └── objects  : stack_t*   ── a stack OF snek_object_t*
```

vm->objects is a census of every object that exists. Every new_snek_* in sneknew.c (file:///home/yelinaung/code/bits/ms/sneknew.c) ends with vm_track_object(vm, obj), so nothing escapes the list. The VM owns these. Only vm_free and sweep may free them.

vm->frames is the call stack. One frame_t is one function activation.

frame->references is the set of objects that frame can currently see — its local variables, in effect. These pointers are borrowed. The same object appears in both vm->objects and possibly several frame->references.

```
```
