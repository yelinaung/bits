# Memory Layout: Unions, Fixed-Width Integers, Alignment

Notes from the boot.dev C course. All numbers measured on this machine:
Linux x86-64, gcc, little-endian.

---

## 1. Unions

A union stores one value at a time. Every member starts at offset 0, so the
members share the same bytes. The union's size is the size of its largest
member.

```c
typedef union AgeOrName {
  int age;
  char *name;
} age_or_name_t;
```

`sizeof(age_or_name_t)` is 8, the size of the pointer.

### The rule that matters

Read the member you wrote. Writing `.age` overwrites whatever `.name` held,
because they are the same bytes. Reading `.name` afterward reinterprets the
integer's bytes as a pointer, and dereferencing that pointer crashes.

C permits reading a member you did not write. The values are
implementation-defined, not undefined. C++ makes the same read undefined
behavior, so this idiom does not port.

### Tagged unions

Since the union cannot tell you which member is live, you store that yourself:
an enum tag beside the union inside a wrapping struct. `snek_object_t` is built
this way — `kind` selects which arm of `data` is valid.

```c
typedef struct SnekObject {
  bool is_marked;
  snek_object_kind_t kind;   // the tag
  snek_object_data_t data;   // the union
} snek_object_t;
```

Every `switch (obj->kind)` in the codebase exists to keep the tag and the union
in agreement. A missing `break` between two cases reads one member and writes
another. That bug freed `v_array.elements` on a `VECTOR3` object.

---

## 2. The Color Union

```c
typedef union Color {
  struct { uint8_t r, g, b, a; } components;
  uint32_t rgba;
} color_t;
```

Measured:

```
sizeof union      = 4    alignof = 4
sizeof components = 4
offsets: r=0  g=1  b=2  a=3
```

```
offset:    0        1        2        3
        +--------+--------+--------+--------+
        |   r    |   g    |   b    |   a    |   <- .components
        +--------+--------+--------+--------+
        |            rgba (uint32_t)        |   <- .rgba
        +-----------------------------------+
```

The inner struct has no padding. `uint8_t` is 1-aligned, so every field already
sits at a legal address and the compiler has no gap to fill.

The union's alignment is 4, inherited from `uint32_t`. A union takes the
strictest alignment among its members, exactly as a struct does.

### Where endianness bites

Set the channels to `r=0x11, g=0x22, b=0x33, a=0x44`, then read `rgba`:

```
rgba      = 0x44332211
raw bytes = 11 22 33 44
```

The bytes sit in memory in the order written, `r` first at offset 0. x86-64
stores a `uint32_t` little-endian, so the byte at the lowest address is the
least significant byte of the integer. `r` lands in the low 8 bits and `a` in
the high 8 bits, reversing the field order when you print the number.

A color that arrives from a file or the network as `0xRRGGBBAA` therefore
loads backwards. Store it into `.rgba` and read `.components.r`, and you get
the alpha byte.

### Which view to use

Pick the member that matches how the data arrived.

- Four separate channels: write and read `.components`.
- One packed word: write and read `.rgba`, and extract channels with shifts and
  masks, such as `(rgba >> 16) & 0xFF`.

Shifts operate on the value, not on the byte order, so they behave the same on
every machine.

---

## 3. Fixed-Width Integers

`uint8_t` is an unsigned integer of exactly 8 bits. It comes from `<stdint.h>`.

Exactly 8 bits, not "at least 8". Range 0 to 255. Size 1 byte, alignment 1.

`char`, `short`, and `int` carry only minimum widths, so `int` is 32 bits here
and 16 bits on some embedded targets. `uint8_t` is identical everywhere it
exists. Wire formats, file headers, and pixel data all use the fixed-width
types for that reason.

On Linux x86-64 `uint8_t` is a typedef for `unsigned char`.

### The family

```
uint8_t   0 .. 255              int8_t   -128 .. 127
uint16_t  0 .. 65535            int16_t  -32768 .. 32767
uint32_t  0 .. 4294967295       int32_t
uint64_t                        int64_t
```

### Unsigned types wrap

A `uint8_t` at 255 plus 1 becomes 0. At 0 minus 1 it becomes 255. The value
reduces modulo 256. This is defined behavior, not UB.

A countdown loop written `for (uint8_t i = n; i >= 0; i--)` never ends, because
`i >= 0` holds for every unsigned value.

