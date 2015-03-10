#pragma once

#include <cassert>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <typeindex>
#include <vector>

#include "../node.hpp"

#include "../utils/hash.hpp"


namespace dragon
{
  class Identifier : public Node
  {
  public:
    std::wstring text;
    Place place;

    Identifier() = default;
    Identifier(std::wstring s) : text(s) {}

  public:
    virtual void accept(Visitor &v) { v.visit(*this); }
    // virtual bool equal(const Node *t) const { if(auto tt = dynamic_cast<const Identifier*>(t)) return (text == tt->text); else return false; }
    // virtual std::size_t hash() const
    // {
    //   return hash_args< std::hash<std::type_index>, std::hash<std::wstring> >( std::type_index(typeid(*this)), text );
    // }
  };

  class Operator : public Node
  {
  public:
    std::wstring text;
    Place place;

  public:
    virtual void accept(Visitor &v) { v.visit(*this); }
  };

  class Literal : public Node
  {
  public:
    std::wstring text;
    Place place;

  public:
    virtual void accept(Visitor &v) { v.visit(*this); }

    void parse_literal();
  };

  class Newline : public Node
  {
  public:
    Place place;
    virtual void accept(Visitor &v) { v.visit(*this); }
  };

  class Indent : public Node
  {
  public:
    Place place;
    virtual void accept(Visitor &v) { v.visit(*this); }
  };

  class Dedent : public Node
  {
  public:
    Place place;
    virtual void accept(Visitor &v) { v.visit(*this); }
  };
}
