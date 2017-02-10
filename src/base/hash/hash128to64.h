#ifndef UTIL_HASH_HASH128TO64_H_
#define UTIL_HASH_HASH128TO64_H_

#include "base/core/int128.h"
#include "base/core/integral_types.h"

namespace base {

// Hash 128 input bits down to 64 bits of output.
// This is intended to be a reasonably good hash function.
// It may change from time to time.
inline uint64 Hash128to64(const uint128& x) {
  // Murmur-inspired hashing.
  const uint64 kMul = 0xc6a4a7935bd1e995ULL;
  uint64 a = (Uint128Low64(x) ^ Uint128High64(x)) * kMul;
  a ^= (a >> 47);
  uint64 b = (Uint128High64(x) ^ a) * kMul;
  b ^= (b >> 47);
  b *= kMul;
  return b;
}

} // namespace base
#endif  // UTIL_HASH_HASH128TO64_H_
