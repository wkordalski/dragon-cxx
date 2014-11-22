#include "lexer.hpp" 

SourceReader::SourceReader(std::wistream &source)
{
  this->_stream = source;
}

virtual bool SourceReader::_next(Character &value)
{
  if(!this->_stream) return false;
  value = {this->_stream.get(), row, col};
  col++;
  if(is_newline(value.chr))
  {
    col = 0;
    row++;
  }
  return true;
}

virtual bool SourceReader::_source_empty()
{
  if(this->_stream) return false;
  return true;
}

CommentRemover::CommentRemover(IGenerator<Character> &source)
{
  this->_source = source;
}



virtual bool CommentRemover::_source_empty()
{
  return _source.empty();
}