#ifndef GUTIL_WALLTIME_H_
#define GUTIL_WALLTIME_H_

#include <sys/time.h>

#include <glog/logging.h>
#include <string>
using std::string;

#if defined(__APPLE__)
#include <mach/clock.h>
#include <mach/mach.h>
#include <mach/mach_time.h>

#include "kudu/gutil/once.h"
#endif  // defined(__APPLE__)

#include "base/core/integral_types.h"

namespace base {

typedef double WallTime;

// Append result to a supplied string.
// If an error occurs during conversion 'dst' is not modified.
void StringAppendStrftime(std::string* dst,
                                 const char* format,
                                 time_t when,
                                 bool local);

// Return the local time as a string suitable for user display.
std::string LocalTimeAsString();

// Similar to the WallTime_Parse, but it takes a boolean flag local as
// argument specifying if the time_spec is in local time or UTC
// time. If local is set to true, the same exact result as
// WallTime_Parse is returned.
bool WallTime_Parse_Timezone(const char* time_spec,
                                    const char* format,
                                    const struct tm* default_time,
                                    bool local,
                                    WallTime* result);

// Return current time in seconds as a WallTime.
WallTime WallTime_Now();

typedef int64 MicrosecondsInt64;

namespace walltime_internal {

#if defined(__APPLE__)

extern GoogleOnceType timebase_info_once;
extern mach_timebase_info_data_t timebase_info;
extern void InitializeTimebaseInfo();

inline void GetCurrentTime(mach_timespec_t* ts) {
  clock_serv_t cclock;
  host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
  clock_get_time(cclock, ts);
  mach_port_deallocate(mach_task_self(), cclock);
}

inline MicrosecondsInt64 GetCurrentTimeMicros() {
  mach_timespec_t ts;
  GetCurrentTime(&ts);
  return ts.tv_sec * 1e6 + ts.tv_nsec / 1e3;
}

inline int64_t GetMonoTimeNanos() {
  // See Apple Technical Q&A QA1398 for further detail on mono time in OS X.
  GoogleOnceInit(&timebase_info_once, &InitializeTimebaseInfo);

  uint64_t time = mach_absolute_time();

  // mach_absolute_time returns ticks, which need to be scaled by the timebase
  // info to get nanoseconds.
  return time * timebase_info.numer / timebase_info.denom;
}

inline MicrosecondsInt64 GetMonoTimeMicros() {
  return GetMonoTimeNanos() / 1e3;
}

inline MicrosecondsInt64 GetThreadCpuTimeMicros() {
  // See https://www.gnu.org/software/hurd/gnumach-doc/Thread-Information.html
  // and Chromium base/time/time_mac.cc.
  task_t thread = mach_thread_self();
  if (thread == MACH_PORT_NULL) {
    LOG(WARNING) << "Failed to get mach_thread_self()";
    return 0;
  }

  mach_msg_type_number_t thread_info_count = THREAD_BASIC_INFO_COUNT;
  thread_basic_info_data_t thread_info_data;

  kern_return_t result = thread_info(
      thread,
      THREAD_BASIC_INFO,
      reinterpret_cast<thread_info_t>(&thread_info_data),
      &thread_info_count);

  if (result != KERN_SUCCESS) {
    LOG(WARNING) << "Failed to get thread_info()";
    return 0;
  }

  return thread_info_data.user_time.seconds * 1e6 + thread_info_data.user_time.microseconds;
}

#else

inline MicrosecondsInt64 GetClockTimeMicros(clockid_t clock) {
  timespec ts;
  clock_gettime(clock, &ts);
  return ts.tv_sec * 1e6 + ts.tv_nsec / 1e3;
}

#endif // defined(__APPLE__)

} // namespace walltime_internal

// Returns the time since the Epoch measured in microseconds.
inline MicrosecondsInt64 GetCurrentTimeMicros() {
#if defined(__APPLE__)
  return walltime_internal::GetCurrentTimeMicros();
#else
  return walltime_internal::GetClockTimeMicros(CLOCK_REALTIME);
#endif  // defined(__APPLE__)
}

// Returns the time since some arbitrary reference point, measured in microseconds.
// Guaranteed to be monotonic (and therefore useful for measuring intervals)
inline MicrosecondsInt64 GetMonoTimeMicros() {
#if defined(__APPLE__)
  return walltime_internal::GetMonoTimeMicros();
#else
  return walltime_internal::GetClockTimeMicros(CLOCK_MONOTONIC);
#endif  // defined(__APPLE__)
}

// Returns the time spent in user CPU on the current thread, measured in microseconds.
inline MicrosecondsInt64 GetThreadCpuTimeMicros() {
#if defined(__APPLE__)
  return walltime_internal::GetThreadCpuTimeMicros();
#else
  return walltime_internal::GetClockTimeMicros(CLOCK_THREAD_CPUTIME_ID);
#endif  // defined(__APPLE__)
}

// A CycleClock yields the value of a cycle counter that increments at a rate
// that is approximately constant.
class CycleClock {
 public:
  // Return the value of the counter.
  static inline int64 Now();

 private:
  CycleClock();
};

} // namespace base

#include "base/time/cycleclock-inl.h"  // inline method bodies

#endif  // GUTIL_WALLTIME_H_
