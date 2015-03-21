%require "2.4.1"
%glr-parser
%skeleton "glr.cc"
%locations
%defines
%define api.namespace {dragon}
%define parser_class_name {Parser}
%parse-param { dragon::Scanner &scanner }
%parse-param { dragon::Handle &root }
%lex-param   { dragon::Scanner &scanner }


%code requires {
	#include "config.hpp"

	#define STRINGIZE_NX(A) #A
	#define STRINGIZE(A) STRINGIZE_NX(A)

	#include STRINGIZE(SYNTACTIC_HPP_FILE)

	#include <vector>

	namespace dragon {
		class Scanner;
		class Handle;
	}
}

%code {
	// Only in parser.cpp - so not visible outside
	using namespace dragon;
	// Prototype for the yylex function
	static int yylex(dragon::Parser::semantic_type * zzlval, dragon::Parser::location_type * zzlloc, dragon::Scanner &scanner);

	template<typename T>
	void del(T a)
	{
		delete a;
	}
	template<typename T, typename... Args>
	void del(T a, Args... b)
	{
		delete a;
		if(sizeof...(b)) del(b...);
	}

	template<typename T>
	T* as(Handle *h)
	{
		return dynamic_cast<T*>(&(*(*(*h))));
	}

	template<typename T, typename... Args>
	Handle * make(Args... args)
	{
		return new Handle(new T(args...));
	}


	std::vector<Handle> * list()
	{
		return new std::vector<Handle>();
	}

	template<typename T, typename... Args>
	std::vector<Handle> * list(Args... args, T v)
	{
		auto l = list(args...);
		l->push_back(v);
		return l;
	}

	template<typename T>
	std::vector<Handle> * list(T v)
	{
		auto l = list();
		l->push_back(v);
		return l;
	}
}


%union {
	dragon::Handle * token;
	std::vector<dragon::Handle> * list;
}

%token <token> IDENTIFIER "[#]"
%token <token> LITERAL "[@]"
%token <token> NEWLINE "[--]"
%token <token> INDENT "[>>]"
%token <token> DEDENT "[<<]"

%token <token> AND_KEYWORD "and"
%token <token> AS_KEYWORD "as"
%token <token> ASSERT_KEYWORD "assert"
%token <token> BREAK_KEYWORD "break"
%token <token> CLASS_KEYWORD "class"
%token <token> CONTINUE_KEYWORD "continue"
%token <token> CUE_KEYWORD "cue"
%token <token> DEF_KEYWORD "def"
%token <token> ELIF_KEYWORD "elif"
%token <token> ELSE_KEYWORD "else"
%token <token> ENUM_KEYWORD "enum"
%token <token> EXCEPT_KEYWORD "except"
%token <token> FINALLY_KEYWORD "finally"
%token <token> FOR_KEYWORD "for"
%token <token> FROM_KEYWORD "from"
%token <token> GET_KEYWORD "get"
%token <token> IF_KEYWORD "if"
%token <token> IMPORT_KEYWORD "import"
%token <token> IN_KEYWORD "in"
%token <token> INTERFACE_KEYWORD "interface"
%token <token> IS_KEYWORD "is"
%token <token> LET_KEYWORD "let"
%token <token> LOCAL_KEYWORD "local"
%token <token> NAMESPACE_KEYWORD "namespace"
%token <token> NOT_KEYWORD "not"
%token <token> OR_KEYWORD "or"
%token <token> RAISE_KEYWORD "raise"
%token <token> REF_KEYWORD "ref"
%token <token> RETURN_KEYWORD "return"
%token <token> SET_KEYWORD "set"
%token <token> STATIC_KEYWORD "static"
%token <token> THEN_KEYWORD "then"
%token <token> TRACE_KEYWORD "trace"
%token <token> TRY_KEYWORD "try"
%token <token> TYPE_KEYWORD "type"
%token <token> USE_KEYWORD "use"
%token <token> VAR_KEYWORD "var"
%token <token> WHERE_KEYWORD "where"
%token <token> WHILE_KEYWORD "while"
%token <token> YIELD_KEYWORD "yield"

