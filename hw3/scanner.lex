%{
#include <stdlib.h>
#include <stdio.h>
#include "hw3.h"
#include "parser.tab.hpp"
%}

%option yylineno
%option noyywrap

whitespace [ |\t|\n|\r]


%%

void				return VOID;
int				return INT;
byte				return BYTE;
b				return B;
bool				return BOOL;
and				return AND;
or				return OR;
not				return NOT;
true				return TRUE;
false				return FALSE;
return				return RETURN;
if				return IF;
else				return ELSE;
while				return WHILE;
break				return BREAK;
continue			return CONTINUE;
;				return SC;
,				return COMMA;
\(				return LPAREN;
\)				return RPAREN;
\{				return LBRACE;
\}				return RBRACE;
=				return ASSIGN;
(==|!=)				return RELOPEQUALITY;
(<|>|<=|>=)			return RELOPRELATION;
(\+|-)				return BINOPADD;
(\*|\/)				return BINOPMUL;			
[a-zA-Z][a-zA-Z0-9]*		{yylval=new Id(yytext); return ID;}
(0|[1-9][0-9]*)			{yylval=new Num(yytext); return NUM;}
\"([^\n\r\"\\]|\\[rnt"\\])+\"	return STRING;
\/\/[^\r\n]*[ \r|\n|\r\n]?	;
{whitespace}+			;
<<EOF>>				return EF;
.				{output::errorLex(yylineno);exit(0);}			
%%

void showToken(char* name){
	printf("%d %s %s\n",yylineno, name, yytext);
}
