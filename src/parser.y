%require "2.4.1"
%glr-parser
%skeleton "glr.cc"
%locations
%defines
%define namespace "dragon"
%define parser_class_name "Parser"
%parse-param { dragon::Scanner &scanner }
%lex-param   { dragon::Scanner &scanner }


%code requires {
	#include "config.hpp"

	#define STRINGIZE_NX(A) #A
	#define STRINGIZE(A) STRINGIZE_NX(A)

	#include STRINGIZE(AST_HPP_FILE)

	namespace dragon {

		template<typename T, typename... Args>
		void del(T a, Args... b)
		{
			delete a;
			if(sizeof...(b)) del(b...);
		}
		template<typename T>
		void del(T a)
		{
			delete a;
		}
		template<typename T>
		T* as(Handle *h)
		{
			return dynamic_cast<T*>(&(*(*(*h))));
		}

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


%union {
	dragon::Handle * token;
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
%token <token> NAMESPACE_KEYWORD "namespace"
%token <token> NOT_KEYWORD "not"
%token <token> OR_KEYWORD "or"
%token <token> RAISE_KEYWORD "raise"
%token <token> RETURN_KEYWORD "return"
%token <token> SET_KEYWORD "set"
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

%type <token> program program_decls
%type <token> declaration

%%

program : program_decls													{ $$ = $1; }
	| "[@]" "[--]" program_decls									{ as<Program>($3)->docstring = *$1; $$ = $3; del($1, $2); }
	;

program_decls
	: declaration program													{ auto &dl = as<Program>($2)->declarations; dl.insert(dl.begin(),*$1); $$ = $2; del($1); }
	| declaration																	{ $$ = new Handle(new Program({*$1})); }
	| NEWLINE program															{ $$ = $2; }
	| NEWLINE																			{ $$ = new Handle(new Program()); }
	;

/* EXPRESSIONS */

expr0
	: IDENTIFIER																	{ $$ = $1; }
	| LITERAL																			{ $$ = $1; }
	| "(" ")"
	| "(" expr_all ")"														{ $$ = $2; del($1, $3);}
	| "[" expr_list "]"		/* an array literal */
	/*| "[" expr_all "for" /* TODO * / "]"*/ /* a nice inline generator */
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
	: attribute_list "namespace" id_dot_list "[--]" "[>>]" "[@]" "[--]" declaration_block "[<<]" { $$ = new Handle(); }
	| attribute_list "namespace" id_dot_list "[--]" "[>>]" declaration_block "[<<]" { $$ = new Handle(); }
	| attribute_list "var" var_attr "[--]" { $$ = new Handle(); }
	;

id_dot_list : "[#]"
	| "[#]" "." id_dot_list
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
