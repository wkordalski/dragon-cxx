#include "../src/prefix_tree.hpp"

#include <gtest/gtest.h>

#include <string>

TEST(PrefixTreeTest, Insert)
{
  dragon::PrefixTree<char> pt;
  std::string a = "elephant";
  std::string b = "elevator";

  pt.insert(std::begin(a), std::end(a));

  ASSERT_TRUE(pt.exists(std::begin(a), std::end(a)))
    << "Inserted element should exist in collection";
  ASSERT_FALSE(pt.exists(std::begin(b), std::end(b)))
    << "Not inserted element shouldn't exist in collection";

  pt.insert(std::begin(a), std::end(a));

  ASSERT_TRUE(pt.exists(std::begin(a), std::end(a)))
    << "Inserted element should exist in collection";

  pt.insert(std::begin(a), std::end(a));
  pt.insert(std::begin(b), std::end(b));

  ASSERT_TRUE(pt.exists(std::begin(a), std::end(a)))
    << "Inserted element should exist in collection";
  ASSERT_TRUE(pt.exists(std::begin(b), std::end(b)))
    << "Inserted element should exist in collection";
}

TEST(PrefixTreeTest, Remove)
{
  dragon::PrefixTree<char> pt;
  std::string a = "elephant";
  std::string b = "elevator";
  pt.insert(std::begin(a), std::end(a));
  pt.remove(std::begin(b), std::end(b));

  ASSERT_TRUE(pt.exists(std::begin(a), std::end(a)))
    << "Inserted element should exist in collection";
  ASSERT_FALSE(pt.exists(std::begin(b), std::end(b)))
    << "Not inserted element shouldn't exist in collection";

  pt.remove(std::begin(a), std::end(a));

  ASSERT_FALSE(pt.exists(std::begin(a), std::end(a)))
    << "Removed element shouldn't exist in collection";
  ASSERT_FALSE(pt.exists(std::begin(b), std::end(b)))
    << "Not inserted element shouldn't exist in collection";

  pt.insert(std::begin(a), std::end(a));
  pt.insert(std::begin(a), std::end(a));

  ASSERT_TRUE(pt.exists(std::begin(a), std::end(a)))
    << "Inserted element should exist in collection";
  ASSERT_FALSE(pt.exists(std::begin(b), std::end(b)))
    << "Not inserted element shouldn't exist in collection";

  pt.remove(std::begin(a), std::end(a));

  ASSERT_FALSE(pt.exists(std::begin(a), std::end(a)))
    << "Removed element shouldn't exist in collection";
  ASSERT_FALSE(pt.exists(std::begin(b), std::end(b)))
    << "Not inserted element shouldn't exist in collection";

  pt.insert(std::begin(a), std::end(a));
  pt.insert(std::begin(b), std::end(b));

  ASSERT_TRUE(pt.exists(std::begin(a), std::end(a)))
    << "Inserted element should exist in collection";
  ASSERT_TRUE(pt.exists(std::begin(b), std::end(b)))
    << "Inserted element should exist in collection";

  pt.remove(std::begin(a), std::end(a));

  ASSERT_FALSE(pt.exists(std::begin(a), std::end(a)))
    << "Removed element shouldn't exist in collection";
  ASSERT_TRUE(pt.exists(std::begin(b), std::end(b)))
    << "Inserted element should exist in collection";

}

TEST(PrefixTreeTest, Prefix)
{
  dragon::PrefixTree<char> pt;
  std::string a = "elephant";
  std::string b = "elevator";

  std::string c = "";
  std::string d = "e";
  std::string e = "elephant";
  std::string f = "elementary";
  std::string g = "elephantor";

  ASSERT_FALSE(pt.prefix(std::begin(c), std::end(c)))
    << "Collection is empty so nothing is prefix of nothing";

  pt.insert(std::begin(a), std::end(a));

  ASSERT_TRUE(pt.prefix(std::begin(c), std::end(c)))
    << "Collection has one element so empty string is a prefix of the element";
  ASSERT_TRUE(pt.prefix(std::begin(d), std::end(d)))
    << "First letter is a prefix of the whole word";
  ASSERT_TRUE(pt.prefix(std::begin(e), std::end(e)))
    << "The word is its prefix";
  ASSERT_FALSE(pt.prefix(std::begin(f), std::end(f)))
    << "The word is not any prefix of 'elephant'";
  ASSERT_FALSE(pt.prefix(std::begin(g), std::end(g)))
    << "Longer word is not a prefix of shorter one";

  pt.insert(std::begin(b), std::end(b));

  ASSERT_TRUE(pt.prefix(std::begin(c), std::end(c)))
    << "Collection has one element so empty string is a prefix of the element";
  ASSERT_TRUE(pt.prefix(std::begin(d), std::end(d)))
    << "First letter is a prefix of the whole word";
  ASSERT_TRUE(pt.prefix(std::begin(e), std::end(e)))
    << "The word is its prefix";
  ASSERT_FALSE(pt.prefix(std::begin(f), std::end(f)))
    << "The word is not any prefix of 'elephant'";
  ASSERT_FALSE(pt.prefix(std::begin(g), std::end(g)))
    << "Longer word is not a prefix of shorter one";
    
}
