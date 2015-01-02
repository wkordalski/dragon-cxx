#include "../src/token.hpp"

#include <gtest/gtest.h>

TEST(ManagerTest, Sample)
{
  auto cast = [](std::shared_ptr<dragon::Token> p)
  {
    return dynamic_cast<dragon::Identifier*>(p.get());
  };
  auto a = std::shared_ptr<dragon::Token>(new dragon::Identifier());
  auto b = std::shared_ptr<dragon::Token>(new dragon::Identifier());
  auto c = std::shared_ptr<dragon::Token>(new dragon::Identifier());
  cast(a)->text = L"a";
  cast(b)->text = L"b";
  cast(c)->text = L"c";

  dragon::Handle A(a);
  dragon::Handle B(b);
  dragon::Handle D(new dragon::Identifier());

  ASSERT_EQ(cast(A.get())->text, L"a");
  ASSERT_EQ(cast(B.get())->text, L"b");

  B = A;

  ASSERT_EQ(cast(A.get())->text, L"a");
  ASSERT_EQ(cast(B.get())->text, L"a");

  B.set(c);

  ASSERT_EQ(cast(A.get())->text, L"c");
  ASSERT_EQ(cast(B.get())->text, L"c");

  cast(D.get())->text = L"d";
  ASSERT_EQ(cast(D.get())->text, L"d");

  auto *p = new dragon::Identifier();
  p->text = L"p";
  c->replace(p);

  ASSERT_EQ(cast(A.get())->text, L"p");
  ASSERT_EQ(cast(B.get())->text, L"p");
}
