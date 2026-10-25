#include "array_string.h"

#include "string_utils.h"

void process_array(char *s, std::size_t len, std::FILE *out) {
  std::size_t first = len;
  std::size_t last = len;
  for (std::size_t i = 0; i < len; ++i) {
    if (s[i] == ':') {
      if (first == len) {
        first = i;
      }
      last = i;
    }
  }

  if (first == len) { // двоеточий нет
    std::fputs("Двоеточий в строке нет\n", out);
    return;
  }

  if (first != last) {
    for (std::size_t i = first + 1; i < last; ++i) {
      std::fputc(s[i], out);
    }
    std::fputc('\n', out);
    return;
  }

  for (std::size_t i = first + 1; i < len; ++i) {
    std::fputc(s[i], out);
  }
  std::fputc('\n', out);

  swap_first_last_letters(s, len);
  for (std::size_t i = 0; i < len; ++i) {
    std::fputc(s[i], out);
  }
  std::fputc('\n', out);
}
