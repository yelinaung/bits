#include <stdint.h>

// [u]int<N>_t
// u means unsigned and N is the exact bit width:
//
// uint8_t   0 .. 255              int8_t   -128 .. 127
// uint16_t  0 .. 65535            int16_t  -32768 .. 32767
// uint32_t  0 .. 4294967295       int32_t  ...
// uint64_t                        int64_t

/*
 * sizeof union      = 4    alignof = 4
 * sizeof components = 4
 * offsets: r=0  g=1  b=2  a=3
 * offset:    0        1        2        3
 *      ┌────────┬────────┬────────┬────────┐
 *      │   r    │   g    │   b    │   a    │   ← .components
 *      ├────────┴────────┴────────┴────────┤
 *      │            rgba (uint32_t)        │   ← .rgba
 *      └───────────────────────────────────┘
 */

typedef union Color {
  struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
  } components;
  uint32_t rgba;
} color_t;

typedef union PacketHeader {
  struct {
    uint16_t src_port;
    uint16_t dest_port;
    uint32_t seq_num;
  } tcp_header;
  uint8_t raw[8];
} packet_header_t;
