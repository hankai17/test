#include "mtl_code.hh"
#include <string.h>
#include <stdio.h>
#include "mtl_assert.hh"

int
mtl_code_incr_md5_init(MTL_DIGEST_CTX * context) {
  return MD5_Init(context);
}

int
mtl_code_incr_md5_update(MTL_DIGEST_CTX * context, const char *input, int input_length) {
  return MD5_Update(context, input, input_length);
}

int
mtl_code_incr_md5_final(char *sixteen_byte_hash_pointer, MTL_DIGEST_CTX * context) {
  return MD5_Final((unsigned char*)sixteen_byte_hash_pointer, context);
}

int
mtl_code_md5(unsigned char *input, int input_length, unsigned char *sixteen_byte_hash_pointer)
{
  MD5_CTX context;
  MD5_Init(&context);
  MD5_Update(&context, input, input_length);
  MD5_Final(sixteen_byte_hash_pointer, &context);
  return 0;
}

/**
  @brief Converts a MD5 to a null-terminated string

  Externalizes an INK_MD5 as a null-terminated string into the first argument.
  Side Effects: none
  Reentrancy:     n/a.
  Thread Safety:  safe.
  Mem Management: stomps the passed dest char*.

  @return returns the passed destination string ptr.
*/
/* reentrant version */
char *
mtl_code_md5_stringify(char *dest33, const size_t destSize, const char *md5)
{
  mtl_assert(destSize >= 33);

  int i;
  for (i = 0; i < 16; i++) {
    // we check the size of the destination buffer above
    // coverity[secure_coding]
    sprintf(&(dest33[i * 2]), "%02X", (unsigned char)md5[i]);
    //sprintf(&(dest33[i * 2 + 1]), "%02X", md5[i] << 4);
  }
  mtl_assert(dest33[32] == '\0');
  return (dest33);
}                               /* End mtl_code_stringify_md5(const char *md5) */

/**
  @brief Converts a MD5 to a null-terminated string

  Externalizes an INK_MD5 as a null-terminated string into the first argument.
  Does so without intenal procedure calls.
  Side Effects: none.
  Reentrancy:     n/a.
  Thread Safety:  safe.
  Mem Management: stomps the passed dest char*.

  @return returns the passed destination string ptr.
*/
/* reentrant version */
char *
mtl_code_md5_stringify_fast(char *dest33, const char *md5)
{
  int i;
  char *d;

  static char hex_digits[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

  d = dest33;
  for (i = 0; i < 16; i += 4) {
    *(d + 0) = hex_digits[((unsigned char) md5[i + 0]) >> 4];
    *(d + 1) = hex_digits[((unsigned char) md5[i + 0]) & 15];
    *(d + 2) = hex_digits[((unsigned char) md5[i + 1]) >> 4];
    *(d + 3) = hex_digits[((unsigned char) md5[i + 1]) & 15];
    *(d + 4) = hex_digits[((unsigned char) md5[i + 2]) >> 4];
    *(d + 5) = hex_digits[((unsigned char) md5[i + 2]) & 15];
    *(d + 6) = hex_digits[((unsigned char) md5[i + 3]) >> 4];
    *(d + 7) = hex_digits[((unsigned char) md5[i + 3]) & 15];
    d += 8;
  }
  *d = '\0';
  return (dest33);
}                               /* End mtl_code_stringify_md5_fast */

