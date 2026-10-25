#include "file_string.h"

#include "c_string.h"
#include "string_utils.h"

#include <cstdio>
#include <cstring>

bool process_file(const char *in_path, const char *out_path) {
  std::FILE *in = std::fopen(in_path, "r");
  if (in == nullptr) {
    return false;
  }

  char s[kMaxLineLength];
  if (std::fgets(s, static_cast<int>(sizeof s), in) == nullptr) {
    s[0] = '\0';
  }
  std::fclose(in);
  s[std::strcspn(s, "\n")] = '\0';

  std::FILE *out = std::fopen(out_path, "w");
  if (out == nullptr) {
    return false;
  }
  process_c_string(s, out);
  std::fclose(out);
  return true;
}