%token <token> AMPERSAND "&"
%token <token> AMPERSAND_EQUAL "&="
%token <token> ARROWL "<-"
%token <token> ARROWR "->"
%token <token> ASTERISK "*"
%token <token> ASTERISK_EQUAL "*="
%token <token> AT "@"
%token <token> CARET "^"
%token <token> CARET_EQUAL "^="
%token <token> COLON ":"
%token <token> COMMA ","
%token <token> DIFFERENT "<>"
%token <token> DIFFERENT_EQUAL "<>="
%token <token> DOT "."
%token <token> DOUBLE_AMPERSAND "&&"
%token <token> DOUBLE_AMPERSAND_EQUAL "&&="
%token <token> DOUBLE_ASTERISK "**"
%token <token> DOUBLE_ASTERISK_EQUAL "**="
%token <token> DOUBLE_DOT ".."
%token <token> DOUBLE_EQUAL "=="
%token <token> DOUBLE_GREATER ">>"
%token <token> DOUBLE_GREATER_EQUAL ">>="
%token <token> DOUBLE_LESSER "<<"
%token <token> DOUBLE_LESSER_EQUAL "<<="
%token <token> DOUBLE_MINUS "--"
%token <token> DOUBLE_PIPE "||"
%token <token> DOUBLE_PIPE_EQUAL "||="
%token <token> DOUBLE_PLUS "++"
%token <token> EQUAL "="
%token <token> EXCLAMATION "!"
%token <token> GREATER ">"
%token <token> GREATER_EQUAL ">="
%token <token> LEFT1 "("
%token <token> LEFT2 "["
%token <token> LEFT3 "{"
%token <token> LESSER "<"
%token <token> LESSER_EQUAL "<="
%token <token> LESSER_EQUAL_GREATER "<=>"
%token <token> MINUS "-"
%token <token> MINUS_EQUAL "-="
%token <token> NOT_DIFFERENT "!<>"
%token <token> NOT_DIFFERENT_EQUAL "!<>="
%token <token> NOT_DOUBLE_EQUAL "!=="
%token <token> NOT_EQUAL "!="
%token <token> NOT_GREATER "!>"
%token <token> NOT_GREATER_EQUAL "!>="
%token <token> NOT_LESSER "!<"
%token <token> NOT_LESSER_EQUAL "!<="
%token <token> PERCENT "%"
%token <token> PERCENT_EQUAL "%="
%token <token> PIPE "|"
%token <token> PIPE_EQUAL "|="
%token <token> PLUS "+"
%token <token> PLUS_EQUAL "+="
%token <token> RIGHT1 ")"
%token <token> RIGHT2 "]"
%token <token> RIGHT3 "}"
%token <token> SEMICOLON ";"
%token <token> SLASH "/"
%token <token> SLASH_EQUAL "/="
%token <token> TILDE "~"
%token <token> TILDE_EQUAL "~="
%token <token> TRIPPLE_EQUAL "==="
%token <token> TRIPPLE_GREATER ">>>"
%token <token> TRIPPLE_GREATER_EQUAL ">>>="
%token <token> TRIPPLE_LESSER "<<<"
%token <token> TRIPPLE_LESSER_EQUAL "<<<="

%type <token>  expr0  expr1  expr2  expr3  expr4  expr5  expr6  expr7  expr8  expr9
%type <token> expr10 expr11 expr12 expr13 expr14 expr15 expr16 expr17 expr18 expr19 expr15a
%type <token> expr20 expr21 expr22

%type <token> expr_all expr_cond expr_val expr_noass

%type <token> declaration

%type <token> elif_then_elses except_then_exprs

%type <token> var_single_decl

%type <token> use_declaration_element

%type <list> id_dot_list declaration_block expr_list expr_list_noempty attribute attribute_list attribute_list_noempty var_single_decl_list
%type <list> module_list use_declaration_list

%%

program : declaration_block											{ root = Handle(new File(*$1)); del($1); }
	| "[@]" "[--]" declaration_block							{ root = Handle(new File(*$3, *$1)); del($1,$2,$3); }
	;

/* ------------------------------------------------------------------------------------------------------ */
/*                                              EXPRESSIONS                                               */
/* ------------------------------------------------------------------------------------------------------ */

