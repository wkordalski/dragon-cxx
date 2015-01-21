#pragma once

#include "lexer.hpp"
#include "parser.hpp"

namespace dragon
{
  class Scanner
  {
    SourceReader sr;
    CommentRemover cr;
    Tokenizer tk;
    std::string filename;
  public:
    Scanner(std::wistream &source, std::string filename = "<input>"): sr(source), cr(sr), tk(cr), filename(filename) {}
    int lex(dragon::Parser::semantic_type * val, dragon::Parser::location_type * loc);
  };
}
