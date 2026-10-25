#include "string_utils.h"

#include <cctype>

namespace {

std::size_t letter_width(const char *s, std::size_t i, std::size_t len) {
  const unsigned char c = static_cast<unsigned char>(s[i]);
  if (c < 0x80) {
    return std::isalpha(c) ? 1 : 0;
  }
  if ((c == 0xD0 || c == 0xD1) && i + 1 < len &&
      (static_cast<unsigned char>(s[i + 1]) & 0xC0) == 0x80) {
    return 2;
  }
  return 0;
}

// [first, last)
void reverse(char *first, char *last) {
  while (first < --last) {
    const char t = *first;
    *first++ = *last;
    *last = t;
  }
}

} // namespace

void swap_first_last_letters(char *s, std::size_t len) {
  std::size_t first = 0;
  std::size_t first_width = 0;
  std::size_t last = 0;
  std::size_t last_width = 0;
  bool found = false;

  for (std::size_t i = 0; i < len;) {
    const std::size_t w = letter_width(s, i, len);
    if (w == 0) {
      ++i;
      continue;
    }
    if (!found) {
      found = true;
      first = i;
      first_width = w;
    }
    last = i;
    last_width = w;
    i += w;
  }

  if (!found || first == last) {
    return;
  }

  reverse(s + first, s + first + first_width);
  reverse(s + first + first_width, s + last);
  reverse(s + last, s + last + last_width);
  reverse(s + first, s + last + last_width);
}