expr0
	: IDENTIFIER																	{ $$ = make<IdentifierExpression>(*$1); del($1); }
	| LITERAL																			{ $$ = $1; as<Literal>($$)->parse_literal(); }
	| "(" ")"																			{ $$ = make<NoneOperator>(); del($1,$2); }
	| "(" expr_all ")"														{ $$ = $2; del($1, $3);}
	| "[" expr_list "]"		/* an array literal */	{ $$ = make<ArrayLiteral>(*$2); del($1, $2, $3); }
	/*| "[" expr_all "for" /* TODO * / "]"*/ /* a nice inline generator */
	| "{" expr_list "}"  /* what this should be? - I don't plan any literals for sets or maps */
	;

expr1 : expr0																		{ $$ = $1; }
	| expr1 "." expr0															{ $$ = make<MemberOperator>(*$1, *$3); del($1,$2,$3); }
	| expr1 "++"																	{ $$ = make<UnaryPostfixUserOperator>("++", *$2); del($1,$2); }
	| expr1 "--"																	{ $$ = make<UnaryPostfixUserOperator>("--", *$2); del($1,$2); }
	| expr1 "*"  /* pointer type */								{ $$ = make<PointerTypeOperator>(*$1); del($1,$2);}
	| expr1 "(" expr_list ")"											{ $$ = make<CallOperator>(*$1, *$3); del($1,$2,$3,$4);}
	| expr1 "[" expr_list "]"											{ $$ = make<IndexOperator>(*$1, *$3); del($1,$2,$3,$4);}
	;

expr2 : expr1																		{ $$ = $1; }
	| "++" expr2																	{ $$ = make<UnaryPrefixUserOperator>("++", *$2); del($1,$2); }
	| "--" expr2																	{ $$ = make<UnaryPrefixUserOperator>("--", *$2); del($1,$2); }
	;

expr3 : expr2																		{ $$ = $1; }
	| expr3 "**" expr2														{ $$ = make<BinaryUserOperator>("**",*$1,*$3); del($1,$2,$3); }
	;

expr4 : expr3																		{ $$ = $1; }
	| "-" expr4																		{ $$ = make<UnaryPrefixUserOperator>("-", *$2); del($1,$2); }
	| "+" expr4																		{ $$ = make<UnaryPrefixUserOperator>("+", *$2); del($1,$2); }
	| "!" expr4																		{ $$ = make<UnaryPrefixUserOperator>("!", *$2); del($1,$2); }
	| "~" expr4																		{ $$ = make<UnaryPrefixUserOperator>("~", *$2); del($1,$2); }
	;

expr5 : expr4																		{ $$ = $1; }
	| expr5 "*" expr4															{ $$ = make<BinaryUserOperator>("*",*$1,*$3); del($1,$2,$3); }
	| expr5 "/" expr4															{ $$ = make<BinaryUserOperator>("/",*$1,*$3); del($1,$2,$3); }
	| expr5 "%" expr4															{ $$ = make<BinaryUserOperator>("%",*$1,*$3); del($1,$2,$3); }
	;

expr6 : expr5																		{ $$ = $1; }
	| expr6 "+" expr5															{ $$ = make<BinaryUserOperator>("+",*$1,*$3); del($1,$2,$3); }
	| expr6 "-" expr5															{ $$ = make<BinaryUserOperator>("-",*$1,*$3); del($1,$2,$3); }
	| expr6 "~" expr5															{ $$ = make<BinaryUserOperator>("~",*$1,*$3); del($1,$2,$3); }
	;

expr7 : expr6																		{ $$ = $1; }
	| expr7 ">>" expr6														{ $$ = make<BinaryUserOperator>(">>",*$1,*$3); del($1,$2,$3); }
	| expr7 "<<" expr6														{ $$ = make<BinaryUserOperator>("<<",*$1,*$3); del($1,$2,$3); }
	| expr7 ">>>" expr6														{ $$ = make<BinaryUserOperator>(">>>",*$1,*$3); del($1,$2,$3); }
	| expr7 "<<<" expr6														{ $$ = make<BinaryUserOperator>("<<<",*$1,*$3); del($1,$2,$3); }
	;

expr8 : expr7																		{ $$ = $1; }
	| expr8 "&" expr7															{ $$ = make<BinaryUserOperator>("&",*$1,*$3); del($1,$2,$3); }
	;

