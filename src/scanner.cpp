#include "scanner.hpp"

namespace dragon
{
  // TODO
  int Scanner::lex(dragon::Parser::semantic_type * yylval)
  {
    Token *t;
    if(tk.get(t))
    {
      yylval->token = new Handle(t);
      // determine token type!
      if(dynamic_cast<Newline*>(t)) return Parser::token::NEWLINE;
      if(dynamic_cast<Indent*>(t)) return Parser::token::INDENT;
      if(dynamic_cast<Dedent*>(t)) return Parser::token::DEDENT;
      if(dynamic_cast<Literal*>(t)) return Parser::token::LITERAL;
      auto id = dynamic_cast<Identifier*>(t);
      if(id)
      {
        // Check keyword
        if(id->text == L"var") return Parser::token::VAR_KEYWORD;
        if(id->text == L"if") return Parser::token::IF_KEYWORD;
        if(id->text == L"else") return Parser::token::ELSE_KEYWORD;
        return Parser::token::IDENTIFIER;
      }
      auto op = dynamic_cast<Operator*>(t);
      if(op)
      {
        if(op->text == L";") return Parser::token::SEMICOLON;
        if(op->text == L":") return Parser::token::COLON;
        if(op->text == L"=") return Parser::token::EQUAL;
        if(op->text == L"(") return Parser::token::LEFT1;
        if(op->text == L")") return Parser::token::RIGHT1;
        if(op->text == L"->") return Parser::token::ARROWR;
      }
    }
    return 0;
  }
}
