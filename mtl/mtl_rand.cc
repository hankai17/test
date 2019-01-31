//#include "libts.h"
#include "mtl_rand.hh"

#define NN 312
#define MM 156
#define MATRIX_A 0xB5026F5AA96619E9ULL
#define UM 0xFFFFFFFF80000000ULL /* Most significant 33 bits */
#define LM 0x7FFFFFFFULL /* Least significant 31 bits */

static uint64_t mag01[2]={0ULL, MATRIX_A};

MtlRand::MtlRand(uint64_t d) {
  seed(d);
}

void MtlRand::seed(uint64_t seed) {
  mt[0] = seed;
  for (mti = 1; mti < NN; mti++) 
    mt[mti] =  (6364136223846793005ULL * (mt[mti-1] ^ (mt[mti-1] >> 62)) + mti);
}

uint64_t MtlRand::random() {
  int i;
  uint64_t x;

  if (mti >= NN) { /* generate NN words at one time */
    for (i = 0; i < NN - MM; i++) {
      x = (mt[i]&UM)|(mt[i+1]&LM);
      mt[i] = mt[i + MM] ^ (x>>1) ^ mag01[(int)(x&1ULL)];
    }
    for (; i < NN - 1; i++) {
      x = (mt[i]&UM)|(mt[i+1]&LM);
      mt[i] = mt[i+(MM-NN)] ^ (x>>1) ^ mag01[(int)(x&1ULL)];
    }
    x = (mt[NN-1]&UM)|(mt[0]&LM);
    mt[NN-1] = mt[MM-1] ^ (x>>1) ^ mag01[(int)(x&1ULL)];

    mti = 0;
  }

  x = mt[mti++];

  x ^= (x >> 29) & 0x5555555555555555ULL;
  x ^= (x << 17) & 0x71D67FFFEDA60000ULL;
  x ^= (x << 37) & 0xFFF7EEE000000000ULL;
  x ^= (x >> 43);

  return x;
}

double MtlRand::drandom() {
  return (random() >> 11) * (1.0/9007199254740991.0);
}