expr9 : expr8																		{ $$ = $1; }
	| expr9 "^" expr8															{ $$ = make<BinaryUserOperator>("^",*$1,*$3); del($1,$2,$3); }
	;

expr10 : expr9																	{ $$ = $1; }
	| expr10 "|" expr9														{ $$ = make<BinaryUserOperator>("|",*$1,*$3); del($1,$2,$3); }
	;

expr11 : expr10																	{ $$ = $1; }
	| expr11 ".." expr10													{ $$ = make<RangeOperator>(*$1,*$3); del($1,$2,$3); }
	;

expr12 : expr11																	{ $$ = $1; }
	| expr12 "<" expr11														{ $$ = make<CompareOperator>("<",*$1,*$3); del($1,$2,$3); }
	| expr12 "<=" expr11													{ $$ = make<CompareOperator>("<=",*$1,*$3); del($1,$2,$3); }
	| expr12 ">" expr11														{ $$ = make<CompareOperator>(">",*$1,*$3); del($1,$2,$3); }
	| expr12 ">=" expr11													{ $$ = make<CompareOperator>(">=",*$1,*$3); del($1,$2,$3); }
	| expr12 "!>" expr11													{ $$ = make<CompareOperator>("!>",*$1,*$3); del($1,$2,$3); }
	| expr12 "!>=" expr11													{ $$ = make<CompareOperator>("!>=",*$1,*$3); del($1,$2,$3); }
	| expr12 "!<" expr11													{ $$ = make<CompareOperator>("!<",*$1,*$3); del($1,$2,$3); }
	| expr12 "!<=" expr11													{ $$ = make<CompareOperator>("!<=",*$1,*$3); del($1,$2,$3); }
	;

expr13 : expr12																	{ $$ = $1; }
	| expr13 "==" expr12													{ $$ = make<CompareOperator>("==",*$1,*$3); del($1,$2,$3); }
	| expr13 "!=" expr12													{ $$ = make<CompareOperator>("!=",*$1,*$3); del($1,$2,$3); }
	| expr13 "!<>=" expr12												{ $$ = make<CompareOperator>("!<>=",*$1,*$3); del($1,$2,$3); }
	| expr13 "!<>" expr12													{ $$ = make<CompareOperator>("!<>",*$1,*$3); del($1,$2,$3); }
	| expr13 "<>" expr12													{ $$ = make<CompareOperator>("<>",*$1,*$3); del($1,$2,$3); }
	| expr13 "<>=" expr12													{ $$ = make<CompareOperator>("<>=",*$1,*$3); del($1,$2,$3); }
	| expr13 "===" expr12													{ $$ = make<RelationOperator>("===", *$1,*$3); del($1,$2,$3);}
	| expr13 "!==" expr12
	{
		$$ = make<UnaryPrefixUserOperator>("!", Handle(new RelationOperator("===", *$1,*$3))); del($1,$2,$3);
	}
	| expr13 "<=>" expr12													{ $$ = make<RelationOperator>("<=>", *$1,*$3); del($1,$2,$3);}
	;

expr14 : expr13																	{ $$ = $1; }
	| expr14 "&&" expr13													{ $$ = make<BinaryUserOperator>("&&",*$1,*$3); del($1,$2,$3); }
	;

expr15 : expr14																	{ $$ = $1; }
	| expr15 "||" expr14													{ $$ = make<BinaryUserOperator>("||",*$1,*$3); del($1,$2,$3); }
	;

expr15a : expr15																{ $$ = $1; }
	| "if" expr_all "then" expr_all elif_then_elses
	{
		$$ = $5;
		auto &ie = as<IfElseExpression>($$)->if_exprs;
		ie.insert(ie.begin(), std::make_pair(*$2, *$4));
		del($1,$2,$3,$4);
	}
	| "try" expr_all except_then_exprs
	{
		$$ = $3;
		as<TryExceptExpression>($$)->expr = *$2;
		del($1,$2);
	}
	;

