#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <string.h>
#include <ctype.h>
#include <arpa/inet.h>

#pragma pack(1)
struct pack1 {
  uint32_t opcode:4;
  bool rsv3:1;
  bool rsv2:1;
  bool rsv1:1;
  bool fin:1;
  uint32_t payload:7;
  bool mask:1;
};
#pragma pack()

struct pack {
  bool fin:1;
  bool rsv1:1;
  bool rsv2:1;
  bool rsv3:1;
  uint32_t opcode: 4;
  bool mask:1;
  uint32_t payload: 7;
};

int main() {
  //uint32_t a = 0x12345678;
  //printf("%x\n", ((char*)&a)[0]);

  //unsigned char buf[] = {(unsigned char)0x01, (unsigned char)0x81};
  unsigned char buf[] = {(unsigned char)0x81, (unsigned char)0x01};
  struct pack p;
  memcpy(&p, buf, 2);
  printf("p.fin: %d\n", p.fin);
  printf("p.rsv1: %d\n", p.rsv1);
  printf("p.rsv2: %d\n", p.rsv2);
  printf("p.rsv3: %d\n", p.rsv3);

  printf("p.opcode: %d\n", p.opcode);
  printf("p.mask: %d\n", p.mask);
  printf("p.payload: %d\n", p.payload);

  return 0;
}
