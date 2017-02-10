// -*- Mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*-

#ifndef BASE_SPINLOCK_INTERNAL_H_
#define BASE_SPINLOCK_INTERNAL_H_

#include "base/core/basictypes.h"
#include "base/core/atomicops.h"

namespace base {
namespace internal {

// SpinLockWait() waits until it can perform one of several transitions from
// "from" to "to".  It returns when it performs a transition where done==true.
struct SpinLockWaitTransition {
  int32 from;
  int32 to;
  bool done;
};

// Wait until *w can transition from trans[i].from to trans[i].to for some i
// satisfying 0<=i<n && trans[i].done, atomically make the transition,
// then return the old value of *w.   Make any other atomic tranistions
// where !trans[i].done, but continue waiting.
int32 SpinLockWait(volatile Atomic32 *w, int n,
                   const SpinLockWaitTransition trans[]);
void SpinLockWake(volatile Atomic32 *w, bool all);
void SpinLockDelay(volatile Atomic32 *w, int32 value, int loop);

} // namespace internal
} // namespace base
#endif
