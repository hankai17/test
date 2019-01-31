#ifndef _MTL_MD5_hh_
#define	_MTL_MD5_hh_

#include "mtl_code.hh"
#include "mtl_defs.hh"
#include <stdint.h>

struct MTL_MD5
{

  // This union is needed to fix strict-aliasing warnings.
  // It is anonymous so that the 'b' member can remain in
  // the top level scope of the struct. This is because
  // other code touches that directly. Once that code is
  // also fixed we can make this an Alias64 type from
  // mtl_align.h
  union
  {
    uint64_t b[2];   // Legacy placeholder
    uint64_t u64[2];
    uint32_t u32[4];
    uint8_t  u8[16];
  };

  const MTL_MD5 & operator =(const MTL_MD5 & md5)
  {
    u64[0] = md5.u64[0];
    u64[1] = md5.u64[1];
    return md5;
  }
  uint32_t word(int i)
  {
    return u32[i];
  }
  unsigned char byte(int i)
  {
    return u8[i];
  }
  MTL_MD5 & loadFromBuffer(char *md5_buf) {
    memcpy((void *) u8, (void *) md5_buf, 16);
    return (*this);
  }
  MTL_MD5 & storeToBuffer(char *md5_buf) {
    memcpy((void *) md5_buf, (void *) u8, 16);
    return (*this);
  }
  MTL_MD5 & operator =(char *md5) {
    return (loadFromBuffer(md5));
  }
  MTL_MD5 & operator =(unsigned char *md5) {
    return (loadFromBuffer((char *) md5));
  }

  char *toStr(char *md5_str)
  {
    return (char *) memcpy((void *) md5_str, (void *) u8, 16);
  }
  void encodeBuffer(unsigned char *buffer, int len)
  {
    mtl_code_md5(buffer, len, u8);
  }
  void encodeBuffer(const char *buffer, int len)
  {
    encodeBuffer((unsigned char *) buffer, len);
  }
  char *str()
  {
    return ((char *) u8);
  }
  char *toHexStr(char hex_md5[33])
  {
    return (mtl_code_md5_stringify_fast(hex_md5, str()));
  }
  void set(MTL_MD5 & md5)
  {
    loadFromBuffer((char *) md5.u8);
  }
  void set(MTL_MD5 * md5)
  {
    loadFromBuffer((char *) md5->u8);
  }
  void set(char *p)
  {
    loadFromBuffer(p);
  }
  void set(uint64_t a1, uint64_t a2)
  {
    u64[0] = a1;
    u64[1] = a2;
  }
  char *string(char buf[33])
  {
    char hex_digits[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
    int i, j;

    for (i = 0, j = 0; i < 16; i += 1, j += 2) {
      buf[j + 0] = hex_digits[u8[i] >> 4];
      buf[j + 1] = hex_digits[u8[i] & 0xF];
    }
    buf[32] = '\0';
    return buf;
  }

  uint64_t fold() const
  {
    return (u64[0] ^ u64[1]);
  }

  uint64_t operator[] (int i) const
  {
    return u64[i];
  }
  bool operator==(MTL_MD5 const& md5)
  {
    return u64[0] == md5.u64[0] && u64[1] == md5.u64[1];
  }
  MTL_MD5() {
    u64[0] = 0;
    u64[1] = 0;
  }
  MTL_MD5(uint64_t a1, uint64_t a2) {
    u64[0] = a1;
    u64[1] = a2;
  }
};

#endif
