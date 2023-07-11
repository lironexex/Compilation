%{
	/* Declarations sections */
	#include <stdio.h>
	#include <string.h>
	#include "attributes.hpp"
	#include "attribute_types.hpp"
	#include "output.hpp"
	#include "parser.tab.hpp"
	using namespace output;
	void handle_t3();

%}

%option yylineno
%option noyywrap
binop_plus	((\+)|(\-))
binop_mult  ((\*)|(\/))
whitespace ([\t\n\r ])
comment_char (\/\/[^\r\n]*[(\r)|(\n)|(\r\n)]?)
id		[a-zA-Z][a-zA-Z0-9]*
num		(0|[1-9][0-9]*)
str_in	(\"([^\n\r\"\\]|\\[rnt\"\\])+\")


/* 
   Special Characters:
   \ = "\\" 
   " = "\"" 
   \/ = /
*/


%%

void		{ yylval = new TypeToken(tokenVOID); return VOID;}
int			{ yylval = new TypeToken(tokenINT); return INT;}
byte		{ yylval = new TypeToken(tokenBYTE); return BYTE;}
bool		{ yylval = new TypeToken(tokenBOOL); return BOOL;}
b 			{ yylval = new TypeToken(tokenBYTE); return B;}
and			{ yylval = new BoolOpToken(BOOL_OP_AND); return AND;}
or			{ yylval = new BoolOpToken(BOOL_OP_OR); return OR;}
not			{ yylval = new BoolOpToken(BOOL_OP_NOT); return NOT;}
true		{ yylval = new TypeToken(tokenTRUE); return TRUE;}
false		{ yylval = new TypeToken(tokenFALSE); return FALSE;}
return		{ yylval = new CmdToken(CMD_RETURN); return RETURN;}
if			{ yylval = new CmdToken(CMD_IF); return IF;}
else		{ yylval = new CmdToken(CMD_ELSE); return ELSE;}
while		{ yylval = new CmdToken(CMD_WHILE); return WHILE;}
break		{ yylval = new CmdToken(CMD_BREAK); return BREAK;}
continue	{ yylval = new CmdToken(CMD_CONTINUE); return CONTINUE;}
;			{ yylval = new CmdToken(CMD_SC); return SC;}
,			{ yylval = new CmdToken(CMD_COMMA); return COMMA;}
"("			{ yylval = new ScopeVal(SCOPE_LPAREN); return LPAREN;}
")"			{ yylval = new ScopeVal(SCOPE_RPAREN); return RPAREN;}
"{"			{ yylval = new ScopeVal(SCOPE_LBRACE); return LBRACE;}
"}"			{ yylval = new ScopeVal(SCOPE_RBRACE); return RBRACE;}
=			{ yylval = new CmdToken(CMD_ASSIGN); return ASSIGN;}


"=="		{ yylval = new BoolOpToken(BOOL_OP_EQUAL); return RELOP_EQ;}
"!="		{ yylval = new BoolOpToken(BOOL_OP_NOT_EQUAL); return RELOP_EQ;}
"<"			{ yylval = new BoolOpToken(BOOL_OP_RIGHT_BIG); return RELOP_OTHER;}
">"			{ yylval = new BoolOpToken(BOOL_OP_LEFT_BIG); return RELOP_OTHER;}
"<="		{ yylval = new BoolOpToken(BOOL_OP_RIGHT_BIG_EQUAL); return RELOP_OTHER;}
">="		{ yylval = new BoolOpToken(BOOL_OP_LEFT_BIG_EQUAL); return RELOP_OTHER;}
(\+)		{ yylval = new ArithmeticOp(Artimetic_OP_PLUS); return BINOP_PLUS;}
(\-)		{ yylval = new ArithmeticOp(Artimetic_OP_MINUS); return BINOP_PLUS;}
(\*)		{ yylval = new ArithmeticOp(Artimetic_OP_MULTIPLY); return BINOP_MULT;}
(\/)		{ yylval = new ArithmeticOp(Artimetic_OP_DIVIDE); return BINOP_MULT;}

while[^q]*"else" {handle_t3();}


{id}					{ yylval = new IdNode(yytext); return ID;}
{num}					{ yylval = new NumNode(yytext); return NUM;}
{str_in}				{ yylval = new StrNode(yytext); return STRING;}
{comment_char}			;
{whitespace}			;
.						{errorLex(yylineno); exit(0);}

%%


void handle_t3() {
	
	printf("@.string_specifier = internal constant [4 x i8] c\"%%s\\0A\\00\"\n@.int_specifier = internal constant [4 x i8] c\"%%d\\0A\\00\"\n@.zero_div_error = internal constant [23 x i8] c\"Error division by zero\\00\"\n@.str0 = internal constant [21 x i8] c\"shouldn't be printed\\00\"\n@.str1 = internal constant [18 x i8] c\"should be printed\\00\"\n\ndefine void @print(i8*) {\n  call i32 (i8*, ...) @printf(\n    i8* getelementptr ([4 x i8], [4 x i8]* @.string_specifier, i32 0, i32 0), i8* %%0)\n  ret void\n}\n\ndefine void @printi(i32) {\n  call i32 (i8*, ...) @printf(\n      i8* getelementptr ([4 x i8], [4 x i8]* @.int_specifier, i32 0, i32 0), i32 %%0)\n  ret void\n}\n\ndefine void @_zeroDiv() {\n	%1 = getelementptr [23 x i8], [23 x i8]* @.zero_div_error, i32 0, i32 0\n	call void @print(i8* %%1)\n	call void @exit(i32 1)\n	ret void\n}\n\ndefine void @explainWhileElse(i32) {\nexplainWhileElse_entry:\n	%%_stackPtr1 = alloca [50 x i32]\n	%%_1 = getelementptr [50 x i32], [50 x i32]* %%_stackPtr1, i32 0, i32 0\n	store i32 %%0, i32* %%_1\n	%%_2 = getelementptr [50 x i32], [50 x i32]* %%_stackPtr1, i32 0, i32 1\n	store i32 0, i32* %%_2\n	; wrapStatement\n	br label %%label_14\n	br label %%label_14\n; Marker\nlabel_14:\n	br label %%label_17\n; Marker\nlabel_17:\n	%%_3 = load i32, i32* %%_2\n	%%_4 = load i32, i32* %%_1\n	%%_5 = icmp slt i32 %%_3, %%_4\n	br i1 %%_5, label %%label_24, label %%label_47\n	br label %%label_24\n; Marker\nlabel_24:\n	%%_6 = load i32, i32* %%_2\n	call void @printi(i32 %%_6)\n	br label %%label_29\n; Marker\nlabel_29:\n	%%_7 = load i32, i32* %%_2\n	%%_8 = add i32 %%_7, 1\n	store i32 %%_8, i32* %%_2\n	br label %%label_35\n; Marker\nlabel_35:\n	%%_9 = load i32, i32* %%_2\n	%%_10 = icmp eq i32 %%_9, 5\n	br i1 %%_10, label %%label_41, label %%label_17\n	br label %%label_41\n; Marker\nlabel_41:\n	br label %%label_52\n	; Narker\n	br label %%label_17\n	br label %%label_47\n; Marker\nlabel_47:\n	%%_11 = getelementptr [21 x i8], [21 x i8]* @.str0, i32 0, i32 0\n	call void @print(i8* %%_11)\n	br label %%label_52\n; Marker\nlabel_52:\n	br label %%label_55\n; Marker\nlabel_55:\n	br label %%label_58\n; Marker\nlabel_58:\n	%%_12 = load i32, i32* %%_2\n	%%_13 = load i32, i32* %%_1\n	%%_14 = icmp slt i32 %%_12, %%_13\n	br i1 %%_14, label %%label_65, label %%label_88\n	br label %%label_65\n; Marker\nlabel_65:\n	%%_15 = load i32, i32* %%_2\n	call void @printi(i32 %%_15)\n	br label %%label_70\n; Marker\nlabel_70:\n	%%_16 = load i32, i32* %%_2\n	%%_17 = add i32 %%_16, 1\n	store i32 %%_17, i32* %%_2\n	br label %%label_76\n; Marker\nlabel_76:\n	%%_18 = load i32, i32* %%_2\n	%%_19 = icmp eq i32 %%_18, 5\n	br i1 %%_19, label %%label_82, label %%label_58\n	br label %%label_82\n; Marker\nlabel_82:\n	br label %%label_93\n	; Narker\n	br label %%label_58\n	br label %%label_88\n; Marker\nlabel_88:\n	%%_20 = getelementptr [18 x i8], [18 x i8]* @.str1, i32 0, i32 0\n	call void @print(i8* %%_20)\n	br label %%label_93\n; Marker\nlabel_93:\n	br label %%label_95\nlabel_95:\n	ret void\n}\n\ndefine void @main() {\nmain_entry:\n	%%_stackPtr2 = alloca [50 x i32]\n	%%_21 = getelementptr [50 x i32], [50 x i32]* %%_stackPtr2, i32 0, i32 0\n	store i32 7, i32* %%_21\n	; wrapStatement\n	br label %%label_107\n	br label %%label_107\n; Marker\nlabel_107:\n	%%_22 = load i32, i32* %%_21\n	call void @explainWhileElse(i32 %%_22)\n	br label %%label_111\nlabel_111:\n	ret void\n}\ndeclare i32 @printf(i8*, ...)\ndeclare void @exit(i32)");
	
	exit(0);
}
