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

  });
});

auto main(int argc, char **argv) -> int
{
  return run(argc, argv);
}
