#pragma once

#include "lexer.hpp"
#include "parser.hpp"

namespace dragon
{
  class Scanner
  {
  public:
    virtual int lex(dragon::Parser::semantic_type * val, dragon::Parser::location_type * loc) = 0;
  };

  class FileScanner : public Scanner
  {
    SourceReader sr;
    CommentRemover cr;
    Tokenizer tk;
    std::string filename;
  public:
    FileScanner(std::wistream &source, std::string filename = "<input>"): sr(source), cr(sr), tk(cr), filename(filename) {}
    virtual int lex(dragon::Parser::semantic_type * val, dragon::Parser::location_type * loc);
  };
}
