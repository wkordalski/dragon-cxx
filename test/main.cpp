#include <gtest/gtest.h>
#include "../src/node.hpp"

using namespace std;

int main(int argc, char* argv[])
{
  testing::InitGoogleTest(&argc, argv);
  int r = RUN_ALL_TESTS();
  dragon::Handle::cleanup();
  return r;
}
