#include "base/strings/charset.h"

#include <string.h>

namespace base {

CharSet::CharSet() {
  memset(this, 0, sizeof(*this));
}

CharSet::CharSet(const char* characters) {
  memset(this, 0, sizeof(*this));
  for (; *characters != '\0'; ++characters) {
    Add(*characters);
  }
}

CharSet::CharSet(const CharSet& other) {
  memcpy(this, &other, sizeof(*this));
}

}  // namespace base
