#pragma once

#include "lexer.hpp"
#include "parser.hpp"

namespace dragon
{
  class Scanner
  {
    // TODO
  public:
    Scanner(std::wistream &source): sr(source), cr(sr), tk(cr) {}
    int lex(dragon::Parser::semantic_type * yylval);

    SourceReader sr;
    CommentRemover cr;
    Tokenizer tk;
  };
}