expr16 : expr15a																{ $$ = $1; }
	| expr15 "=" expr16														{ $$ = make<AssignOperator>(*$1, *$3); del($1,$2,$3); }
	| expr15 "<-" expr16													{ $$ = make<StorePointerOperator>(*$1, *$3); del($1,$2,$3); }
	| expr15 "+=" expr16													{ $$ = make<BinaryUserAssignOperator>("+",*$1,*$3); del($1,$2,$3); }
	| expr15 "-=" expr16													{ $$ = make<BinaryUserAssignOperator>("-",*$1,*$3); del($1,$2,$3); }
	| expr15 "*=" expr16													{ $$ = make<BinaryUserAssignOperator>("*",*$1,*$3); del($1,$2,$3); }
	| expr15 "/=" expr16													{ $$ = make<BinaryUserAssignOperator>("/",*$1,*$3); del($1,$2,$3); }
	| expr15 "%=" expr16													{ $$ = make<BinaryUserAssignOperator>("%",*$1,*$3); del($1,$2,$3); }
	| expr15 "&=" expr16													{ $$ = make<BinaryUserAssignOperator>("&",*$1,*$3); del($1,$2,$3); }
	| expr15 "|=" expr16													{ $$ = make<BinaryUserAssignOperator>("|",*$1,*$3); del($1,$2,$3); }
	| expr15 "^=" expr16													{ $$ = make<BinaryUserAssignOperator>("^",*$1,*$3); del($1,$2,$3); }
	| expr15 "~=" expr16													{ $$ = make<BinaryUserAssignOperator>("~",*$1,*$3); del($1,$2,$3); }
	| expr15 "<<=" expr16													{ $$ = make<BinaryUserAssignOperator>("<<",*$1,*$3); del($1,$2,$3); }
	| expr15 ">>=" expr16													{ $$ = make<BinaryUserAssignOperator>(">>",*$1,*$3); del($1,$2,$3); }
	| expr15 "<<<=" expr16												{ $$ = make<BinaryUserAssignOperator>("<<<",*$1,*$3); del($1,$2,$3); }
	| expr15 ">>>=" expr16												{ $$ = make<BinaryUserAssignOperator>(">>>",*$1,*$3); del($1,$2,$3); }
	| expr15 "&&=" expr16													{ $$ = make<BinaryUserAssignOperator>("&&",*$1,*$3); del($1,$2,$3); }
	| expr15 "||=" expr16													{ $$ = make<BinaryUserAssignOperator>("||",*$1,*$3); del($1,$2,$3); }
	| expr15 "**=" expr16													{ $$ = make<BinaryUserAssignOperator>("**",*$1,*$3); del($1,$2,$3); }
	| lambda_head "->" expr16
	;

expr17 : expr16																	{ $$ = $1; }
	| expr17 "as" expr16													{ $$ = make<RelationOperator>("as", *$1,*$3); del($1,$2,$3);}
	| expr17 "not" "as" expr16
	{
		$$ = make<UnaryPrefixUserOperator>("!", Handle(new RelationOperator("as", *$1, *$4))); del($1,$2,$3,$4);
	}
	| expr17 "!" "as" expr16
	{
		$$ = make<UnaryPrefixUserOperator>("!", Handle(new RelationOperator("as", *$1, *$4))); del($1,$2,$3,$4);
	}
	;

expr18 : expr17																	{ $$ = $1; }
	| expr18 "," expr17														{ $$ = make<CommaOperator>(*$1,*$3); del($1,$2,$3);}
	;

expr19 : expr18																	{ $$ = $1; }
	| expr19 "is" expr18													{ $$ = make<RelationOperator>("===", *$1,*$3); del($1,$2,$3);} /* or maby <=> ? */
	| expr19 "not" "is" expr18
	{
		$$ = make<UnaryPrefixUserOperator>("!", Handle(new RelationOperator("===", *$1, *$4))); del($1,$2,$3,$4);
	}
	| expr19 "!" "is" expr18
	{
		$$ = make<UnaryPrefixUserOperator>("!", Handle(new RelationOperator("===", *$1, *$4))); del($1,$2,$3,$4);
	}
	;

expr20 : expr19																	{ $$ = $1; }
	| expr20 "in" expr19													{ $$ = make<RelationOperator>("in", *$1,*$3); del($1,$2,$3); } /* only one overloadable relation operator */
	| expr20 "not" "in" expr19
	{
		$$ = make<UnaryPrefixUserOperator>("!", Handle(new RelationOperator("in", *$1, *$4))); del($1,$2,$3,$4);
	}
	| expr20 "!" "in" expr19
	{
		$$ = make<UnaryPrefixUserOperator>("!", Handle(new RelationOperator("in", *$1, *$4))); del($1,$2,$3,$4);
	}
	;

