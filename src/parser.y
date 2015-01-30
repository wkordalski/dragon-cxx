%language "c++"
%require "2.4.1"
%glr-parser
/* custom skeleton for non-trivially constructed semantic type support */
/* it is a modification of GLR parser provided by Bison */
%skeleton "./skel.cc"
%locations
%defines
%define api.namespace {dragon}
%define api.value.type {dragon::Handle}
%define parser_class_name {Parser}
%parse-param { dragon::Scanner &scanner }
%lex-param   { dragon::Scanner &scanner }


%code requires {
	#include "config.hpp"

	#define STRINGIZE_NX(A) #A
	#define STRINGIZE(A) STRINGIZE_NX(A)

	#include STRINGIZE(AST_HPP_FILE)

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
}


%token IDENTIFIER "[#]"
%token LITERAL "[@]"
%token NEWLINE "[--]"
%token INDENT "[>>]"
%token DEDENT "[<<]"

%token AND_KEYWORD "and"
%token AS_KEYWORD "as"
%token ASSERT_KEYWORD "assert"
%token BREAK_KEYWORD "break"
%token CLASS_KEYWORD "class"
%token CONTINUE_KEYWORD "continue"
%token CUE_KEYWORD "cue"
%token DEF_KEYWORD "def"
%token ELIF_KEYWORD "elif"
%token ELSE_KEYWORD "else"
%token ENUM_KEYWORD "enum"
%token EXCEPT_KEYWORD "except"
%token FINALLY_KEYWORD "finally"
%token FOR_KEYWORD "for"
%token FROM_KEYWORD "from"
%token GET_KEYWORD "get"
%token IF_KEYWORD "if"
%token IMPORT_KEYWORD "import"
%token IN_KEYWORD "in"
%token INTERFACE_KEYWORD "interface"
%token IS_KEYWORD "is"
%token LET_KEYWORD "let"
%token NAMESPACE_KEYWORD "namespace"
%token NOT_KEYWORD "not"
%token OR_KEYWORD "or"
%token RAISE_KEYWORD "raise"
%token RETURN_KEYWORD "return"
%token SET_KEYWORD "set"
%token THEN_KEYWORD "then"
%token TRACE_KEYWORD "trace"
%token TRY_KEYWORD "try"
%token TYPE_KEYWORD "type"
%token USE_KEYWORD "use"
%token VAR_KEYWORD "var"
%token WHERE_KEYWORD "where"
%token WHILE_KEYWORD "while"
%token YIELD_KEYWORD "yield"

%token AMPERSAND "&"
%token AMPERSAND_EQUAL "&="
%token ARROWL "<-"
%token ARROWR "->"
%token ASTERISK "*"
%token ASTERISK_EQUAL "*="
%token AT "@"
%token CARET "^"
%token CARET_EQUAL "^="
%token COLON ":"
%token COMMA ","
%token DIFFERENT "<>"
%token DIFFERENT_EQUAL "<>="
%token DOT "."
%token DOUBLE_AMPERSAND "&&"
%token DOUBLE_AMPERSAND_EQUAL "&&="
%token DOUBLE_ASTERISK "**"
%token DOUBLE_ASTERISK_EQUAL "**="
%token DOUBLE_DOT ".."
%token DOUBLE_EQUAL "=="
%token DOUBLE_GREATER ">>"
%token DOUBLE_GREATER_EQUAL ">>="
%token DOUBLE_LESSER "<<"
%token DOUBLE_LESSER_EQUAL "<<="
%token DOUBLE_MINUS "--"
%token DOUBLE_PIPE "||"
%token DOUBLE_PIPE_EQUAL "||="
%token DOUBLE_PLUS "++"
%token EQUAL "="
%token EXCLAMATION "!"
%token GREATER ">"
%token GREATER_EQUAL ">="
%token LEFT1 "("
%token LEFT2 "["
%token LEFT3 "{"
%token LESSER "<"
%token LESSER_EQUAL "<="
%token MINUS "-"
%token MINUS_EQUAL "-="
%token NOT_DIFFERENT "!<>"
%token NOT_DIFFERENT_EQUAL "!<>="
%token NOT_DOUBLE_EQUAL "!=="
%token NOT_EQUAL "!="
%token NOT_GREATER "!>"
%token NOT_GREATER_EQUAL "!>="
%token NOT_LESSER "!<"
%token NOT_LESSER_EQUAL "!<="
%token PERCENT "%"
%token PERCENT_EQUAL "%="
%token PIPE "|"
%token PIPE_EQUAL "|="
%token PLUS "+"
%token PLUS_EQUAL "+="
%token RIGHT1 ")"
%token RIGHT2 "]"
%token RIGHT3 "}"
%token SEMICOLON ";"
%token SLASH "/"
%token SLASH_EQUAL "/="
%token TILDE "~"
%token TILDE_EQUAL "~="
%token TRIPPLE_EQUAL "==="
%token TRIPPLE_GREATER ">>>"
%token TRIPPLE_GREATER_EQUAL ">>>="
%token TRIPPLE_LESSER "<<<"
%token TRIPPLE_LESSER_EQUAL "<<<="

