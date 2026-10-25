#include "file_string.h"

#include <cstdio>

int main() {
  if (!process_file("file1.txt", "file2.txt")) {
    std::fputs("Ошибка: не удалось открыть файл для чтения или записи\n",
               stderr);
    return 1;
  }

  std::fputs("Готово: результат записан в file2.txt\n", stdout);
  return 0;
}
