#include <catch.hpp>

#include "c_string.h"

#include <cstdio>
#include <cstring>
#include <string>

namespace {

struct Result {
  std::string output;
  std::string text;
};

Result process(const std::string &input) {
  REQUIRE(input.size() < 256);
  char s[256];
  std::memcpy(s, input.c_str(), input.size() + 1);

  std::FILE *out = std::tmpfile();
  REQUIRE(out != nullptr);
  process_c_string(s, out);
  std::fflush(out);
  std::rewind(out);

  char buf[512];
  const std::size_t n = std::fread(buf, 1, sizeof buf, out);
  std::fclose(out);

  return {std::string(buf, n), std::string(s)};
}

} // namespace

TEST_CASE("C-строка: символы между первым и последним двоеточием",
          "[c_string]") {
  const Result r = process("до:слово:после");
  REQUIRE(r.output == "слово\n");
  REQUIRE(r.text == "до:слово:после");
}

TEST_CASE("C-строка: больше двух двоеточий", "[c_string]") {
  const Result r = process("a:b:c:d");
  REQUIRE(r.output == "b:c\n");
  REQUIRE(r.text == "a:b:c:d");
}

TEST_CASE("C-строка: соседние двоеточия дают пустой фрагмент", "[c_string]") {
  const Result r = process("a::b");
  REQUIRE(r.output == "\n");
  REQUIRE(r.text == "a::b");
}

TEST_CASE("C-строка: единственное двоеточие — символы после него",
          "[c_string]") {
  const Result r = process("ab:cd");
  REQUIRE(r.output == "Срез после двоеточия: cd\n"
                      "Обработанная строка: db:ca\n");
  REQUIRE(r.text == "db:ca");
}

TEST_CASE("C-строка: двоеточие в начале строки", "[c_string]") {
  const Result r = process(":abc");
  REQUIRE(r.output == "Срез после двоеточия: abc\n"
                      "Обработанная строка: :cba\n");
  REQUIRE(r.text == ":cba");
}

TEST_CASE("C-строка: двоеточие в конце строки", "[c_string]") {
  const Result r = process("abc:");
  REQUIRE(r.output == "Срез после двоеточия: \n"
                      "Обработанная строка: cba:\n");
  REQUIRE(r.text == "cba:");
}

TEST_CASE("C-строка: двоеточий нет", "[c_string]") {
  const Result r = process("abcdef");
  REQUIRE(r.output == "Двоеточий в строке нет\n");
  REQUIRE(r.text == "abcdef");
}

TEST_CASE("C-строка: пустая строка", "[c_string]") {
  const Result r = process("");
  REQUIRE(r.output == "Двоеточий в строке нет\n");
  REQUIRE(r.text == "");
}

TEST_CASE("C-строка: кириллица, единственное двоеточие", "[c_string]") {
  const Result r = process("привет:мир");
  REQUIRE(r.output == "Срез после двоеточия: мир\n"
                      "Обработанная строка: рривет:мип\n");
  REQUIRE(r.text == "рривет:мип");
}

TEST_CASE("C-строка: кириллица, два двоеточия", "[c_string]") {
  const Result r = process("до:слово:после");
  REQUIRE(r.output == "слово\n");
  REQUIRE(r.text == "до:слово:после");
}

TEST_CASE("C-строка: разная ширина букв (латиница и кириллица)", "[c_string]") {
  SECTION("первая буква латинская, последняя кириллическая") {
    const Result r = process("a:бв");
    REQUIRE(r.output == "Срез после двоеточия: бв\n"
                        "Обработанная строка: в:бa\n");
    REQUIRE(r.text == "в:бa");
  }
  SECTION("первая буква кириллическая, последняя латинская") {
    const Result r = process("мир:ok");
    REQUIRE(r.output == "Срез после двоеточия: ok\n"
                        "Обработанная строка: kир:oм\n");
    REQUIRE(r.text == "kир:oм");
  }
}

TEST_CASE("C-строка: одно двоеточие, но букв в строке нет", "[c_string]") {
  const Result r = process("12:345");
  REQUIRE(r.output == "Срез после двоеточия: 345\n"
                      "Обработанная строка: 12:345\n");
  REQUIRE(r.text == "12:345");
}

TEST_CASE("C-строка: только одна буква в строке", "[c_string]") {
  const Result r = process("a:1");
  REQUIRE(r.output == "Срез после двоеточия: 1\n"
                      "Обработанная строка: a:1\n");
  REQUIRE(r.text == "a:1");
}
