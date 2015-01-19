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

%token <token> ELSE_KEYWORD "else"
%token <token> IF_KEYWORD "if"
%token <token> VAR_KEYWORD "var"

%token <token> COLON ":"
%token <token> EQUAL "="
%token <token> LEFT1 "("
%token <token> RIGHT1 ")"
%token <token> SEMICOLON ";"
%token <token> ARROWR "->"


%%

program
	: declaration ";" NEWLINE program {}
	| declaration ";" NEWLINE					{}
	;

declaration
	: "var" var_attr "[--]" {}
	| "var" var_attr "[--]" "[>>]" "[@]" "[--]" "[<<]" {}
	;

expr0
	: IDENTIFIER
	| "(" expr1 ")"
	;

expr1 : expr0
	| expr1 "." expr0
	;

expr9 : expr1
expr10 : expr9
	| expr9 "=" expr10
	| expr1 "->" expr10
	;

expr11 : expr10

var_attr
	: "[#]"
	| "[#]" ":" expr9
	| "[#]" "=" expr10
	| "[#]" ":" expr9 "=" expr10
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
