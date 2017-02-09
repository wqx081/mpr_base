// Additional constants from logging.h and its dependencies which are
// not exported by glog.

#ifndef _LOGGING_IN_H_
#define _LOGGING_IN_H_

// DFATAL is FATAL in debug mode, ERROR in normal mode
#ifdef NDEBUG
#define DFATAL_LEVEL ERROR
#else
#define DFATAL_LEVEL FATAL
#endif

// NDEBUG usage helpers related to (RAW_)DCHECK:
//
// DEBUG_MODE is for small !NDEBUG uses like
//   if (DEBUG_MODE) foo.CheckThatFoo();
// instead of substantially more verbose
//   #ifndef NDEBUG
//     foo.CheckThatFoo();
//   #endif
//
#ifdef NDEBUG
const bool DEBUG_MODE = false;
#else
const bool DEBUG_MODE = true;
#endif

#endif  // _LOGGING_IN_H_
