#ifndef BASE_CORE_CALLBACK_FORWARD_H_
#define BASE_CORE_CALLBACK_FORWARD_H_

namespace base {

template <typename Sig>
class Callback;

typedef Callback<void(void)> Closure;

}  // namespace base

#endif  // BASE_CORE_CALLBACK_FORWARD_H