%%

program : program_decls													{ $$ = $1; }
	| "[@]" "[--]" program_decls									{ $$ = $3; $$.as<Program>()->docstring = $1; }
	;

program_decls
	: program_decls	declaration 									{ $$ = $1; $$.as<Program>()->declarations.push_back($2); }
	| declaration																	{ $$ = Handle::make<Program>($1); }
	| program_decls NEWLINE 											{ $$ = $1; }
	| NEWLINE																			{ $$ = Handle::make<Program>(); }
	;

/* EXPRESSIONS */

expr0
	: IDENTIFIER																	{ $$ = $1; }
	| LITERAL																			{ $$ = $1; }
	| "(" ")"
	| "(" expr_all ")"														{ $$ = $2; }
	| "[" expr_list "]"		/* an array literal */
	| "[" expr_all "for" "[#]" ":" expr18 "in" expr18 "]" /* a nice inline generator */
	| "{" expr_list "}"
	;

expr1 : expr0																		{ $$ = $1; }
	| expr1 "." expr0
	| expr1 "++"
	| expr1 "--"
	| expr1 "*"  /* pointer type */
	| expr1 "(" expr_list ")"
	| expr1 "[" expr_list "]"
	;

expr2 : expr1																		{ $$ = $1; }
	| "++" expr2
	| "--" expr2
	| "*" expr2
	| "&" expr2
	;

expr3 : expr2																		{ $$ = $1; }
	| expr3 "**" expr2
	;

expr4 : expr3																		{ $$ = $1; }
	| "-" expr4
	| "+" expr4
	| "!" expr4
	| "~" expr4
	;

expr5 : expr4																		{ $$ = $1; }
	| expr5 "*" expr4
	| expr5 "/" expr4
	| expr5 "%" expr4
	;

expr6 : expr5																		{ $$ = $1; }
	| expr6 "+" expr5
	| expr6 "-" expr5
	| expr6 "~" expr5
	;

expr7 : expr6																		{ $$ = $1; }
	| expr7 ">>" expr6
	| expr7 "<<" expr6
	| expr7 ">>>" expr6
	| expr7 "<<<" expr6
	;

expr8 : expr7																		{ $$ = $1; }
	| expr8 "&" expr7
	;

expr9 : expr8																		{ $$ = $1; }
	| expr9 "^" expr8
	;

expr10 : expr9																	{ $$ = $1; }
	| expr10 "|" expr9
	;

expr11 : expr10																	{ $$ = $1; }
	| expr11 ".." expr10
	;

expr12 : expr11																	{ $$ = $1; }
	| expr12 "<" expr11
	| expr12 "<=" expr11
	| expr12 ">" expr11
	| expr12 ">=" expr11
	| expr12 "!>" expr11
	| expr12 "!>=" expr11
	| expr12 "!<" expr11
	| expr12 "!<=" expr11
	;

