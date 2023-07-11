%{

/* Declarations section */
#include <stdio.h>
#include <string.h>

void showToken(char *);
/* void fix_string(); */
void error_handler(char* original_input, char* error);

int charToInt(char ch);
void do_to_dec(char* original_input, int base, char* token_name);
void dec_do_to_dec(char* original_input, int base, char* token_name);
void do_comment(char* original_input, int type);
void do_string(char* original_input, char* token_name);

%}

%option yylineno
%option noyywrap
digit   		([0-9])
octal			([0-7])
letter  		([a-zA-Z])
whitespace		([\t\n\r ])
binary			([0-1])
hex				([0-9a-fA-F])
sign 			([+|-])
ascii			([0-9a-zA-Z])
number          ({digit}+)
word            ({ascii}+)
type            ("Int"|"UInt"|"Double"|"Float"|"Bool"|"String"|"Character")
backdoor		("\\""n"|"\\""r"|"\\""t"|"\\""\\"|"\\""u{")
allchars		(.|\n|\r)
commentopener   ("/*")
commentcloser	("*/")
stringopener	(\")
fullstring		({stringopener}([^(\\\"|\n|\r)]*)(\\\")*([^(\\\"|\n|\r)]*)(\\\")*(\"))

/* 
   Special Characters:
   \ = "\\" 
   " = "\"" 
   \/ = /
*/

/* ================================================================================================ */

/*
currently working: {stringopener}([^(\"|\n|\r)]*)(\")
followedby		("n"|"r"|"t"|"\\"|"\""|"u{")
backdoor2 		("\\"/n|"\\"/"r"|"\\"/"t"|"\\"/"\"|"\\"/"""|"\\"/"u{")
backdoornot		(("\\")/([^{followedby}]))

/*"\""[^{backdoornot}]+"\""				showToken("STRING");		/* TODO: fix this */
/*{followedby}							showToken("followedby");	/* TODO: for debugging */
/*{backdoor2} 							showToken("backdoor2");		/* TODO: for debugging */
/*{backdoornot}							showToken("backdoornot");	/* TODO: for debugging */
/*{backdoor}							showToken("backdoor");		/* TODO: for debugging */
/* "\""{fullstring}*"\""				showToken("STRING");  //fix_string(); TODO: fix this for \u{n} */

/* {stringopener}([^{stringhelper}]*)		error_handler(yytext, "UNCLOSED_STRING"); */

/* ================================================================================================ */

%%
{whitespace}							
"//".*									do_comment(yytext, 1);
{commentopener}([^*]|[*])*("*/")	    do_comment(yytext, 2);
{commentopener}([^*]|[*])*("/*")	    error_handler(yytext, "NESTED_COMMENT");
{commentopener}([^("*/")]*)				error_handler(yytext, "UNCLOSED_COMMENT");
{digit}+          						dec_do_to_dec(yytext, 10, "DEC_INT");
"0b"{binary}+                 			do_to_dec(yytext, 2, "BIN_INT");
"0o"{octal}+                 			do_to_dec(yytext, 8, "OCT_INT");
"0x"{hex}+                 				do_to_dec(yytext, 16, "HEX_INT");
{digit}*"."{digit}*          			showToken("DEC_REAL");
{digit}*"."{digit}*(e|E){sign}{digit}+	showToken("DEC_REAL");
{digit}*"."(e|E){sign}{digit}+			showToken("DEC_REAL");
"0x"{hex}+(p|P){sign}{digit}+ 			showToken("HEX_FP");
\"([^\n\r\"\\]|\\[rntuU>"\\])+\"		do_string(yytext, "STRING");
\"([^\n\r\"\\]|\\[rntuU>"\\])+			error_handler(yytext, "UNCLOSED_STRING");
{type}									showToken("TYPE");
"var"									showToken("VAR");
"let" 									showToken("LET");
"func"									showToken("FUNC");
"import"								showToken("IMPORT");
"nil"									showToken("NIL");
"while"									showToken("WHILE");
"if"                          			showToken("IF");
"else"									showToken("ELSE");
"return"								showToken("RETURN");
";"										showToken("SC");
","										showToken("COMMA");
"("										showToken("LPAREN");
")"										showToken("RPAREN");
"{"										showToken("LBRACE");
"}"										showToken("RBRACE");
"["										showToken("LBRACKET");
"]"										showToken("RBRACKET");
"="										showToken("ASSIGN");
"=="|"!="|"<"|">"|"<="|">="				showToken("RELOP");
"||"|"&&"								showToken("LOGOP");
"+"|"-"|"*"|"/"|"%"						showToken("BINOP");
"true"									showToken("TRUE");
"false"									showToken("FALSE");
"->"									showToken("ARROW");
":"										showToken("COLON");
{letter}+{ascii}*						showToken("ID");
"_"{ascii}+								showToken("ID");

.		error_handler(yytext, "UNKNOWN_CHAR");						/*printf("Lex doesn't know what that is!\n");*/

%%

void showToken(char* name) {
	// printf("Line number is: %d, Token name is: %s, the lexeme is %s, its length is %d\n", yylineno, name, yytext, yyleng);
	printf("%d %s %s\n", yylineno, name, yytext);
}

void error_handler(char* original_input, char* error) {
	if (!strcmp(error, "UNKNOWN_CHAR")) {
		printf("Error %c\n", *original_input);
	} else if (!strcmp(error, "UNCLOSED_STRING")) {
		printf("Error unclosed string\n");
	} else if (!strcmp(error, "UNCLOSED_COMMENT")) {
		printf("Error unclosed comment\n");
	} else if (!strcmp(error, "NESTED_COMMENT")) {
		printf("Warning nested comment\n");
	} else if (!strcmp(error, "BAD_BACKSLASH")) {
		printf("Error undefined escape sequence %c\n", *original_input);
	} else if (!strcmp(error, "BAD_U_BACKSLASH")) {
		printf("Error undefined escape sequence u\n");
	}
	
	exit(0);
}

int charToInt(char ch) {
	if (ch >= '0' && ch <= '9') return (int) ch - '0';
	else if (ch >= 'A' && ch <= 'Z') return (int) ch - 'A' + 10;
	else return (int) ch - 'a' + 10;
}

void do_to_dec(char* original_input, int base, char* token_name) { 
	int len = strlen(original_input); 
    int power = 1;
    int res = 0;
    
	int i; 
    for (i = len - 1; i >= 2; --i) { 
        int currInt = charToInt(original_input[i]);
		
		if (currInt >= base) { 
           printf("ERROR??");
        }
		
        res += currInt * power; 
        power *= base;
    }
	
	printf("%d %s %d\n", yylineno, token_name, res);
}

void dec_do_to_dec(char* original_input, int base, char* token_name) {
	int len = strlen(original_input); 
    int power = 1;
    int res = 0;
    
	int i; 
    for (i = len - 1; i >= 0; --i) { 
        int currInt = charToInt(original_input[i]);
		
		if (currInt >= base) { 
           printf("ERROR??");
        }
		
        res += currInt * power; 
        power *= base;
    }
	
	printf("%d %s %d\n", yylineno, token_name, res);
}

void do_comment(char* original_input, int type) {
	// GO OVER ALL THE CHARS TO FIND NON_PRINTABLES https://web.itu.edu.tr/sgunduz/courses/mikroisl/ascii.html
	int len = strlen(original_input);
	int i;
	int num_lines = 1;
	int comment_opener_counter = 0;
	int comment_closer_counter = 0;
	
	if (type == 2) {
	
		for (int i = 0; i < yyleng - 1; i++) {
			
			if (original_input[i] == '\n') {
				num_lines++;
				i++;
			}
			
			if (original_input[i] == '\r') {
				num_lines++;
				if (original_input[i+1] == '\n') {
					i++;
				}
			}
			
			if (original_input[i] == '/' && original_input[i+1] == '*') {
				comment_opener_counter++;
				i++;
				if (comment_opener_counter > 1) {
					error_handler(original_input, "NESTED_COMMENT");
					return;
				}
			}
			
			if (original_input[i] == '*' && original_input[i+1] == '/') {
				comment_closer_counter++;
				i++;
				
				if (comment_opener_counter == comment_closer_counter) {
					printf("%d COMMENT %d\n", yylineno, num_lines);
					comment_opener_counter = 0;
					comment_closer_counter = 0;
					num_lines = 1;
					return;
				}
			}
			
		}
		
		/*
		if (comment_closer_counter < 1) {
			error_handler(original_input, "UNCLOSED_COMMENT");
			return;
		}
		*/
			
		printf("%d COMMENT %d\n", yylineno, num_lines);
		comment_opener_counter = 0;
		comment_closer_counter = 0;
		num_lines = 1;
		return;
		
	}
	
	if (type == 1) {
		num_lines = 1;
	} 
	
	/*
	else {
		for (i = 2; i < len - 3; ++i) { 
			if (original_input[i] <= 31) {
				error_handler(&original_input[i], "UNKNOWN_CHAR");
			}
			
			if ((i + 1) < (len - 3) && original_input[i] == '\n' && original_input[i + 1] == '\r') {
				++i;
				++num_lines;
			} else if (original_input[i] == '\r' || original_input[i] == '\n') {
				++num_lines;
			} else if ((i + 1) < (len - 3) && original_input[i] == '/' && original_input[i + 1] == '*') {
				error_handler(original_input, "NESTED_COMMENT");
			}
		}
		
	}
	*/
	
	printf("%d COMMENT %d\n", yylineno, num_lines);
	
	if (type == 1) {
		for (i = 2; i < len - 1; ++i) {
			if (original_input[i] <= 31) {
				error_handler(&original_input[i], "UNKNOWN_CHAR");
			}
		}
	}
}


void do_string(char* original_input, char* token_name) {
	
		
	
	for (int i = 0; i < yyleng; i++) {
	
		if (i == yyleng - 1 && yytext[i] != '\"') {	
			error_handler(yytext, "UNCLOSED_STRING");
		}
		
		if (yytext[i] < 32) {
			error_handler(&yytext[i], "UNKNOWN_CHAR");
		}
		
		if (yytext[i] == '\\') {
			
			if (i == yyleng - 1) {
				error_handler("\0", "BAD_BACKSLASH");
				return;
			}
			if (yytext[i+1] != 'n' && yytext[i+1] != 'r' && yytext[i+1] != 't' && yytext[i+1] != '\\' && yytext[i+1] != '\"' && yytext[i+1] != 'u') {
				error_handler(&yytext[i+1], "BAD_BACKSLASH");
				return;
			}
			
			if (yytext[i+1] == 'n') {
				i++;
				continue;
			}
			
			if (yytext[i+1] == 'r') {
				i++;
				continue;
			}
			
			if (yytext[i+1] == 't') {
				i++;
				continue;
			}
			
			if (yytext[i+1] == '\\') { //TODO: fix this
				i++;
				continue;
			}
			
			if (yytext[i+1] == '\"') { //TODO: fix this
				i++;
				continue;
			}
			
			if (yytext[i+1] == 'u') {
									
				if (i+1 == yyleng - 1) {
					error_handler(yytext, "BAD_U_BACKSLASH");
					return;
				}
				if (yytext[i+2] != '{') {
					error_handler(yytext, "BAD_U_BACKSLASH");
					return;
				}
				if (i+2 == yyleng - 1) {
					error_handler(yytext, "BAD_U_BACKSLASH");
					return;
				}
				int j = i + 3;
				int counter = 0;
				if (yytext[j] == '1') {
					error_handler(yytext, "BAD_U_BACKSLASH");
				}
				if (yytext[j] == '7' && j+1 < yyleng) {
					if (yytext[j+1] == 'F' || yytext[j+1] == 'f') {
						error_handler(yytext, "BAD_U_BACKSLASH");
					}
				}
				while (yytext[j] != '}') {
					
					if (yytext[j] == '\0') {
						error_handler(yytext, "BAD_U_BACKSLASH");
					}
					
					if (yytext[j] != '0' && yytext[j] != '1' && yytext[j] != '1' 
					&& yytext[j] != '2' && yytext[j] != '3' && yytext[j] != '4' 
					&& yytext[j] != '5' && yytext[j] != '6' && yytext[j] != '7' 
					&& yytext[j] != '8' && yytext[j] != '9' && yytext[j] != 'A' 
					&& yytext[j] != 'B' && yytext[j] != 'C' && yytext[j] != 'D' 
					&& yytext[j] != 'E' && yytext[j] != 'F' 
					&& yytext[j] != 'a' && yytext[j] != 'b' && yytext[j] != 'c' 
					&& yytext[j] != 'd' && yytext[j] != 'e' && yytext[j] != 'f') {
						error_handler(yytext, "BAD_U_BACKSLASH");
					}
					
					
					counter++;
					j++;
				
				}
				j = i + 3;
				
				if (counter < 2 || counter > 6) {  		// the \u{} has to have between 1 to 6 characters 
					error_handler(yytext, "BAD_U_BACKSLASH");
				}
								
				int power = 1;
				int res = 0;
				int q;
				for (q = counter - 1; q >= 0; --q) { 
									
					int currInt = charToInt(yytext[j+q]);
									
					res += currInt * power; 
					power *= 16;
				}
				
				
				i+= counter + 3;  //TODO: fix this  for the next print.
				continue;
			}
		}
	}
	
	printf("%d %s ", yylineno, token_name);
			
	for (int i = 0; i < yyleng; i++) {
							
		if (yytext[i] < 32) {
			error_handler(&yytext[i], "UNKNOWN_CHAR");
		}
		
		if (yytext[i] == '\\') {
			
			if (i == yyleng - 1) {
				error_handler("\0", "BAD_BACKSLASH");
				return;
			}
			if (yytext[i+1] != 'n' && yytext[i+1] != 'r' && yytext[i+1] != 't' && yytext[i+1] != '\\' && yytext[i+1] != '\"' && yytext[i+1] != 'u') {
				error_handler(&yytext[i+1], "BAD_BACKSLASH");
				return;
			}
			
			if (yytext[i+1] == 'n') {
				printf("\n");
				i++;
				continue;
			}
			
			if (yytext[i+1] == 'r') {
				printf("\r");
				i++;
				/*
				if (i < yyleng-1) {
					if (yytext[i+1] == '\\' && i+2 < yyleng) {
						if (yytext[i+2] == 'n') {
							i+=2;
						}
					}
				}
				*/
				continue;
			}
			
			if (yytext[i+1] == 't') {
				printf("\t");
				i++;
				continue;
			}
			
			if (yytext[i+1] == '\\') { //TODO: fix this
				printf("\\");
				i++;
				continue;
			}
			
			if (yytext[i+1] == '\"') { //TODO: fix this
				printf("\"");
				i++;
				continue;
			}
			
			if (yytext[i+1] == 'u') {
									
				if (i+1 == yyleng - 1) {
					error_handler(yytext, "BAD_U_BACKSLASH");
					return;
				}
				if (yytext[i+2] != '{') {
					error_handler(yytext, "BAD_U_BACKSLASH");
					return;
				}
				if (i+2 == yyleng - 1) {
					error_handler(yytext, "BAD_U_BACKSLASH");
					return;
				}
				int j = i + 3;
				int counter = 0;
				while (yytext[j] != '}') {
					
					if (yytext[j] == '\0') {
						error_handler(yytext, "BAD_U_BACKSLASH");
					}
					
					if (yytext[j] != '0' && yytext[j] != '1' && yytext[j] != '1' 
					&& yytext[j] != '2' && yytext[j] != '3' && yytext[j] != '4' 
					&& yytext[j] != '5' && yytext[j] != '6' && yytext[j] != '7' 
					&& yytext[j] != '8' && yytext[j] != '9' && yytext[j] != 'A' 
					&& yytext[j] != 'B' && yytext[j] != 'C' && yytext[j] != 'D' 
					&& yytext[j] != 'E' && yytext[j] != 'F' 
					&& yytext[j] != 'a' && yytext[j] != 'b' && yytext[j] != 'c' 
					&& yytext[j] != 'd' && yytext[j] != 'e' && yytext[j] != 'f') {
						error_handler(yytext, "BAD_U_BACKSLASH");
					}
					
					
					counter++;
					j++;
				
				}
				j = i + 3;
				
				if (counter < 1 || counter > 6) {  		// the \u{} has to have between 1 to 6 characters 
					error_handler(yytext, "BAD_U_BACKSLASH");
				}
								
				int power = 1;
				int res = 0;
				int q;
				for (q = counter - 1; q >= 0; --q) { 
									
					int currInt = charToInt(yytext[j+q]);
									
					res += currInt * power; 
					power *= 16;
				}
				
				printf("%c",res);  // print the character
				i+= counter + 3;  //TODO: fix this  for the next print.
				continue;
			}
		}
		if (i != 0 && i != yyleng -1) {
			printf ("%c", yytext[i]);
		}
	}
	printf("\n");
	
}