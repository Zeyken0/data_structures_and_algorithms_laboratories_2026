#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include "array_string.h"

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
  process_array(s, input.size(), out);
  std::fflush(out);
  std::rewind(out);

  char buf[512];
  const std::size_t n = std::fread(buf, 1, sizeof buf, out);
  std::fclose(out);

  return {std::string(buf, n), std::string(s, input.size())};
}

} // namespace

TEST_CASE("Массив: символы между первым и последним двоеточием", "[array]") {
  const Result r = process("до:слово:после");
  REQUIRE(r.output == "слово\n");
  REQUIRE(r.text == "до:слово:после");
}

TEST_CASE("Массив: больше двух двоеточий", "[array]") {
  const Result r = process("a:b:c:d");
  REQUIRE(r.output == "b:c\n");
  REQUIRE(r.text == "a:b:c:d");
}

TEST_CASE("Массив: соседние двоеточия дают пустой фрагмент", "[array]") {
  const Result r = process("a::b");
  REQUIRE(r.output == "\n");
  REQUIRE(r.text == "a::b");
}

TEST_CASE("Массив: единственное двоеточие — символы после него", "[array]") {
  const Result r = process("ab:cd");
  REQUIRE(r.output == "cd\ndb:ca\n");
  REQUIRE(r.text == "db:ca");
}

TEST_CASE("Массив: двоеточие в начале строки", "[array]") {
  const Result r = process(":abc");
  REQUIRE(r.output == "abc\n:cba\n");
  REQUIRE(r.text == ":cba");
}

TEST_CASE("Массив: двоеточие в конце строки", "[array]") {
  const Result r = process("abc:");
  REQUIRE(r.output == "\ncba:\n");
  REQUIRE(r.text == "cba:");
}

TEST_CASE("Массив: двоеточий нет", "[array]") {
  const Result r = process("abcdef");
  REQUIRE(r.output == "Двоеточий в строке нет\n");
  REQUIRE(r.text == "abcdef");
}

TEST_CASE("Массив: пустая строка", "[array]") {
  const Result r = process("");
  REQUIRE(r.output == "Двоеточий в строке нет\n");
  REQUIRE(r.text == "");
}

TEST_CASE("Массив: кириллица, единственное двоеточие", "[array]") {
  const Result r = process("привет:мир");
  REQUIRE(r.output == "мир\nрривет:мип\n");
  REQUIRE(r.text == "рривет:мип");
}

TEST_CASE("Массив: кириллица, два двоеточия", "[array]") {
  const Result r = process("до:слово:после");
  REQUIRE(r.output == "слово\n");
  REQUIRE(r.text == "до:слово:после");
}

TEST_CASE("Массив: разная ширина букв (латиница и кириллица)", "[array]") {
  SECTION("первая буква латинская, последняя кириллическая") {
    const Result r = process("a:бв");
    REQUIRE(r.output == "бв\nв:бa\n");
    REQUIRE(r.text == "в:бa");
  }
  SECTION("первая буква кириллическая, последняя латинская") {
    const Result r = process("мир:ok");
    REQUIRE(r.output == "ok\nkир:oм\n");
    REQUIRE(r.text == "kир:oм");
  }
}

TEST_CASE("Массив: одно двоеточие, но букв в строке нет", "[array]") {
  const Result r = process("12:345");
  REQUIRE(r.output == "345\n12:345\n");
  REQUIRE(r.text == "12:345");
}

TEST_CASE("Массив: только одна буква в строке", "[array]") {
  const Result r = process("a:1");
  REQUIRE(r.output == "1\na:1\n");
  REQUIRE(r.text == "a:1");
}
