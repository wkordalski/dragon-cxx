#include "glr.hpp"

#include <cassert>
#include <iostream>
#include <stack>

using namespace std;

#define YYCHK1(YYE)                                                          \
  do {                                                                       \
    switch (YYE) {                                                           \
    case yyok:                                                               \
      break;                                                                 \
    case yyabort:                                                            \
      goto yyabortlab;                                                       \
    case yyaccept:                                                           \
      goto yyacceptlab;                                                      \
    case yyerr:                                                              \
      goto yyuser_error;                                                     \
    default:                                                                 \
      goto yybuglab;                                                         \
    }                                                                        \
  } while (0)


namespace dragon
{
  ////////////////////////////////////////////////////////////////////////////
  // GLR state
  ////////////////////////////////////////////////////////////////////////////
  class GLRState
  {
    // true  -> yysval
    // false -> yyfirstVal
    bool resolved;

    // Number of corresponding LALR(1) machine state
    int state;


    // Source position of the last token produced by my symbol
    size_t posn;

    // UNION
    // ----------------------------------------------
    // Semantic value for this state
    YYSTYPE yysval;
    // -- OR
    // First in a chain of alteernative reductions producing the
    // non-terminal corresponding to this state, threaded through yynext
    yySemanticOption *yyfirstVal;
    // ----------------------------------------------

    // Source location for this state
    YYLTYPE yyloc;
  };

  ////////////////////////////////////////////////////////////////////////////
  // Grammar table manipulation
  ////////////////////////////////////////////////////////////////////////////
  static inline int RHSLength(int rule)
  {
    return yyr2[rule];
  }

  static inline yySymbol LHSSymbol(int rule)
  {
    return yyr1[rule];
  }

  static inline void getLRActions(int state, int token,
                                  int &action, (const short int *) &conflicts)
  {
    int index = yypact[state] + token;
    if (yypact_value_is_default (yypact[yystate])
        || index < 0 || YYLAST < index || yycheck[index] != token)
    {
      action = -yydefact[state];
      conflicts = yyconfl;
    }
    else if (! yytable_value_is_error (yytable[index]))
    {
      action = yytable[index];
      conflicts = yyconfl + yyconflp[index];
    }
    else
    {
      action = 0;
      conflicts = yyconfl + yyconflp[index];
    }
  }

  static inline int getLRGotoState(int state, yySymbol sym)
  {
    int yyr = yypgoto[sym - YYNTOKENS] + state;
    if(0 <= yyr and yyr <= YYLAST and yychack[yyr] == state)
      return yytable[yyr];
    else
      return yydefgoto[yysym - YYNTOKENS];
  }

  static inline bool isShiftAction(int action)
  {
    return 0 < action;
  }

  static inline bool isErrorAction(int action)
  {
    return 0 == action;
  }

  ////////////////////////////////////////////////////////////////////////////
  // Parser implementation
  ////////////////////////////////////////////////////////////////////////////


  Parser::Parser(Scanner &scanner, ostream &dbg = std::cerr)
    : dbgs(dbg), scanner(scanner) {}

  Parser::~Parser() {}

