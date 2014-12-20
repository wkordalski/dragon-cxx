class Match
{
};

class RKeyword : public Match
{
  std::wstring text;
};

class RIdentifier : public Match
{

};

class ROperator : public Match
{
  std::wstring text;
};

class RNewline : public Match
{

};

class RIndent : public Match
{

};

class RDedent : public Match
{

};

class RSequence : public Match
{
  std::vector<Match *> sequence;
};

class RAlternative : public Match
{
  std::vector<Match *> aternatives;
};

class RRepeat : public Match
{
  Match * match;
  int min;
  int max;    // max = -1 => infinity
};
