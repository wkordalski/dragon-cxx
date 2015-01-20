#include "scanner.hpp"

namespace dragon
{
  // TODO
  int Scanner::lex(dragon::Parser::semantic_type * yylval, dragon::Parser::location_type * yylloc)
  {
    Token *t;
    if(tk.get(t))
    {
      yylval->token = new Handle(t);
      // determine token type!
      if(auto tt = dynamic_cast<Newline*>(t))
      {
        *yylloc = tt->place.location(filename);
        return Parser::token::NEWLINE;
      }
      if(auto tt = dynamic_cast<Indent*>(t))
      {
        *yylloc = tt->place.location(filename);
        return Parser::token::INDENT;
      }
      if(auto tt = dynamic_cast<Dedent*>(t))
      {
        *yylloc = tt->place.location(filename);
        return Parser::token::DEDENT;
      }
      if(auto tt = dynamic_cast<Literal*>(t))
      {
        *yylloc = tt->place.location(filename);
        return Parser::token::LITERAL;
      }
      if(auto tt = dynamic_cast<Identifier*>(t))
      {
        *yylloc = tt->place.location(filename);
        // Check keyword
        if(tt->text == L"and") return Parser::token::AND_KEYWORD;
        if(tt->text == L"as") return Parser::token::AS_KEYWORD;
        if(tt->text == L"assert") return Parser::token::ASSERT_KEYWORD;
        if(tt->text == L"break") return Parser::token::BREAK_KEYWORD;
        if(tt->text == L"class") return Parser::token::CLASS_KEYWORD;
        if(tt->text == L"continue") return Parser::token::CONTINUE_KEYWORD;
        if(tt->text == L"cue") return Parser::token::CUE_KEYWORD;
        if(tt->text == L"def") return Parser::token::DEF_KEYWORD;
        if(tt->text == L"elif") return Parser::token::ELIF_KEYWORD;
        if(tt->text == L"else") return Parser::token::ELSE_KEYWORD;
        if(tt->text == L"enum") return Parser::token::ENUM_KEYWORD;
        if(tt->text == L"except") return Parser::token::EXCEPT_KEYWORD;
        if(tt->text == L"finally") return Parser::token::FINALLY_KEYWORD;
        if(tt->text == L"for") return Parser::token::FOR_KEYWORD;
        if(tt->text == L"from") return Parser::token::FROM_KEYWORD;
        if(tt->text == L"get") return Parser::token::GET_KEYWORD;
        if(tt->text == L"if") return Parser::token::IF_KEYWORD;
        if(tt->text == L"import") return Parser::token::IMPORT_KEYWORD;
        if(tt->text == L"in") return Parser::token::IN_KEYWORD;
        if(tt->text == L"interface") return Parser::token::INTERFACE_KEYWORD;
        if(tt->text == L"is") return Parser::token::IS_KEYWORD;
        if(tt->text == L"let") return Parser::token::LET_KEYWORD;
        if(tt->text == L"namespace") return Parser::token::NAMESPACE_KEYWORD;
        if(tt->text == L"not") return Parser::token::NOT_KEYWORD;
        if(tt->text == L"or") return Parser::token::OR_KEYWORD;
        if(tt->text == L"raise") return Parser::token::RAISE_KEYWORD;
        if(tt->text == L"return") return Parser::token::RETURN_KEYWORD;
        if(tt->text == L"set") return Parser::token::SET_KEYWORD;
        if(tt->text == L"then") return Parser::token::THEN_KEYWORD;
        if(tt->text == L"trace") return Parser::token::TRACE_KEYWORD;
        if(tt->text == L"try") return Parser::token::TRY_KEYWORD;
        if(tt->text == L"type") return Parser::token::TYPE_KEYWORD;
        if(tt->text == L"use") return Parser::token::USE_KEYWORD;
        if(tt->text == L"var") return Parser::token::VAR_KEYWORD;
        if(tt->text == L"where") return Parser::token::WHERE_KEYWORD;
        if(tt->text == L"while") return Parser::token::WHILE_KEYWORD;
        if(tt->text == L"yield") return Parser::token::YIELD_KEYWORD;
        return Parser::token::IDENTIFIER;
      }
      if(auto tt = dynamic_cast<Operator*>(t))
      {
        *yylloc = tt->place.location(filename);
        // Check operator
        if(tt->text == L"&") return Parser::token::AMPERSAND;
        if(tt->text == L"&=") return Parser::token::AMPERSAND_EQUAL;
        if(tt->text == L"->") return Parser::token::ARROWR;
        if(tt->text == L"*") return Parser::token::ASTERISK;
        if(tt->text == L"*=") return Parser::token::ASTERISK_EQUAL;
        if(tt->text == L"@") return Parser::token::AT;
        if(tt->text == L"^") return Parser::token::CARET;
        if(tt->text == L"^=") return Parser::token::CARET_EQUAL;
        if(tt->text == L":") return Parser::token::COLON;
        if(tt->text == L",") return Parser::token::COMMA;
        if(tt->text == L"<>") return Parser::token::DIFFERENT;
        if(tt->text == L"<>=") return Parser::token::DIFFERENT_EQUAL;
        if(tt->text == L".") return Parser::token::DOT;
        if(tt->text == L"&&") return Parser::token::DOUBLE_AMPERSAND;
        if(tt->text == L"&&=") return Parser::token::DOUBLE_AMPERSAND_EQUAL;
        if(tt->text == L"**") return Parser::token::DOUBLE_ASTERISK;
        if(tt->text == L"**=") return Parser::token::DOUBLE_ASTERISK_EQUAL;
        if(tt->text == L"..") return Parser::token::DOUBLE_DOT;
        if(tt->text == L"==") return Parser::token::DOUBLE_EQUAL;
        if(tt->text == L">>") return Parser::token::DOUBLE_GREATER;
        if(tt->text == L">>=") return Parser::token::DOUBLE_GREATER_EQUAL;
        if(tt->text == L"<<") return Parser::token::DOUBLE_LESSER;
        if(tt->text == L"<<=") return Parser::token::DOUBLE_LESSER_EQUAL;
        if(tt->text == L"--") return Parser::token::DOUBLE_MINUS;
        if(tt->text == L"||") return Parser::token::DOUBLE_PIPE;
        if(tt->text == L"||=") return Parser::token::DOUBLE_PIPE_EQUAL;
        if(tt->text == L"++") return Parser::token::DOUBLE_PLUS;
        if(tt->text == L"=") return Parser::token::EQUAL;
        if(tt->text == L"!") return Parser::token::EXCLAMATION;
        if(tt->text == L">") return Parser::token::GREATER;
        if(tt->text == L">=") return Parser::token::GREATER_EQUAL;
        if(tt->text == L"(") return Parser::token::LEFT1;
        if(tt->text == L"[") return Parser::token::LEFT2;
        if(tt->text == L"{") return Parser::token::LEFT3;
        if(tt->text == L"<") return Parser::token::LESSER;
        if(tt->text == L"<=") return Parser::token::LESSER_EQUAL;
        if(tt->text == L"-") return Parser::token::MINUS;
        if(tt->text == L"-=") return Parser::token::MINUS_EQUAL;
        if(tt->text == L"!<>") return Parser::token::NOT_DIFFERENT;
        if(tt->text == L"!<>=") return Parser::token::NOT_DIFFERENT_EQUAL;
        if(tt->text == L"!==") return Parser::token::NOT_DOUBLE_EQUAL;
        if(tt->text == L"!=") return Parser::token::NOT_EQUAL;
        if(tt->text == L"!>") return Parser::token::NOT_GREATER;
        if(tt->text == L"!>=") return Parser::token::NOT_GREATER_EQUAL;
        if(tt->text == L"!<") return Parser::token::NOT_LESSER;
        if(tt->text == L"!<=") return Parser::token::NOT_LESSER_EQUAL;
        if(tt->text == L"%") return Parser::token::PERCENT;
        if(tt->text == L"%=") return Parser::token::PERCENT_EQUAL;
        if(tt->text == L"|") return Parser::token::PIPE;
        if(tt->text == L"|=") return Parser::token::PIPE_EQUAL;
        if(tt->text == L"+") return Parser::token::PLUS;
        if(tt->text == L"+=") return Parser::token::PLUS_EQUAL;
        if(tt->text == L")") return Parser::token::RIGHT1;
        if(tt->text == L"]") return Parser::token::RIGHT2;
        if(tt->text == L"}") return Parser::token::RIGHT3;
        if(tt->text == L";") return Parser::token::SEMICOLON;
        if(tt->text == L"/") return Parser::token::SLASH;
        if(tt->text == L"/=") return Parser::token::SLASH_EQUAL;
        if(tt->text == L"~") return Parser::token::TILDE;
        if(tt->text == L"~=") return Parser::token::TILDE_EQUAL;
        if(tt->text == L"===") return Parser::token::TRIPPLE_EQUAL;
        if(tt->text == L">>>") return Parser::token::TRIPPLE_GREATER;
        if(tt->text == L">>>=") return Parser::token::TRIPPLE_GREATER_EQUAL;
        if(tt->text == L"<<<") return Parser::token::TRIPPLE_LESSER;
        if(tt->text == L"<<<=") return Parser::token::TRIPPLE_LESSER_EQUAL;
      }
    }
    return 0;
  }
}
