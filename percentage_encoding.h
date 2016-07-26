#pragma once

#include <string>
#include <numeric>
#include <functional>
#include <array>

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

constexpr auto is_base16(char c) -> bool
{
  return (c >= 'a' && c <= 'f')
          || (c >= 'A' && c <= 'F')
          || (c >= '0' && c <= '9');
}

struct decoder_context
{
  decoder_state state;
  std::string acc;
  char hex_comp;
};

const auto decoder_state_handlers = std::array<std::function<decoder_context(const decoder_context&, char)>, 3> {
  // S0
  [](const decoder_context& context, char c) -> decoder_context
  {
    if (c == '+')
      return {decoder_state::S0, context.acc + " ", '\0'};

    if (is_printable(c))
      return {decoder_state::S0, context.acc + c, '\0'};

    if (c == '%')
      return {decoder_state::S1, context.acc, '\0'};

    throw std::runtime_error(std::string{"Invalid input"});
  },

  // S1
  [](const decoder_context& context, char c) -> decoder_context
  {
    if (!is_base16(c))
      throw std::runtime_error("Invalid state on S1");

    return {decoder_state::S2, context.acc, c};
  },

  // S2
  [](const decoder_context& context, char c) -> decoder_context
  {
    if (!is_base16(c))
      throw std::runtime_error("Invalid state on S2");

    const auto encoded_value = {context.hex_comp, c, '\0'};

    const auto encoded_char = std::stoi(encoded_value, nullptr, 16);

    return {decoder_state::S0, context.acc + static_cast<char>(encoded_char), '\0'};
  }
};

auto percentage_decode(const std::string &input) -> std::string
{
  return std::accumulate(
    begin(input),
    end(input),
    decoder_context{decoder_state::S0, std::string{}, char{0}},
    [&](const decoder_context &context, char c) -> decoder_context {
      return decoder_state_handlers[static_cast<std::size_t>(context.state)](context, c);
    }).acc;
}
