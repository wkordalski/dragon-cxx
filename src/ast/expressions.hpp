#pragma once

#include "../containers.hpp"
#include "../node.hpp"

namespace dragon
{
  struct IdentifierExpression : public Node
  {
    Member id;

    IdentifierExpression(Local id) : id(id) {}
};

  struct ArrayLiteral : public Node
  {
    MVector exprs;

    ArrayLiteral() {}
    ArrayLiteral(LVector elements) : exprs(to_member(elements)) {}
  };

  class MemberOperator : public Node
  {
  public:
    Member left;
    Member right;

    MemberOperator(Local l, Local r) : left(l), right(r) {}
  };

  class CallOperator : public Node
  {
  public:
    Member left;
    MVector right;

    CallOperator(Local l, LVector r) : left(l), right(to_member(r)) {}
  };

  class IndexOperator : public Node
  {
  public:
    Member left;
    MVector right;

    IndexOperator(Local l, LVector r) : left(l), right(to_member(r)) {}
  };

  class IfElseExpression : public Node
  {
  public:
    MMVector if_exprs;
    Member else_expr;

    IfElseExpression() {}
    IfElseExpression(Handle else_expr) : else_expr(else_expr) {}
  };

  class TryExceptExpression : public Node
  {
  public:
    Member expr = Handle();
    MMVector catches;

    TryExceptExpression() {}
    TryExceptExpression(LLPair acatch) : catches({acatch}) {}
  };

  class AssignOperator : public Node
  {
  public:
    Member left;
    Member right;

    AssignOperator() {}
    AssignOperator(Local l, Local r) : left(l), right(r) {}
  };

  class BinaryUserOperator : public Node
  {
  public:
    Member left;
    Member right;
    std::string op;

    BinaryUserOperator() {}
    BinaryUserOperator(std::string o, Local l, Local r) : left(l), right(r), op(o) {}
  };

  class BinaryUserAssignOperator : public Node
  {
  public:
    Member left;
    Member right;
    std::string op;

    BinaryUserAssignOperator() {}
    BinaryUserAssignOperator(std::string o, Local l, Local r) : left(l), right(r), op(o) {}
  };

  class RangeOperator : public Node
  {
  public:
    Member left;
    Member right;

    RangeOperator() {}
    RangeOperator(Local l, Local r) : left(l), right(r) {}
  };

  class CompareOperator : public Node
  {
  public:
    Member left;
    Member right;
    std::string rel;

    CompareOperator() {}
    CompareOperator(std::string o, Local l, Local r) : left(l), right(r), rel(o) {}
  };

  class UnaryPrefixUserOperator : public Node
  {
  public:
    Member expr;
    std::string op;

    UnaryPrefixUserOperator() {}
    UnaryPrefixUserOperator(std::string o, Local e) : expr(e), op(o) {}
  };

  class UnaryPostfixUserOperator : public Node
  {
  public:
    Member expr;
    std::string op;

    UnaryPostfixUserOperator() {}
    UnaryPostfixUserOperator(std::string o, Local e) : expr(e), op(o) {}
  };

  class PointerTypeOperator : public Node
  {
  public:
    Member type;

    PointerTypeOperator(Local t) : type(t) {}
  };

  class StorePointerOperator : public Node
  {
  public:
    Member left;
    Member right;

    StorePointerOperator() {}
    StorePointerOperator(Local l, Local r) : left(l), right(r) {}
  };


  class RelationOperator : public Node
  {
  public:
    Member left;
    Member right;
    std::string rel;

    RelationOperator() {}
    RelationOperator(std::string o, Local l, Local r) : left(l), right(r), rel(o) {}
  };

  class LambdaOperator : public Node
  {
    // TODO
  };

  class CommaOperator : public Node
  {
  public:
    Member left;
    Member right;

    CommaOperator() {}
    CommaOperator(Local l, Local r) : left(l), right(r) {}
  };

  class NoneOperator : public Node
  {
  public:
    NoneOperator() {}
  };

  class PostfixLiteralOperator : public Node
  {
  public:
    Member literal;
    Member op;

    PostfixLiteralOperator() {}
    PostfixLiteralOperator(Local literal, Local op) : literal(literal), op(op) {}
  };
}