  int Parser::parse()
  {
    int yyresult;
    // Stack of states
    auto yystack = COWStack<yyGLRStackItem>::make();
    // State set [ state (stack item) , readahead ]

    GLRSet stateset = {make_pair(yystack,what?)};
    dbgs << "Starting parse" << endl;

    yychar = YYEMPTY;
    Handle yylval;
    Location yylloc;
    size_t yyposn = 0;

    yyglrShift (yystack, 0, 0, 0, &yylval, &&yylloc);

    while(true)
    {
      /* Two loops for performance */

      /*
       * Standard mode
       * Only deterministic operation - single stack
       */
      while(true)
      {
        int yyrule;
        int yyaction;
        const short int* yyconflicts;
        yyStateNum yystate = yystack.yytops.yystates[0]->yylrState;
        dbgs << "Entering state " << yystate << endl;
        if(yystate == YYFINAL)
          goto yyacceptlab;
        if(yyisDefaultedState(yystate))
        {
          yyrule = yydefaultAction(yystate);
          if(yyrule == 0)
          {
            yystack.yyerror_range[1].yystate.yyloc = yylloc;
            yyreportSyntaxError(yystack);
            goto yyuser_error;
          }
          YYCHK1(yyglrReduce(yystack, 0, yyrule, true));
        }
        else
        {
          yySymbol yytoken;
          if(yychar == YYEMPTY)
          {
            dbgs << "Reading a token: ";
            yychar = scanner.lex(&yylval, &yylloc);
          }

          if(yychar <= YYEOF)
          {
            yychar = yytoken = YYEOF;
            dbgs << "Now at end of input." << endl;
          }
          else
          {
            yytoken = YYTRANSLATE(yychar);
            YY_SYMBOL_PRINT("Next token is", yytoken, &yylval, &yylloc);
          }

          getLRActions(yystate, yytoken, yyaction, yyconflicts);
          if(yyconflicts != 0) break;
          if(yyisShiftAction(yyaction))
          {
            YY_SYMBOL_PRINT("Shifting", yytoken, &yylval, &yylloc);
            yychar = YYEMPTY;
            yyposn++;
            yyglrShift(&yystack, 0, yyaction, yyposn, &yylval, &yylloc);
            if(0 < yystack.yyerrState)
              yystack.yyerrState--;
          } else
          if(yyisErrorAction(yyaction))
          {
            yystack.yyerror_range[1].yystate.yyloc = yylloc;
            yyreportSyntaxError(yystack, 0, -yyaction, true);
            goto yyuser_error;
          }
          else
            YYCHK1(yyglrReduce(yystack, 0, -yyaction, true));
        }
      }

      while(true)
      {
        yySymbol yytoken_to_shift;
        size_t yys;

        for(size_t yys = 0; yys < yystack.yytops.yysize; yys++)
          yystackp->yytops.yylookaheadNeeds[yys] = yychar != YYEMPTY;

        for(size_t yys = 0; yys < yystack.yytops.yysize; yys++)
          YYCHK1 (yyprocessOneStack(&yystack, yys, yyposn, &yylloc));
        yyremoveDeletes(&yystack);
        if(yystack.yytops.yysize == 0)
        {
          yyundeleteLastStack(&yystack);
          if(yystack.yytops.yysize == 0)
            yyFail(&yystack, &yylloc, YY_("syntax error"));
          YYCHK1(yyresolveStack(&yystack));
          dbgs << "Returning to deterministic operation." << endl;
          yystack.yyerror_range[1].yystate.yyloc = yylloc;
          yyreportSyntaxError(&yystack);
          goto yyuser_error;
        }

        yytoken_to_shift = YYTRANSLATE(yychar);
        yychar = YYEMPTY;
        yyposn++;
        for(size_t yys = 0; yys < yystack.yytops.yysize; yys++)
        {
          int yyaction;
          const short int * yyconflicts;
          yyStateNum yystate = yystack.yytops.yystates[yys]->yylrState;
          getLRActions(yystate, yytoken_to_shift, yyaction, yyconflicts);
          /* Note that yyconflicts were handled by yyprocessOneStack */
          dbgs << "On stack " << yys;
          YY_SYMBOL_PRINT("shifting", yytoken_to_shift, &yylval, &yylloc);
          yyglrShift(&yystack, yys, yyaction, yyposn, &yylval, &yylloc);
          dbgs << "Stack " << yys << " now in state #"
               << yystack.yytops.yystates[yys]->yylrState << endl;
        }
        if(yystack.yytops.yysize == 1)
        {
          YYCHK1(yyresolveStack(&yystack));
          dbgs << "Returning to deterministic operation." << endl;
          yycompressStack(&yystack);
          break;
        }
      }
      continue;

    yyuser_error:
      yyrecoverSyntaxError(&yystack);
      yyposn = yystack.yytops.yystates[0]->yyposn;
    }

  yyacceptlab:
    yyresult = 0;
    goto yyreturn;

  yybuglab:
    YYASSERT(false);
    goto yyabortlab;

  yyabortlab:
    yyresult = 1;
    goto yyreturn;

  yyexhaustedlab:
    yyerror(&yylloc, YY_("memory exhausted"));
    yyresult = 2;
    goto yyreturn;

  yyreturn:
    if(yychar != YYEMPTY)
      yydestruct("Cleanyp: discarding lookahead",
                 YYTRANSLATE(yychar), &yylval, &yylloc);

    if(yystack.yyitems)
    {
      yyGLRState ** yystates = yystack.yytops.yystates;
      if(yystates)
      {
        size_t yysize = yystack.yytops.yysize;
        size_t yyk;
        for(size_t yyk = 0; yyk < yysize; yyk++)
        {
          if(yystates[yyk])
          {
            while(yystates[yyk])
            {
              yyGLRState *yys = yystates[yyk];
              yystack.yyerror_range[1].yystate.yyloc = yys->yyloc;
              if(yys->yypred != yy_NULLPTR)
                yydestroyGLRState("Cleanup: popping", yys);
              yystates[yyk] = yys->yypred;
              yystack.yynextFree--;
              yystack.yyspaceLeft++;
            }
            break;
          }
        }
      }
      yyfreeGLRStack(&&yystack);
    }
    return yyresult;
  }

