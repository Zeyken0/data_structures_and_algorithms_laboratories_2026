#pragma once

#include <cstddef>

inline constexpr std::size_t kMaxLineLength = 4096;

void swap_first_last_letters(char *s, std::size_t len);
