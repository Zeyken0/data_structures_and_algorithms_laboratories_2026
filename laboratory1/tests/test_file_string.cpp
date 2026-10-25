#include <catch.hpp>

#include "file_string.h"

#include <cstdio>
#include <string>

namespace {

struct TestFiles {
  TestFiles(const char *in, const char *out) : in_path(in), out_path(out) {}
  ~TestFiles() {
    std::remove(in_path);
    std::remove(out_path);
  }

  const char *in_path;
  const char *out_path;
};

void write_text(const char *path, const char *text) {
  std::FILE *f = std::fopen(path, "w");
  REQUIRE(f != nullptr);
  std::fputs(text, f);
  std::fclose(f);
}

std::string read_text(const char *path) {
  std::FILE *f = std::fopen(path, "r");
  REQUIRE(f != nullptr);

  char buf[512];
  const std::size_t n = std::fread(buf, 1, sizeof buf, f);
  std::fclose(f);
  return std::string(buf, n);
}

} // namespace

TEST_CASE("Файл: символы между первым и последним двоеточием", "[file]") {
  TestFiles files("test_input.txt", "test_output.txt");
  write_text(files.in_path, "Пример:строки:для:файла\n");

  REQUIRE(process_file(files.in_path, files.out_path));
  REQUIRE(read_text(files.out_path) == "строки:для\n");
}

TEST_CASE("Файл: единственное двоеточие — символы после него", "[file]") {
  TestFiles files("test_input.txt", "test_output.txt");
  write_text(files.in_path, "ab:cd\n");

  REQUIRE(process_file(files.in_path, files.out_path));
  REQUIRE(read_text(files.out_path) == "Срез после двоеточия: cd\n"
                                       "Обработанная строка: db:ca\n");
}

TEST_CASE("Файл: кириллица, единственное двоеточие", "[file]") {
  TestFiles files("test_input.txt", "test_output.txt");
  write_text(files.in_path, "привет:мир\n");

  REQUIRE(process_file(files.in_path, files.out_path));
  REQUIRE(read_text(files.out_path) == "Срез после двоеточия: мир\n"
                                       "Обработанная строка: рривет:мип\n");
}

TEST_CASE("Файл: двоеточий нет", "[file]") {
  TestFiles files("test_input.txt", "test_output.txt");
  write_text(files.in_path, "abcdef\n");

  REQUIRE(process_file(files.in_path, files.out_path));
  REQUIRE(read_text(files.out_path) == "Двоеточий в строке нет\n");
}

TEST_CASE("Файл: пустой входной файл", "[file]") {
  TestFiles files("test_input.txt", "test_output.txt");
  write_text(files.in_path, "");

  REQUIRE(process_file(files.in_path, files.out_path));
  REQUIRE(read_text(files.out_path) == "Двоеточий в строке нет\n");
}

TEST_CASE("Файл: входной файл отсутствует", "[file]") {
  TestFiles files("no_such_input.txt", "test_output.txt");
  REQUIRE_FALSE(process_file(files.in_path, files.out_path));
}
