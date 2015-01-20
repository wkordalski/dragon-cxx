#include "parser.hpp"     // cause it is in autoinclude dir
#include "../src/scanner.hpp"

#include <gtest/gtest.h>

TEST(ParserTest, Sample)
{
  std::wstring code =
  // L"import hallo.world\n"
  // L"namespace a\n"
  // L"  var b\n"
  // L"  namespace c.x.y. z\n"
  // L"    namespace d.h\n"
  // L"      var e =\n"
  // L"  var f :\n"
  // L"var g :=\n";
  L"var a : int = floor(3 + 5 * 8) + [1,2,3,4][2]\n"
  L"var b = try a.func(5, 8) except Error then if 1 < 2 then -1 else 1 except Bug then 0\n";
  std::wistringstream ss(code);
  dragon::Scanner sc(ss);
  dragon::Parser sp(sc);
  ASSERT_EQ(sp.parse(), 0);
}
