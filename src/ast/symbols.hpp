#pragma once

namespace dragon
{
  class ISymbolTable
  {
  public:
    virtual Handle get_parent_table() = 0;

    Handle lookup(Handle identifier)
    {
      Handle res = Handle();
      ISymbolTable *curr = this;
      if(curr->lookup_this_only(identifier, res)) return res;
      Handle h = curr->get_parent_table();
      while(h)
      {
        curr = h.as<ISymbolTable>();
        if(curr->lookup_this_only(identifier, res)) return res;
        h = curr->get_parent_table();
      }
      return res;
    }

    virtual bool lookup_this_only(Handle identifier, Handle &result) = 0;
  };
}
