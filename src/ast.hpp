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

  class VariableDecl : public Token
  {
  public:
    std::vector<Handle> attribs;
    std::vector<Handle> decls;
    Handle docstring;

    VariableDecl() {}
    VariableDecl(std::vector<Handle> attribs, std::vector<Handle> decls, Handle doc = Handle())
      : attribs(attribs), decls(decls), docstring(doc) {}

    virtual void levelup() {}
  };

  class VariableSingleDecl : public Token
  {
  public:
    Handle id;
    Handle type;
    Handle value;

    VariableSingleDecl() {}
    VariableSingleDecl(Handle id, Handle type, Handle value) : id(id), type(type), value(value) {}

    virtual void levelup() {}
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

  class AssignOperator : public Token
  {
  public:
    Handle left;
    Handle right;

    AssignOperator() {}
    AssignOperator(Handle l, Handle r) : left(l), right(r) {}

    virtual void levelup() {}
  };

  class BinaryUserOperator : public Token
  {
  public:
    Handle left;
    Handle right;
    std::string op;

    BinaryUserOperator() {}
    BinaryUserOperator(std::string o, Handle l, Handle r) : left(l), right(r), op(o) {}

    virtual void levelup() {}
  };

  class BinaryUserAssignOperator : public Token
  {
  public:
    Handle left;
    Handle right;
    std::string op;

    BinaryUserAssignOperator() {}
    BinaryUserAssignOperator(std::string o, Handle l, Handle r) : left(l), right(r), op(o) {}

    virtual void levelup() {}
  };

  class RangeOperator : public Token
  {
  public:
    Handle left;
    Handle right;

    RangeOperator() {}
    RangeOperator(Handle l, Handle r) : left(l), right(r) {}

    virtual void levelup() {}
  };

  class CompareOperator : public Token
  {
  public:
    Handle left;
    Handle right;
    std::string rel;

    CompareOperator() {}
    CompareOperator(std::string o, Handle l, Handle r) : left(l), right(r), rel(o) {}

    virtual void levelup() {}
  };

  class UnaryPrefixUserOperator : public Token
  {
  public:
    Handle expr;
    std::string op;

    UnaryPrefixUserOperator() {}
    UnaryPrefixUserOperator(std::string o, Handle e) : expr(e), op(o) {}

    virtual void levelup() {}
  };

  class UnaryPostfixUserOperator : public Token
  {
  public:
    Handle expr;
    std::string op;

    UnaryPostfixUserOperator() {}
    UnaryPostfixUserOperator(std::string o, Handle e) : expr(e), op(o) {}

    virtual void levelup() {}
  };

  class PointerTypeOperator : public Token
  {
  public:
    Handle type;

    PointerTypeOperator(Handle t) : type(t) {}

    virtual void levelup() {}
  };

  class StorePointerOperator : public Token
  {
  public:
    Handle left;
    Handle right;

    StorePointerOperator() {}
    StorePointerOperator(Handle l, Handle r) : left(l), right(r) {}

    virtual void levelup() {}
  };


  class RelationOperator : public Token
  {
  public:
    Handle left;
    Handle right;
    std::string rel;

    RelationOperator() {}
    RelationOperator(std::string o, Handle l, Handle r) : left(l), right(r), rel(o) {}

    virtual void levelup() {}
  };

  class LambdaOperator : public Token
  {
    // TODO
  };

  class CommaOperator : public Token
  {
  public:
    Handle left;
    Handle right;

    CommaOperator() {}
    CommaOperator(Handle l, Handle r) : left(l), right(r) {}

    virtual void levelup() {}
  };

  class NoneOperator : public Token
  {
  public:
    NoneOperator() {}

    virtual void levelup() {}
  };
}
