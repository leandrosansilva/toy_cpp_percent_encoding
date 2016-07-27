#include <string>
#include <numeric>

#include <bandit/bandit.h>

#include "percentage_encoding.h"

using namespace bandit;

go_bandit([]{
  describe("Decoding", []{
    it("decodes an empty string to an empty string", [&] {
      AssertThat(percentage_decode(""), Equals(""));
    });

    it("decodes an alphanumeric string to itself", [&] {
      AssertThat(percentage_decode("cacatua123"), Equals("cacatua123"));
    });

    it("decodes 'lala+123' to 'lala 123'", [&] {
      AssertThat(percentage_decode("lala+123"), Equals("lala 123"));
    });

    it("decodes 'mamamia%21' to 'mamamia!'", [&] {
      AssertThat(percentage_decode("mamamia%21"), Equals("mamamia!"));
    });

    it("decodes 'Weird+char\%3A+\%E2\%8C\%9B' to 'Weird char: ⌛'", [&] {
      AssertThat(percentage_decode("Weird+char\%3A+\%E2\%8C\%9B"), Equals("Weird char: ⌛"));
    });

    it("decodes 'A\%C3\%84BCDEFGHIJKLMNO\%C3\%96PQRS\%C3\%9FTU\%C3\%9CVWXYZ' to 'AÄBCDEFGHIJKLMNOÖPQRSßTUÜVWXYZ'", [&] {
      AssertThat(percentage_decode("A\%C3\%84BCDEFGHIJKLMNO\%C3\%96PQRS\%C3\%9FTU\%C3\%9CVWXYZ"), Equals("AÄBCDEFGHIJKLMNOÖPQRSßTUÜVWXYZ"));
    });

    it("decodes 'Is+this+Chinese+char\%3A+\%F0\%9F\%88\%9A\%EF\%B8\%8E+Chinese%3F+I+don%27t+know%E2%80%A6' to 'Is this Chinese char: 🈚︎ Chinese? I don't know…'", [&] {
      AssertThat(percentage_decode("Is+this+Chinese+char\%3A+\%F0\%9F\%88\%9A\%EF\%B8\%8E+Chinese%3F+I+don%27t+know%E2%80%A6"), Equals("Is this Chinese char: 🈚︎ Chinese? I don't know…"));
    });

    it("decodes '2+%2B+2+%3D+5' to '2 + 2 = 5'", [&] {
      AssertThat(percentage_decode("2+%2B+2+%3D+5"), Equals("2 + 2 = 5"));
    });

    it("fails to decode a%GG", [&] {
      AssertThrows(std::runtime_error, percentage_decode("a%GG"));
    });

    it("fails to decode a%BG", [&] {
      AssertThrows(std::runtime_error, percentage_decode("a%BG"));
    });

    it("fails to decode ABC%", [&] {
      AssertThrows(std::runtime_error, percentage_decode("ABC%"));
    });

    it("decodes 'C.O.' to 'C.O.", [&] {
      AssertThat(percentage_decode("C.O."), Equals("C.O."));
    });

    it ("decodes some more complex vietnemese text", [&] {
      AssertThat(percentage_decode("Qu%E1%BA%A7n+%C4%91%E1%BA%A3o+Tr%C6%B0%E1%BB%9Dng+Sa+%28ti%E1%BA%BFng+Anh%3A+Spratly+Islands%29+l%C3%A0+m%E1%BB%99t+t%E1%BA%ADp+h%E1%BB%A3p+th%E1%BB%B1c+th%E1%BB%83+%C4%91%E1%BB%8Ba+l%C3%AD+%C4%91%C6%B0%E1%BB%A3c+bao+quanh+b%E1%BB%9Fi+nh%E1%BB%AFng+v%C3%B9ng+%C4%91%C3%A1nh+c%C3%A1+tr%C3%B9+ph%C3%BA+v%C3%A0+c%C3%B3+ti%E1%BB%81m+n%C4%83ng+d%E1%BA%A7u+m%E1%BB%8F+v%C3%A0+kh%C3%AD+%C4%91%E1%BB%91t+thu%E1%BB%99c+bi%E1%BB%83n+%C4%90%C3%B4ng."), Equals("Quần đảo Trường Sa (tiếng Anh: Spratly Islands) là một tập hợp thực thể địa lí được bao quanh bởi những vùng đánh cá trù phú và có tiềm năng dầu mỏ và khí đốt thuộc biển Đông."));
    });
  });
});

auto main(int argc, char **argv) -> int
{
  return run(argc, argv);
}
