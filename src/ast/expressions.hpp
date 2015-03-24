#pragma once

#include "../node.hpp"

namespace dragon
{
  struct IdentifierExpression : public Node
  {
    Handle id;

    IdentifierExpression(Handle id) : id(id) {}
};

  struct ArrayLiteral : public Node
  {
    std::vector<Handle> exprs;

    ArrayLiteral() {}
    ArrayLiteral(std::vector<Handle> elements) : exprs(elements) {}
  };

  class MemberOperator : public Node
  {
  public:
    Handle left;
    Handle right;

    MemberOperator(Handle l, Handle r) : left(l), right(r) {}
  };

  class CallOperator : public Node
  {
  public:
    Handle left;
    std::vector<Handle> right;

    CallOperator(Handle l, std::vector<Handle> r) : left(l), right(r) {}
  };

  class IndexOperator : public Node
  {
  public:
    Handle left;
    std::vector<Handle> right;

    IndexOperator(Handle l, std::vector<Handle> r) : left(l), right(r) {}
  };

  class IfElseExpression : public Node
  {
  public:
    std::vector<std::pair<Handle,Handle>> if_exprs;
    Handle else_expr;

    IfElseExpression() {}
    IfElseExpression(Handle else_expr) : else_expr(else_expr) {}
  };

  class TryExceptExpression : public Node
  {
  public:
    Handle expr = Handle();
    std::vector<std::pair<Handle, Handle>> catches;

    TryExceptExpression() {}
    TryExceptExpression(std::pair<Handle,Handle> acatch) : catches({acatch}) {}
  };

  class AssignOperator : public Node
  {
  public:
    Handle left;
    Handle right;

    AssignOperator() {}
    AssignOperator(Handle l, Handle r) : left(l), right(r) {}
  };

  class BinaryUserOperator : public Node
  {
  public:
    Handle left;
    Handle right;
    std::string op;

    BinaryUserOperator() {}
    BinaryUserOperator(std::string o, Handle l, Handle r) : left(l), right(r), op(o) {}
  };

  class BinaryUserAssignOperator : public Node
  {
  public:
    Handle left;
    Handle right;
    std::string op;

    BinaryUserAssignOperator() {}
    BinaryUserAssignOperator(std::string o, Handle l, Handle r) : left(l), right(r), op(o) {}
  };

  class RangeOperator : public Node
  {
  public:
    Handle left;
    Handle right;

    RangeOperator() {}
    RangeOperator(Handle l, Handle r) : left(l), right(r) {}
  };

  class CompareOperator : public Node
  {
  public:
    Handle left;
    Handle right;
    std::string rel;

    CompareOperator() {}
    CompareOperator(std::string o, Handle l, Handle r) : left(l), right(r), rel(o) {}
  };

  class UnaryPrefixUserOperator : public Node
  {
  public:
    Handle expr;
    std::string op;

    UnaryPrefixUserOperator() {}
    UnaryPrefixUserOperator(std::string o, Handle e) : expr(e), op(o) {}
  };

  class UnaryPostfixUserOperator : public Node
  {
  public:
    Handle expr;
    std::string op;

    UnaryPostfixUserOperator() {}
    UnaryPostfixUserOperator(std::string o, Handle e) : expr(e), op(o) {}
  };

  class PointerTypeOperator : public Node
  {
  public:
    Handle type;

    PointerTypeOperator(Handle t) : type(t) {}
  };

  class StorePointerOperator : public Node
  {
  public:
    Handle left;
    Handle right;

    StorePointerOperator() {}
    StorePointerOperator(Handle l, Handle r) : left(l), right(r) {}
  };


  class RelationOperator : public Node
  {
  public:
    Handle left;
    Handle right;
    std::string rel;

    RelationOperator() {}
    RelationOperator(std::string o, Handle l, Handle r) : left(l), right(r), rel(o) {}
  };

  class LambdaOperator : public Node
  {
    // TODO
  };

  class CommaOperator : public Node
  {
  public:
    Handle left;
    Handle right;

    CommaOperator() {}
    CommaOperator(Handle l, Handle r) : left(l), right(r) {}
  };

  class NoneOperator : public Node
  {
  public:
    NoneOperator() {}
  };

  class PostfixLiteralOperator : public Node
  {
  public:
    Handle literal;
    Handle op;

    PostfixLiteralOperator() {}
    PostfixLiteralOperator(Handle literal, Handle op) : literal(literal), op(op) {}
  };
}
