#pragma once

#include "token.hpp"

namespace dragon
{
  class Program : public Token
  {
  public:
    Handle docstring = Handle();
    std::vector<Handle> declarations;

    Program() {}
    Program(std::vector<Handle> decls, Handle doc = Handle()) : docstring(doc), declarations(decls) {}

    virtual void levelup() { /* TODO */ }
    virtual void print(std::wostream &os)
  { os << L"PROGRAM"<<std::endl<<"==========="<<std::endl; for(auto h : declarations)if(h) h->print(os); }
  };

  class NamespaceDecl : public Token
  {
  public:
    std::vector<Handle> name;
    std::vector<Handle> declarations;

    NamespaceDecl() {}
    NamespaceDecl(std::vector<Handle> name, std::vector<Handle> decls = {}) : name(name), declarations(decls) {}

    virtual void levelup() { /* TODO */ }
    virtual void print(std::wostream &os)
  { os << L"[namespace "; for(auto h : declarations) h->print(os); os<<"]" << std::endl; for(auto h : declarations)if(h) h->print(os); }
  };

  class ArrayLiteral : public Token
  {
  public:
    std::vector<Handle> exprs;

    ArrayLiteral() {}
    ArrayLiteral(std::vector<Handle> elements) : exprs(elements) {}

    virtual void print(std::wostream &os) { os << L"[ArrayLiteral]"; }

    virtual void levelup() { /* TODO */ }
  };

  class MemberOperator : public Token
  {
  public:
    Handle left;
    Handle right;

    MemberOperator(Handle l, Handle r) : left(l), right(r) {}

    virtual void levelup() {}
  };

  class CallOperator : public Token
  {
  public:
    Handle left;
    std::vector<Handle> right;

    CallOperator(Handle l, std::vector<Handle> r) : left(l), right(r) {}

    virtual void levelup() {}
  };

  class IndexOperator : public Token
  {
  public:
    Handle left;
    std::vector<Handle> right;

    IndexOperator(Handle l, std::vector<Handle> r) : left(l), right(r) {}

    virtual void levelup() {}
  };

  class IfElseExpression : public Token
  {
  public:
    std::vector<std::pair<Handle,Handle>> if_exprs;
    Handle else_expr;

    IfElseExpression() {}
    IfElseExpression(Handle else_expr) : else_expr(else_expr) {}

    virtual void levelup() {}
  };

  class TryExceptExpression : public Token
  {
  public:
    Handle expr = Handle();
    std::vector<std::pair<Handle, Handle>> catches;

    TryExceptExpression() {}
    TryExceptExpression(std::pair<Handle,Handle> acatch) : catches({acatch}) {}

    virtual void levelup() {}
  };
}
