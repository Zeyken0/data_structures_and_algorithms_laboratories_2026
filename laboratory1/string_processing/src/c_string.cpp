#include "c_string.h"

#include "string_utils.h"

#include <cstring>

void process_c_string(char *s, std::FILE *out) {
  const char *first = std::strchr(s, ':');
  if (first == nullptr) {
    std::fputs("Двоеточий в строке нет\n", out);
    return;
  }
  const char *last = std::strrchr(s, ':');

  if (first != last) {
    for (const char *p = first + 1; p < last; ++p) {
      std::fputc(*p, out);
    }
    std::fputc('\n', out);
    return;
  }

  std::fputs("Срез после двоеточия: ", out);
  std::fputs(first + 1, out);
  std::fputc('\n', out);

  swap_first_last_letters(s, std::strlen(s));
  std::fputs("Обработанная строка: ", out);
  std::fputs(s, out);
  std::fputc('\n', out);
}
