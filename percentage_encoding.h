#pragma once

#include <string>
#include <numeric>
#include <vector>
#include <tuple>
#include <iostream>

enum decoder_state
{
  S0 = 0,
  S1 = 1,
  S2 = 2
};

constexpr auto is_printable(char c) -> bool
{
  return (c >= '0' && c <= '9') 
          || (c >= 'a' && c <= 'z')
          || (c >= 'A' && c <= 'Z');
}

constexpr auto has_hexa_base(char c) -> bool
{
  return (c >= 'a' && c <= 'f')
          || (c >= 'A' && c <= 'F') 
          || (c >= '0' && c <= '9');
}

auto percentage_decode(const std::string &input) -> std::string
{
  return std::get<1>(std::accumulate(
    begin(input), 
    end(input), 
    std::make_tuple(decoder_state::S0, std::string{}, char{0}), 
    [](const std::tuple<decoder_state, std::string, char> &context, char c) {
      const auto current_state = std::get<0>(context);
      const auto acc = std::get<1>(context);
      const auto comp1 = std::get<2>(context);

      //std::cout << std::endl
      //          << "On S" << current_state 
      //          << "  : acc: \"" << acc << "\""
      //          << "    c: " << c 
      //          << "    comp1: " << comp1 
      //          << std::endl;

      if (current_state == S0) {
        if (c == '+') {
          return std::make_tuple(S0, acc + " ", '\0');
        }

        if (is_printable(c)) {
          return std::make_tuple(S0, acc + c, '\0');
        }

        if (c != '%') {
          throw std::runtime_error("Invalid char");
        }

        return std::make_tuple(S1, acc, '\0');
      }

      if (current_state == S1) {
        if (!has_hexa_base(c)) {
          throw std::runtime_error("Invalid state on S1");
        }

        return std::make_tuple(S2, acc, c);
      }

      if (current_state == S2) {
        if (!has_hexa_base(c)) {
          throw std::runtime_error("Invalid state on S2");
        }

        const auto encoded_value = {comp1, c, '\0'};

        const auto encoded_char = std::stoi(encoded_value, nullptr, 16);

        return std::make_tuple(S0, acc + static_cast<char>(encoded_char), '\0');
      }

      throw std::runtime_error("Invalid state ever");
    }));
}
