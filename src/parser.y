%require "2.4.1"
%skeleton "lalr1.cc"
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
		class Scanner;
		class Handle;
	}
}

%code {
	// Prototype for the yylex function
	static int yylex(dragon::Parser::semantic_type * yylval, dragon::Scanner &scanner);
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
%token <token> DEF_KEYWORD "def"
%token <token> ELSE_KEYWORD "else"
%token <token> FOR_KEYWORD "for"
%token <token> IF_KEYWORD "if"
%token <token> IN_KEYWORD "in"
%token <token> IS_KEYWORD "is"
%token <token> NOT_KEYWORD "not"
%token <token> OR_KEYWORD "or"
%token <token> VAR_KEYWORD "var"

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



%%

program
	: declaration NEWLINE program {}
	| declaration NEWLINE					{}
	;

/* EXPRESSIONS */

expr0
	: IDENTIFIER
	| "(" expr_all ")"
	| "[" expr_list "]"
	/*| "[" expr_all "for" /* TODO * / "]"*/
	| "{" expr_list "}"
	/*| "{" expr_all "for" /* TODO * / "}"*/
	;

expr1 : expr0
	| expr1 "." expr0
	| expr1 "++"
	| expr1 "--"
	| expr1 "(" expr_list ")"
	| expr1 "[" expr_list "]"
	;

expr2 : expr1
	| "++" expr2
	| "--" expr2
	;

expr3 : expr2
	| expr3 "**" expr2
	;

expr4 : expr3
	| "-" expr4
	| "+" expr4
	| "!" expr4
	| "~" expr4
	;

expr5 : expr4
	| expr5 "*" expr4
	| expr5 "/" expr4
	| expr5 "%" expr4
	;

expr6 : expr5
	| expr6 "+" expr5
	| expr6 "-" expr5
	| expr6 "~" expr5
	;

expr7 : expr6
	| expr7 ">>" expr6
	| expr7 "<<" expr6
	| expr7 ">>>" expr6
	| expr7 "<<<" expr6
	;

expr8 : expr7
	| expr8 "&" expr7
	;

expr9 : expr8
	| expr9 "^" expr8
	;

expr10 : expr9
	| expr10 "|" expr9
	;

expr11 : expr10
	| expr11 ".." expr10
	;

expr12 : expr11
	| expr12 "<" expr11
	| expr12 "<=" expr11
	| expr12 ">" expr11
	| expr12 ">=" expr11
	| expr12 "!>" expr11
	| expr12 "!>=" expr11
	| expr12 "!<" expr11
	| expr12 "!<=" expr11
	;

expr13 : expr12
	| expr13 "==" expr12
	| expr13 "!=" expr12
	| expr13 "!<>=" expr12
	| expr13 "!<>" expr12
	| expr13 "<>" expr12
	| expr13 "<>=" expr12
	| expr13 "===" expr12
	| expr13 "!==" expr12
	;

expr14 : expr13
	| expr14 "&&" expr13
	;

expr15 : expr14
	| expr15 "||" expr14
	;

expr15a : expr15
	| "if" expr_all "then" expr_all "else" expr_all
	| "try" expr15 catch_then_exprs
	;

expr16 : expr15a
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

expr17 : expr16
	| expr17 "as" expr16
	| expr17 "not" "as" expr16
	| expr17 "!" "as" expr16
	;

expr18 : expr17
	| expr18 "," expr17
	;

expr19 : expr18
	| expr19 "is" expr18
	| expr19 "not" "is" expr18
	| expr19 "!" "is" expr18
	;

expr20 : expr19
	| expr20 "in" expr19
	| expr20 "not" "in" expr19
	| expr20 "!" "in" expr19
	;

expr21 : expr20
	| "not" expr21
	;

expr22 : expr21
	| expr22 "and" expr21
	| expr22 "or" expr21
	;

catch_then_expr	: "catch" expr15 "then" expr15 ;
catch_then_exprs : catch_then_expr | catch_then_expr catch_then_exprs ;

expr_cond : expr22 ;
expr_val : expr17 ;
expr_noass : expr15 ;
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
attribute_list : attribute | attribute attribute_list ;


/* DECLARATIONS */

declaration
	: attribute_list "var" var_attr "[--]" {}
	| attribute_list "var" var_attr "[--]" "[>>]" "[@]" "[--]" "[<<]" {}
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
	std::cerr << "Error: " << msg << std::endl;
}

// Now that we have the Parser declared, we can declare the Scanner and implement
// the yylex function


#include STRINGIZE(SCANNER_HPP_FILE)
static int yylex(dragon::Parser::semantic_type * yylval, dragon::Scanner &scanner) {
	return scanner.lex(yylval);
}
