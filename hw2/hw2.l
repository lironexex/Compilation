%{
	#include "attributes.h"
	#include "truthval.tab.hpp"
%}

%option yylineno
%option noyywrap

%%

"void"						return tk_void;
"int"						return tk_int;
"byte"						return tk_byte;
"b"							return tk_b;
"bool"						return tk_bool;
"and"						return tk_and;
"or"						return tk_or;
"true"						{ yylval.constVal = 1; return tk_const; }
"false"						{ yylval.constVal = 0; return tk_const; }
"return"					return tk_return;
"if"						return tk_if;
"else"						return tk_else;
"while"						return tk_while;
"break"						return tk_break;
"continue"					return tk_continue;
";"							return tk_sc;
","							return tk_comma;
"("							return tk_lparen;
")"							return tk_rparen;
"{"							return tk_lbrace;
"}"							return tk_rbrace;
"="							return tk_assign;
"=="|"!="|"<"|">"|"<="|">=" return tk_relop;
"+"|"-"|"*"|"/"				return tk_binop;
[a-zA-Z][a-zA-Z0-9]*		return tk_id;
0|[1-9][0-9]*				return tk_num;
"([^\n\r\"\\]|\\[rnt"\\])+"	return tk_string;
[ \n\t]	;	// Whitespace
//[^\r\n]*[\r|\n|\r\n]?

%%