expr21 : expr20																	{ $$ = $1; }
	| "not" expr21																{ $$ = make<UnaryPrefixUserOperator>("!", *$2); del($1,$2); }
	;

expr22 : expr21																	{ $$ = $1; }
	| expr22 "and" expr21													{ $$ = make<BinaryUserOperator>("&&",*$1,*$3); del($1,$2,$3); }
	| expr22 "or" expr21													{ $$ = make<BinaryUserOperator>("||",*$1,*$3); del($1,$2,$3); }
	;

/* ------------------------------------------------------------------------------------------------------ */
/*                                           EXPRESSION HELPERS                                           */
/* ------------------------------------------------------------------------------------------------------ */

except_then_exprs
	: "except" expr_all "then" expr_all
	{
		$$ = make<TryExceptExpression>(std::make_pair(*$2,*$4));
		del($1,$2,$3,$4);
	}
	| "except" expr_all "then" expr_all except_then_exprs
	{
		$$ = $5;
		auto &ie = as<TryExceptExpression>($$)->catches;
		ie.insert(ie.begin(), std::make_pair(*$2, *$4));
		del($1,$2,$3,$4);
	}
	;
elif_then_elses
	: "else" expr_all
	{
		$$ = make<IfElseExpression>(*$2);
		del($1,$2);
	}
	| "elif" expr_all "then" expr_all elif_then_elses
	{
		$$ = $5;
		auto &ie = as<IfElseExpression>($$)->if_exprs;
		ie.insert(ie.begin(), std::make_pair(*$2, *$4));
		del($1,$2,$3,$4);
	}
	;

expr_cond : expr22															{ $$ = $1; };
expr_val : expr17 															{ $$ = $1; };
expr_noass : expr15 														{ $$ = $1; };
expr_list_noempty : expr_val										{ $$ = list(*$1); del($1); }
	| expr_list_noempty "," expr_val							{ $$ = $1; $$->push_back(*$3); del($2, $3); }
	;

expr_list : expr_list_noempty										{ $$ = $1; }
	| /* EMPTY */																	{ $$ = list(); }
	;

expr_all : expr22 ;

/* LAMBDA */

lambda_head : "[#]"
	/* TODO */
	;

/* ------------------------------------------------------------------------------------------------------ */
/*                                               ATTRIBUTES                                               */
/* ------------------------------------------------------------------------------------------------------ */

attribute : "@" expr_list "[--]"								{ $$ = $2; del($1,$3); } ;
attribute_list_noempty : attribute { $$ = $1; } | attribute_list_noempty attribute {$$ = $1; std::move($2->begin(), $2->end(), $1->end());del($2);} ;
attribute_list : attribute_list_noempty {$$ = $1;} | /* EMPTY */  {$$=list();};

/* ------------------------------------------------------------------------------------------------------ */
/*                                              DECLARATIONS                                              */
/* ------------------------------------------------------------------------------------------------------ */

declaration
	: "import" module_list "[--]"
	{
		$$ = make<ImportDecls>(*$2); del($1, $2);
	}
	| "namespace" id_dot_list "[--]" "[>>]" declaration_block "[<<]"
	{
		$$ = make<syntax::NamespaceDeclaration>(*$2, *$5); del($1, $2, $3, $4, $5, $6);
	}
	| attribute_list "var" var_single_decl_list "[--]"
	{
		$$ = make<syntax::VariablesDeclaration>(*$1, *$3, Handle()); del($1,$2,$3,$4);
	}
	| attribute_list "var" var_single_decl_list "[--]" "[>>]" "[@]" "[--]" "[<<]"
	{
		$$ = make<syntax::VariablesDeclaration>(*$1, *$3, *$6); del($1,$2,$3,$4,$5,$6,$7,$8);
	}
	| "use" use_declaration_list
	{
		$$ = make<syntax::UseDeclaration>(*$2); del($1, $2);
	}
	| "import" /* identifier list separated with dot - specify version? */
	{
	}
	| "import" "local" LITERAL
	{
	}
	| "import" "static" LITERAL
	{
	}
	;

