#pragma once

#include "../token.hpp"

namespace dragon
{
  class File : public Token
  {
  public:
    Handle docstring = Handle();
    std::vector<Handle> declarations;

    File() {}
    File(std::vector<Handle> decls, Handle doc = Handle()) : docstring(doc), declarations(decls) {}

    virtual void levelup() { /* TODO */ }
    virtual void print(std::wostream &os) const
    {
      os << "File [" << handle() << "] ( doc = " << int(docstring) << ", decls = [ ";
      for(auto h : declarations) os << int(h) << " ";
      os << "] )" << std::endl;
    }

    void fillin(Handle h);
  };

  class ImportDecls : public Token
  {
  public:
    std::vector<Handle> imports;

    ImportDecls() {}
    ImportDecls(std::vector<Handle> imports) : imports(imports) {}

    virtual void print(std::wostream &os) const
    {
      os << "ImportDecls ["<<handle()<<"] ( imports = [ ";
      for(auto h : imports) os << int(h) << " ";
      os << "] )" << std::endl;
    }

    void add_imports(Handle h);
  };

  class ImportDecl : public Token
  {
  public:
    std::vector<Handle> module;

    ImportDecl() {}
    ImportDecl(std::vector<Handle> module) : module(module) {}

    virtual void print(std::wostream &os) const
    {
      os << "ImportDecl ["<<handle()<<"] ( name = [ ";
      for(auto h : module) os << int(h) << " ";
      os << "] )" << std::endl;
    }

    void add_import(Handle h);
  };

  class NamespaceDecl : public Token
  {
  public:
    std::vector<Handle> name;
    std::vector<Handle> declarations;

    NamespaceDecl() {}
    NamespaceDecl(std::vector<Handle> name, std::vector<Handle> decls = {}) : name(name), declarations(decls) {}

    virtual void print(std::wostream &os) const
    {
      os << "NamespaceDecl [" << handle() << "] ( name = [ ";
      for(auto h : name) os << int(h) << " ";
      os << "], decls = [ ";
      for(auto h : declarations) os << int(h) << " ";
      os << "] )" << std::endl;
    }

    void fillin_decls(Handle h);
  };

  class VariableDecls : public Token
  {
  public:
    std::vector<Handle> attribs;
    std::vector<Handle> decls;
    Handle docstring;

    VariableDecls() {}
    VariableDecls(std::vector<Handle> attribs, std::vector<Handle> decls, Handle doc = Handle())
      : attribs(attribs), decls(decls), docstring(doc) {}

    virtual void print(std::wostream &os) const
    {
      os << "VariableDecls ["<<handle()<<"] ( attribs = [ ";
      for(auto h : attribs) os << int(h) << " ";
      os << "], decls = [ ";
      for(auto h : decls) os << int(h) << " ";
      os << "])" << std::endl;
    }

    void fillin_decls(Handle h);
  };

  class VariableDecl : public Token
  {
  public:
    Handle id;
    Handle type;
    Handle value;

    VariableDecl() {}
    VariableDecl(Handle id, Handle type, Handle value) : id(id), type(type), value(value) {}

    virtual void print(std::wostream &os) const
    {
      os << "VariableDecl ["<<handle()<<"] ( id = " << int(id) << ", type = "<< int(type) << ", value = "<<int(value)<<")" <<std::endl;
    }

    void fillin_decls(Handle h, std::vector<Handle> attribs);
  };

  class ArrayLiteral : public Token
  {
  public:
    std::vector<Handle> exprs;

    ArrayLiteral() {}
    ArrayLiteral(std::vector<Handle> elements) : exprs(elements) {}

    virtual void print(std::wostream &os) const { os << L"[ArrayLiteral]"; }
  };

  class MemberOperator : public Token
  {
  public:
    Handle left;
    Handle right;

    MemberOperator(Handle l, Handle r) : left(l), right(r) {}
  };

  class CallOperator : public Token
  {
  public:
    Handle left;
    std::vector<Handle> right;

    CallOperator(Handle l, std::vector<Handle> r) : left(l), right(r) {}
  };

  class IndexOperator : public Token
  {
  public:
    Handle left;
    std::vector<Handle> right;

    IndexOperator(Handle l, std::vector<Handle> r) : left(l), right(r) {}
  };

  class IfElseExpression : public Token
  {
  public:
    std::vector<std::pair<Handle,Handle>> if_exprs;
    Handle else_expr;

    IfElseExpression() {}
    IfElseExpression(Handle else_expr) : else_expr(else_expr) {}
  };

  class TryExceptExpression : public Token
  {
  public:
    Handle expr = Handle();
    std::vector<std::pair<Handle, Handle>> catches;

    TryExceptExpression() {}
    TryExceptExpression(std::pair<Handle,Handle> acatch) : catches({acatch}) {}
  };

  class AssignOperator : public Token
  {
  public:
    Handle left;
    Handle right;

    AssignOperator() {}
    AssignOperator(Handle l, Handle r) : left(l), right(r) {}
  };

  class BinaryUserOperator : public Token
  {
  public:
    Handle left;
    Handle right;
    std::string op;

    BinaryUserOperator() {}
    BinaryUserOperator(std::string o, Handle l, Handle r) : left(l), right(r), op(o) {}
  };

  class BinaryUserAssignOperator : public Token
  {
  public:
    Handle left;
    Handle right;
    std::string op;

    BinaryUserAssignOperator() {}
    BinaryUserAssignOperator(std::string o, Handle l, Handle r) : left(l), right(r), op(o) {}
  };

  class RangeOperator : public Token
  {
  public:
    Handle left;
    Handle right;

    RangeOperator() {}
    RangeOperator(Handle l, Handle r) : left(l), right(r) {}
  };

  class CompareOperator : public Token
  {
  public:
    Handle left;
    Handle right;
    std::string rel;

    CompareOperator() {}
    CompareOperator(std::string o, Handle l, Handle r) : left(l), right(r), rel(o) {}
  };

  class UnaryPrefixUserOperator : public Token
  {
  public:
    Handle expr;
    std::string op;

    UnaryPrefixUserOperator() {}
    UnaryPrefixUserOperator(std::string o, Handle e) : expr(e), op(o) {}
  };

  class UnaryPostfixUserOperator : public Token
  {
  public:
    Handle expr;
    std::string op;

    UnaryPostfixUserOperator() {}
    UnaryPostfixUserOperator(std::string o, Handle e) : expr(e), op(o) {}
  };

  class PointerTypeOperator : public Token
  {
  public:
    Handle type;

    PointerTypeOperator(Handle t) : type(t) {}
  };

  class StorePointerOperator : public Token
  {
  public:
    Handle left;
    Handle right;

    StorePointerOperator() {}
    StorePointerOperator(Handle l, Handle r) : left(l), right(r) {}
  };


  class RelationOperator : public Token
  {
  public:
    Handle left;
    Handle right;
    std::string rel;

    RelationOperator() {}
    RelationOperator(std::string o, Handle l, Handle r) : left(l), right(r), rel(o) {}
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
  };

  class NoneOperator : public Token
  {
  public:
    NoneOperator() {}
  };

  class PostfixLiteralOperator : public Token
  {
  public:
    Handle literal;
    Handle op;

    PostfixLiteralOperator() {}
    PostfixLiteralOperator(Handle literal, Handle op) : literal(literal), op(op) {}

    virtual void print(std::wostream &os) const { os << "PostfixLiteralOperator ["<<handle()<<"] ( literal = "<<int(literal)<<", operator = "<<int(op)<<")"<<std::endl;}
  };
}
