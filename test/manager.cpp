#include "../src/ast/source.hpp"

#include <gtest/gtest.h>

TEST(ManagerTest, Sample)
{
  /*
  auto cast = [](dragon::Handle p)
  {
    return dynamic_cast<dragon::Identifier*>(p.get());
  };
  auto a = new dragon::Identifier(L"a");
  auto b = new dragon::Identifier(L"b");
  auto c = new dragon::Identifier(L"c");

  dragon::Handle A(a);
  dragon::Handle B(b);
  dragon::Handle D(new dragon::Identifier(L"d"));

  ASSERT_EQ(cast(A)->text, L"a");
  ASSERT_EQ(cast(B)->text, L"b");

  B = A;

  ASSERT_EQ(cast(A)->text, L"a");
  ASSERT_EQ(cast(B)->text, L"a");

  B.set(c);

  ASSERT_EQ(cast(A)->text, L"c");
  ASSERT_EQ(cast(B)->text, L"c");

  cast(D)->text = L"d";
  ASSERT_EQ(cast(D)->text, L"d");

  auto *p = new dragon::Identifier(L"p");
  c->replace(p);

  ASSERT_EQ(cast(A)->text, L"p");
  ASSERT_EQ(cast(B)->text, L"p");
  */
}