expr13 : expr12																	{ $$ = $1; }
	| expr13 "==" expr12
	| expr13 "!=" expr12
	| expr13 "!<>=" expr12
	| expr13 "!<>" expr12
	| expr13 "<>" expr12
	| expr13 "<>=" expr12
	| expr13 "===" expr12
	| expr13 "!==" expr12
	| expr13 "<=>" expr12
	;

expr14 : expr13																	{ $$ = $1; }
	| expr14 "&&" expr13
	;

expr15 : expr14																	{ $$ = $1; }
	| expr15 "||" expr14
	;

expr15a : expr15																{ $$ = $1; }
	| "if" expr_all "then" expr_all elif_then_elses
	| "try" expr_all except_then_exprs
	;

expr16 : expr15a																{ $$ = $1; }
	/* Expr15 is intentional here below */
	| expr15 "=" expr16
	| expr15 "+=" expr16
	| expr15 "-=" expr16
	| expr15 "*=" expr16
	| expr15 "/=" expr16
	| expr15 "%=" expr16
	| expr15 "&=" expr16
	| expr15 "|=" expr16
	| expr15 "^=" expr16
	| expr15 "~=" expr16
	| expr15 "<<=" expr16
	| expr15 ">>=" expr16
	| expr15 "<<<=" expr16
	| expr15 ">>>=" expr16
	| expr15 "&&=" expr16
	| expr15 "||=" expr16
	| expr15 "**=" expr16
	| expr15 "<-" expr16
	| lambda_head "->" expr16
	;

expr17 : expr16																	{ $$ = $1; }
	| expr17 "as" expr16
	| expr17 "not" "as" expr16
	| expr17 "!" "as" expr16
	;

expr18 : expr17																	{ $$ = $1; }
	| expr18 "," expr17
	;

expr19 : expr18																	{ $$ = $1; }
	| expr19 "is" expr18
	| expr19 "not" "is" expr18
	| expr19 "!" "is" expr18
	;

expr20 : expr19																	{ $$ = $1; }
	| expr20 "in" expr19
	| expr20 "not" "in" expr19
	| expr20 "!" "in" expr19
	;

expr21 : expr20																	{ $$ = $1; }
	| "not" expr21
	;

expr22 : expr21																	{ $$ = $1; }
	| expr22 "and" expr21
	| expr22 "or" expr21
	;

except_then_expr	: "except" expr_all "then" expr_all ;
except_then_exprs : except_then_expr | except_then_expr except_then_exprs ;
elif_then_elses
	: "else" expr_all
	| "elif" expr_all "then" expr_all elif_then_elses
	;

expr_cond : expr22															{ $$ = $1; };
expr_val : expr17 															{ $$ = $1; };
expr_noass : expr15 														{ $$ = $1; };
expr_list_noempty : expr_val
	| expr_val "," expr_list
	;

expr_list : expr_list_noempty | /* EMPTY */ ;

expr_all : expr22 ;

/* LAMBDA */

lambda_head : "[#]"
	/* TODO */
	;

/* ATTRIBUTES */

attribute : "@" expr_list "[--]" ;
attribute_list_noempty : attribute | attribute attribute_list_noempty ;
attribute_list : attribute_list_noempty | /* EMPTY */ ;

/* DECLARATIONS */

declaration
	: attribute_list "namespace" id_dot_list "[--]" "[>>]" "[@]" "[--]" declaration_block "[<<]"
	{
		$$ = Handle(); /* TODO */
	}
	| attribute_list "namespace" id_dot_list "[--]" "[>>]" declaration_block "[<<]"
	{
		$$ = Handle(); /* TODO */
	}
	| attribute_list "var" var_attr "[--]"
	{
		$$ = Handle(); /* TODO */
	}
	;

id_dot_list : "[#]"
	| id_dot_list "." "[#]"
	;

declaration_block : declaration
	| declaration declaration_block
	;


var_attr
	: "[#]"
	| "[#]" ":" expr_noass
	| "[#]" "=" expr_val
	| "[#]" ":" expr_noass "=" expr_val
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
