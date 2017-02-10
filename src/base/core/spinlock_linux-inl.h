// -*- Mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*-

#include <errno.h>
#include <sched.h>
#include <time.h>
#include <limits.h>
#include "base/core/linux_syscall_support.h"

#define FUTEX_WAIT 0
#define FUTEX_WAKE 1
#define FUTEX_PRIVATE_FLAG 128

static bool have_futex;
static int futex_private_flag = FUTEX_PRIVATE_FLAG;

namespace {
static struct InitModule {
  InitModule() {
    int x = 0;
    // futexes are ints, so we can use them only when
    // that's the same size as the lockword_ in SpinLock.
#ifdef __arm__
    // ARM linux doesn't support sys_futex1(void*, int, int, struct timespec*);
    have_futex = 0;
#else
    have_futex = (sizeof (Atomic32) == sizeof (int) &&
                  sys_futex(&x, FUTEX_WAKE, 1, 0) >= 0);
#endif
    if (have_futex &&
        sys_futex(&x, FUTEX_WAKE | futex_private_flag, 1, 0) < 0) {
      futex_private_flag = 0;
    }
  }
} init_module;

}  // anonymous namespace


namespace base {
namespace internal {

void SpinLockDelay(volatile Atomic32 *w, int32 value, int loop) {
  if (loop != 0) {
    int save_errno = errno;
    struct timespec tm;
    tm.tv_sec = 0;
    if (have_futex) {
      tm.tv_nsec = base::internal::SuggestedDelayNS(loop);
    } else {
      tm.tv_nsec = 2000001;   // above 2ms so linux 2.4 doesn't spin
    }
    if (have_futex) {
      tm.tv_nsec *= 16;  // increase the delay; we expect explicit wakeups
      sys_futex(reinterpret_cast<int *>(const_cast<Atomic32 *>(w)),
                FUTEX_WAIT | futex_private_flag,
                value, reinterpret_cast<struct kernel_timespec *>(&tm));
    } else {
      nanosleep(&tm, NULL);
    }
    errno = save_errno;
  }
}

void SpinLockWake(volatile Atomic32 *w, bool all) {
  if (have_futex) {
    sys_futex(reinterpret_cast<int *>(const_cast<Atomic32 *>(w)),
              FUTEX_WAKE | futex_private_flag, all? INT_MAX : 1, 0);
  }
}

} // namespace internal
} // namespace base
