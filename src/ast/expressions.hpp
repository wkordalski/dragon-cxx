#pragma once

#include "../node.hpp"
#include "symbols.hpp"
#include "declarations/alias.hpp"

namespace dragon
{
  class IExpression
  {
  public:
    virtual void desymbolize_expression(Handle symbol_table) { assert(false or "Should be abstract!"); }
  };

  class IdentifierExpression : public Node, public IExpression
  {
  public:
    Handle id;

    IdentifierExpression(Handle id) : id(id) {}

    virtual void desymbolize_expression(Handle symbol_table)
    {
      auto desym = symbol_table.as<ISymbolTable>()->lookup(id);
      if(!desym) assert("Error - no suchly named thing!");
      while(desym.is<Alias>())
      {
        desym = desym.as<Alias>()->get_target();
      }
      replace(desym.get());
    }

    virtual void print(std::wostream &os) const { os << L"IdentifierExpression ["<<handle()<<"] ( identifier = "<< int(id) <<" )" << std::endl;}
  };

  class ArrayLiteral : public Node, public IExpression
  {
  public:
    std::vector<Handle> exprs;

    ArrayLiteral() {}
    ArrayLiteral(std::vector<Handle> elements) : exprs(elements) {}

    virtual void print(std::wostream &os) const { os << L"[ArrayLiteral]"; }
  };

  class MemberOperator : public Node, public IExpression
  {
  public:
    Handle left;
    Handle right;

    MemberOperator(Handle l, Handle r) : left(l), right(r) {}
  };

  class CallOperator : public Node, public IExpression
  {
  public:
    Handle left;
    std::vector<Handle> right;

    CallOperator(Handle l, std::vector<Handle> r) : left(l), right(r) {}
  };

  class IndexOperator : public Node, public IExpression
  {
  public:
    Handle left;
    std::vector<Handle> right;

    IndexOperator(Handle l, std::vector<Handle> r) : left(l), right(r) {}
  };

  class IfElseExpression : public Node, public IExpression
  {
  public:
    std::vector<std::pair<Handle,Handle>> if_exprs;
    Handle else_expr;

    IfElseExpression() {}
    IfElseExpression(Handle else_expr) : else_expr(else_expr) {}
  };

  class TryExceptExpression : public Node, public IExpression
  {
  public:
    Handle expr = Handle();
    std::vector<std::pair<Handle, Handle>> catches;

    TryExceptExpression() {}
    TryExceptExpression(std::pair<Handle,Handle> acatch) : catches({acatch}) {}
  };

  class AssignOperator : public Node, public IExpression
  {
  public:
    Handle left;
    Handle right;

    AssignOperator() {}
    AssignOperator(Handle l, Handle r) : left(l), right(r) {}
  };

  class BinaryUserOperator : public Node, public IExpression
  {
  public:
    Handle left;
    Handle right;
    std::string op;

    BinaryUserOperator() {}
    BinaryUserOperator(std::string o, Handle l, Handle r) : left(l), right(r), op(o) {}
  };

  class BinaryUserAssignOperator : public Node, public IExpression
  {
  public:
    Handle left;
    Handle right;
    std::string op;

    BinaryUserAssignOperator() {}
    BinaryUserAssignOperator(std::string o, Handle l, Handle r) : left(l), right(r), op(o) {}
  };

  class RangeOperator : public Node, public IExpression
  {
  public:
    Handle left;
    Handle right;

    RangeOperator() {}
    RangeOperator(Handle l, Handle r) : left(l), right(r) {}
  };

  class CompareOperator : public Node, public IExpression
  {
  public:
    Handle left;
    Handle right;
    std::string rel;

    CompareOperator() {}
    CompareOperator(std::string o, Handle l, Handle r) : left(l), right(r), rel(o) {}
  };

  class UnaryPrefixUserOperator : public Node, public IExpression
  {
  public:
    Handle expr;
    std::string op;

    UnaryPrefixUserOperator() {}
    UnaryPrefixUserOperator(std::string o, Handle e) : expr(e), op(o) {}
  };

  class UnaryPostfixUserOperator : public Node, public IExpression
  {
  public:
    Handle expr;
    std::string op;

    UnaryPostfixUserOperator() {}
    UnaryPostfixUserOperator(std::string o, Handle e) : expr(e), op(o) {}
  };

  class PointerTypeOperator : public Node, public IExpression
  {
  public:
    Handle type;

    PointerTypeOperator(Handle t) : type(t) {}
  };

  class StorePointerOperator : public Node, public IExpression
  {
  public:
    Handle left;
    Handle right;

    StorePointerOperator() {}
    StorePointerOperator(Handle l, Handle r) : left(l), right(r) {}
  };


  class RelationOperator : public Node, public IExpression
  {
  public:
    Handle left;
    Handle right;
    std::string rel;

    RelationOperator() {}
    RelationOperator(std::string o, Handle l, Handle r) : left(l), right(r), rel(o) {}
  };

  class LambdaOperator : public Node, public IExpression
  {
    // TODO
  };

  class CommaOperator : public Node, public IExpression
  {
  public:
    Handle left;
    Handle right;

    CommaOperator() {}
    CommaOperator(Handle l, Handle r) : left(l), right(r) {}
  };

  class NoneOperator : public Node, public IExpression
  {
  public:
    NoneOperator() {}
  };

  class PostfixLiteralOperator : public Node, public IExpression
  {
  public:
    Handle literal;
    Handle op;

    PostfixLiteralOperator() {}
    PostfixLiteralOperator(Handle literal, Handle op) : literal(literal), op(op) {}

    virtual void print(std::wostream &os) const { os << "PostfixLiteralOperator ["<<handle()<<"] ( literal = "<<int(literal)<<", operator = "<<int(op)<<")"<<std::endl;}
  };
}