use_declaration_element
	: id_dot_list																{ $$ = make<syntax::UsingNamespaceDeclaration>(*$1); del($1); }
	/*| "[#]" "=" some_expression_rule_is_here */
	;

use_declaration_list : use_declaration_element				{ $$ = list(*$1); del($1); }
	| use_declaration_list "," use_declaration_element	{ $$ = $1; $$->push_back(*$3); del($2,$3); }
	;

id_dot_list : "[#]"														{ $$ = list(*$1); del($1); }
	| id_dot_list "." "[#]"											{ $$ = $1; $$->push_back(*$3); del($2, $3); }
	;

module_list : id_dot_list											{ $$ = list(Handle::make<ImportDecl>(*$1)); del($1); }
	| module_list "," id_dot_list								{ $$ = $1; $$->push_back(Handle::make<ImportDecl>(*$3)); del($2, $3); }
	;

declaration_block : declaration								{ $$ = list(*$1); del($1); }
	| NEWLINE																		{ $$ = list(); del($1); }
	| declaration_block NEWLINE									{ $$ = $1; del($2); }
	| declaration_block declaration							{ $$ = $1; $$->push_back(*$2); del($2); }
	;


var_single_decl
	: "[#]"																			{ $$ = make<syntax::SingleVariableDeclaration>(*$1, Handle(), Handle()); del($1); }
	| "[#]" ":" expr_noass											{ $$ = make<syntax::SingleVariableDeclaration>(*$1, *$3, Handle()); del($1,$2,$3); }
	| "[#]" "=" expr_val												{ $$ = make<syntax::SingleVariableDeclaration>(*$1, Handle(), *$3); del($1,$2,$3); }
	| "[#]" ":" expr_noass "=" expr_val					{ $$ = make<syntax::SingleVariableDeclaration>(*$1, *$3, *$5); del($1,$2,$3,$4,$5); }
	;

var_single_decl_list
	: var_single_decl														{ $$ = list(*$1); del($1); }
	| var_single_decl_list var_single_decl			{ $$ = $1; $$->push_back(*$2); del($2); }
	;

/* ------------------------------------------------------------------------------------------------------ */
/*                                               STATEMENTS                                               */
/* ------------------------------------------------------------------------------------------------------ */

/* instrukcja wraz z tokenem nowej lini */
statement
	: one_line_statement "[--]"
	| if_statement
	;

one_line_statement
	: nonewline_statement
	| one_line_statement ";" nonewline_statement
	;

nonewline_statement
	: expr_all
	| return_statement
	| assert_statement
	| trace_statement
	;

statement_block : statement
	| NEWLINE
	| statement_block NEWLINE
	| statement_block statement
	;

if_statement
	: "if" expr_cond "[--]" "[>>]" statement_block "[<<]" else_statements
	| "if" expr_cond "then" statement else_statements
	;

else_statements
	: "else" "[--]" "[>>]" statement_block "[<<]"
	| "else" statement
	| "elif" expr_cond "[--]" "[>>]" statement_block "[<<]" else_statements
	| "elif" expr_cond "then" statement else_statements
	;

while_statement
	: "while" expr_cond "[--]" "[>>]" statement_block "[<<]"
	| "while" expr_cond "then" statement
	;

return_statement
	: "return"
	| "return" expr_all
	;

assert_statement
	: "assert" expr_cond
	;

trace_statement
	: "trace" expr_list_noempty
	;

%%

// We have to implement the error function
void dragon::Parser::error(const dragon::Parser::location_type &loc, const std::string &msg) {
	std::cerr << "Error [" << (*loc.begin.filename) << ": " << loc.begin.line << ":" << loc.begin.column << "-" << loc.end.column << "]: " << msg << std::endl;
}

// Now that we have the Parser declared, we can declare the Scanner and implement
// the yylex function


#include STRINGIZE(SCANNER_HPP_FILE)
static int yylex(dragon::Parser::semantic_type * zzlval, dragon::Parser::location_type *zzlloc, dragon::Scanner &scanner) {
	return scanner.lex(zzlval, zzlloc);
}
