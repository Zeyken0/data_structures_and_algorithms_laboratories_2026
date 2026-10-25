#include "c_string.h"
#include "string_utils.h"

#include <cstdio>
#include <cstring>

int main() {
  char s[kMaxLineLength];

  std::fputs("Введите строку: ", stdout);
  if (std::fgets(s, static_cast<int>(sizeof s), stdin) == nullptr) {
    std::fputs("Ошибка чтения строки\n", stderr);
    return 1;
  }
  s[std::strcspn(s, "\n")] = '\0';

  process_c_string(s, stdout);
  return 0;
}
