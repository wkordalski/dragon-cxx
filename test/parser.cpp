#include "../src/parser.hpp"
#include "../src/lexer.hpp"

#include <gtest/gtest.h>

TEST(ParserTest, Sample)
{
  std::wstring code =
  L"import hallo.world\n"
  L"namespace a\n"
  L"  var b\n"
  L"  namespace c.x.y. z\n"
  L"    namespace d.h\n"
  L"      var e =\n"
  L"  var f :\n"
  L"var g :=\n";
  std::wistringstream ss(code);
  auto toks = dragon::tokenize(ss);
  // std::vector<dragon::Handle>

  dragon::Parser ps;
  auto prog = ps.parse(toks);
  ASSERT_TRUE(prog.valid());
  prog->print(std::wcout);
  std::wcout << std::endl;
}
