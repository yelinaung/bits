# Endianness

Every number below comes from running the program at the end, on Linux x86-64
with gcc. The machine is little-endian.

Companion note: [notes-memory-layout.md](notes-memory-layout.md).

---

## The definition

Endianness fixes the order in which the bytes of a multi-byte integer sit in
memory. It governs integers and floats. It never governs arrays.

`uint32_t v = 0x12345678` holds four bytes. `12` is the most significant, `78`
the least. Little-endian stores `12` at the highest address. Big-endian stores
it at the lowest:

```text
little-endian (x86-64, ARM default)   big-endian (network, older SPARC/PPC)

addr:  +0   +1   +2   +3              addr:  +0   +1   +2   +3
       78   56   34   12                     12   34   56   78
       ^ least significant first             ^ most significant first
```

This machine prints:

```text
uint32 0x12345678     78 56 34 12
uint16 0xABCD         CD AB
```

The little end, the least significant byte, lands first at the lowest address.
The name records which end goes there.

---

## Why little-endian looks backwards

Decimal notation is big-endian. Write 1234 and the thousands digit comes first
in reading order, the units digit last, exactly as big-endian puts the most
significant byte at the lowest address.

A memory dump runs left to right by address, so a little-endian integer reads
in reverse of how you would write it. You are comparing two orderings, and only
one of them trained your eyes.

Little-endian is the more consistent of the two orders. Byte *i* always holds
bits `8i` through `8i+7`, at every width. Truncating a `uint32_t` to a
`uint16_t` needs no address change: you read fewer bytes from the same spot.

Big-endian matches how people read, which is why the internet protocols and
most file formats adopted it.

The industry runs on both.

---

## What endianness leaves alone

Byte order governs scalars, not sequences:

```text
char[] "ABCD"     41 42 43 44
uint8_t[4]        12 34 56 78
```

Both come out in written order on every machine. An array holds separate
1-byte objects, each with its own address, so no multi-byte integer exists to
reorder.

Arithmetic and bit operations also hold still, because C defines them on the
value, not on the storage:

```text
v >> 24  ==  0x12      everywhere
```

`v >> 24` yields the most significant byte on any host, and
`(rgba >> 16) & 0xFF` extracts the same channel on any host. Shifts and masks
unpack a packed integer portably. A cast to `uint8_t *` does not.

---

## Where it bites

The problem appears only where the same bytes cross between the integer view
and the byte view.

- A binary file or a network packet written on another machine arrives in the
  other order.
- A union overlays the two views. In `packet_header_t`, `raw[0]` holds the low
  byte of `src_port` here and the high byte on a big-endian host.
- A pointer cast reinterprets. `*(uint32_t *)buf` reads four bytes in the local
  order, whatever the data means.
- `memcpy` moves an integer between machines byte for byte.

One program on one machine hits none of these. The compiler agrees with itself.

---

## Network byte order

The internet protocols fix network byte order as big-endian. The conversions
live in `<arpa/inet.h>`:

```text
htonl(0x12345678) = 0x78563412
  as bytes          12 34 56 78
```

Those two lines say different things about the same variable. The printed
*value* changed, because this machine swapped the bytes. The *bytes in memory*
now run `12 34 56 78`, the big-endian layout the wire expects.

Four functions cover the two directions and the two widths:

```text
htons / htonl   host to network,  short (16 bit) / long (32 bit)
ntohs / ntohl   network to host,  short (16 bit) / long (32 bit)
```

A big-endian host compiles all four to nothing. Call them anyway. Code that
calls `htons` runs correctly on either architecture, and code that swaps bytes
by hand runs correctly on one.

---

## Detecting the local order

```c
uint16_t x = 1;
bool little = (*(uint8_t *)&x == 1);
```

A 1 at the lowest address means little-endian. C23 adds
`__STDC_ENDIAN_NATIVE__`, and GCC exposes `__BYTE_ORDER__` today.

Write code that never asks the question:

- Pack and unpack with shifts and masks.
- Call `htonl` and `ntohl` at the I/O boundary.
- Read a parsed buffer one byte at a time.

---

## Test program

```c
#include <arpa/inet.h>
#include <stdint.h>
#include <stdio.h>

static void dump(const char *label, const void *p, size_t n) {
  const unsigned char *b = p;
  printf("%-22s", label);
  for (size_t i = 0; i < n; i++) printf("%02X ", b[i]);
  printf("\n");
}

int main(void) {
  uint32_t v = 0x12345678;
  dump("uint32 0x12345678", &v, 4);

  uint32_t n = htonl(v);
  printf("htonl(0x12345678)     = 0x%08X\n", n);
  dump("  as bytes", &n, 4);

  char str[] = "ABCD";
  dump("char[] \"ABCD\"", str, 4);

  printf("v >> 24               = 0x%02X\n", v >> 24);
  return 0;
}
```
