#include "parser.hpp"     // cause it is in autoinclude dir
#include "../src/scanner.hpp"

#include <gtest/gtest.h>

TEST(ParserTest, Sample)
{
  std::wstring code =
  L"'''\n"
  L"Sample module for us to test parser.\n"
  L"We need to fix lexer to proparely support location of multiline literals.\n"
  L"'''\n"
  L"var a : int * = floor(5 * 5) + [1,2,3,4][2]\n"//(3 + 5 * 8)\n"// \n"
  L"var b = try a.func(5, 8) except Error then if 1 < 2 then -1 else 1 except Bug then 0\n";
  std::wistringstream ss(code);
  dragon::Scanner sc(ss);
  dragon::Parser sp(sc);
  //sp.set_debug_level(1);
  ASSERT_EQ(sp.parse(), 0);
}
