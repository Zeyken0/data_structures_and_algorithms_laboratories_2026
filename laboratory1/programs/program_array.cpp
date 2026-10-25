#include "array_string.h"
#include "string_utils.h"

#include <cstdio>

int main() {
  char s[kMaxLineLength];

  std::fputs("Введите строку: ", stdout);
  if (std::fgets(s, static_cast<int>(sizeof s), stdin) == nullptr) {
    std::fputs("Ошибка чтения строки\n", stderr);
    return 1;
  }

  std::size_t len = 0;
  while (len < kMaxLineLength && s[len] != '\n' && s[len] != '\0') {
    ++len;
  }

  process_array(s, len, stdout);
  return 0;
}
