/*
   References:
   T. Nishimura, ``Tables of 64-bit Mersenne Twisters'' (马特赛特旋转演算法)
     ACM Transactions on Modeling and 
     Computer Simulation 10. (2000) 348--357.
   M. Matsumoto and T. Nishimura,
     ``Mersenne Twister: a 623-dimensionally equidistributed
       uniform pseudorandom number generator''
     ACM Transactions on Modeling and 
     Computer Simulation 8. (Jan. 1998) 3--30.

   Any feedback is very welcome.
   http://www.math.hiroshima-u.ac.jp/~m-mat/MT/emt.html
   email: m-mat @ math.sci.hiroshima-u.ac.jp (remove spaces)
*/

#ifndef __MTL_RAND_HH__
#define __MTL_RAND_HH__

#include "mtl_defs.hh"
#include "mtl_apidefs.hh"
#include <stdint.h>

class MtlRand
{
public:
  MtlRand(uint64_t d);

  void seed(uint64_t d);
  uint64_t random();
  double drandom();

private:
  uint64_t mt[312];
  int mti;
};

inline int mtl_rand_r(uint32_t * p) {
  return (((*p) = (*p) * 1103515245 + 12345) % ((uint32_t) 0x7fffffff + 1));
}

#endif

