
Match * init_rules()
{
  auto declaration = new RAlternative();
  auto program = new RRepeat(declaration);
  auto module_name = new RRepeat(new RIdentifier(), new ROperator(L"."));
  auto import_smt = new RSequence(new RKeyword(L"import"), module_name);
  declaration->add(import_smt);
  return program;
}