### Arithmetic promotes to int

```c
uint8_t x = 200, y = 100;
int sum = x + y;        // 300, not 44
```

Both operands widen to `int` before the addition, which happens in 32 bits.
Truncation occurs only when you store the result back into a `uint8_t`, so
`x + y` gives 300 while `x += y` gives 44.

Integer promotion is also why `printf("%d", some_uint8)` works.

### Printing

`%hhu` works. The portable spelling uses the macros in `<inttypes.h>`:

```c
printf("%" PRIu8 "\n", x);      // decimal
printf("0x%02" PRIx8 "\n", x);  // hex, padded to 2 digits
```

`vendor/munit/munit_bootdev.h` prints `uint8_t` with `0x%02" PRIx8` so byte
comparisons read as bytes.

### Why uint8_t suits byte layouts

Three properties line up.

1. The width is exact, so `uint8_t raw[8]` occupies exactly 8 bytes.
2. The alignment is 1, so arrays and structs of them carry no padding.
3. As `unsigned char` underneath, it is exempt from strict aliasing.

The third property is the reason byte-inspection code uses `uint8_t *` or
`unsigned char *`. You may point either at any object and read its bytes. An
`int *` aimed at the same object breaks the aliasing rule.

```c
typedef union PacketHeader {
  struct { uint16_t src_port; uint16_t dest_port; uint32_t seq_num; } tcp_header;
  uint8_t raw[8];
} packet_header_t;
```

---

## 4. Alignment

Alignment is a rule about addresses. A type that is N-aligned may only sit at
an address that is a multiple of N.

A 4-aligned `int` can live at 1000, 1004, or 1008. It cannot live at 1001.

### Why the rule exists

The CPU fetches memory in fixed-size chunks, 8 bytes on x86-64, starting at
chunk boundaries. A 4-byte `int` at a multiple of 4 always falls inside one
chunk, so one fetch retrieves it.

The same `int` at address 1001 straddles two chunks. The hardware then needs
two fetches and a merge. x86-64 tolerates that at a cost in cycles. ARM and
many embedded targets fault instead. Some SIMD instructions fault even on x86.

C avoids the question. Reading an object through a misaligned pointer is
undefined behavior, so the compiler assumes it never happens and emits the fast
instruction unconditionally.

### Alignments on this machine

```
char = 1   short = 2   int = 4   long = 8   double = 8   pointer = 8
```

For scalar types, alignment equals size.

"1-aligned" follows from that. `uint8_t` is 1 byte, every address is a multiple
of 1, and nothing can ever be misaligned at 1.

### Padding

A struct must satisfy every member's alignment at once, so the compiler inserts
unnamed padding. Field order changes the total:

```c
struct Bad  { uint8_t a; uint32_t b; uint8_t c; };   // size 12
struct Good { uint32_t b; uint8_t a; uint8_t c; };   // size 8
```

Measured offsets:

```
Bad :  a@0   [3 pad]   b@4   c@8   [3 tail pad]   = 12
Good:  b@0             a@4   c@5   [2 tail pad]   =  8
```

`b` is 4-aligned, so in `Bad` it cannot start at offset 1. Three bytes push it
out to offset 4. In `Good`, `b` takes offset 0 and the two 1-aligned bytes tuck
in behind it.

Two rules produce this:

1. A struct's alignment is the largest alignment among its members. Both
   structs here are 4-aligned because of the `uint32_t`.
2. A struct's size rounds up to a multiple of its own alignment. That is the
   tail padding, and it exists so that every element of an array of these
   structs still lands on a legal address.

Ordering members from largest to smallest usually minimizes padding.

### Consequences elsewhere

The color union measures `sizeof = 4, alignof = 4`. The alignment comes from
`uint32_t`, and the four `uint8_t` channels demand none of their own, so the
struct view carries zero padding and overlays the integer view cleanly. Widen
one channel to `uint16_t` and padding appears, and the overlay stops describing
the same bytes.

`malloc` returns memory aligned strictly enough for any type, 16 bytes on
x86-64. The allocator covers the worst case, which is why allocating a
`snek_object_t` never raises the question.

### Tools

From `<stddef.h>` and the language itself:

```c
sizeof(T)              // bytes, including padding
_Alignof(T)            // required alignment
offsetof(S, member)    // byte offset of a member within a struct
```

Print all three with `%zu`.