  void Parser::shift(GLRStack &stack,
                     int state, size_t posn, semantic &val, location &loc)
  {
    GLRState s;
    s.resolved = true;
    s.posn = posn;
    s.state = state;
    s.val = val;
    s.loc = loc;
    stack = stack->push(s);
  }

  void Parser::shift_defer(GLRStack &stack,
                           int state, size_t posn, GLRState rhs, int rule)
  {
    GLRState s;
    s.resolved = false;
    s.posn = posn;
    s.state = state;
    // firstval TODO
    stack = stack->push(s);
  }

  // Consumes RHS of reduced rule
  // and setups val and loc with yyuserAction
  void Parser::action(GLRStack &stack,
                      int rule, semantic &val, location &loc)
  {
    int nrhs = RHSLength(rule);
    std::vector<yyGLRStackItem> rhs(nrhs);
    for(int i = 0; i < nrhs; i++)
    {
      rhs.push_back(stack->top());
      stack = stack->pop();
    }
    //user(rule, stackitems, stack, val, loc [, parser, scanner])
    yyuserAction(rule, rhs, stack, val, loc);
  }

  void Parser::action_defer(GLRStack &stack, GLRSetElement &elt, GLRState state,
                            std::vector<GLRState> rhs, int rule)
  {
    SemanticOption s;
    s.state = rhs;
    s.rule = rule;
    if(elt.second)
    {
      o.rawchar = yychar;
      o.val = semantic();
      o.loc = location();
    }
    else
    {
      o.rawchar = YYEMPTY;
    }
    // TODO
  }

  void Parser::reduce(GLRStack &stack, GLRSet &set,
                      int rule, bool force_eval)
  {
    size_t posn = stack->top().posn;
    if(force_eval or set.size() <= 1)
    {
      semantic val;
      location loc;
      action(stack, rule, val, loc);
      shift(stack, getLRGotoState(stack->top().state, LHSSymbol(rule)),
            posn, val, loc);
    }
    else
    {
      int newstate;
      GLRStack yys = stack;
      GLRStack yys0 = stack;
      for(int yyn = RHSLength(rule); 0 < yyn; yyn--)
      {
        yys = yys->pop();
      }
      int newstate = getLRGotoState(yys->top().state, LHSSymbol(rule));
      // Stack reduced - but action deferred
      for(GLRStack g : set)
      {
        if(g == stack) continue;
        while(g != yys && !g.empty() && g->top().posn >= posn)
        {
          if(g->top().state == newstate and g->pop() == yys)
          {
            // TODO
          }
        }
      }
      stack = yys;
      shift_defer(stack, newstate, posn, yys0, rule);
    }
  }
}
