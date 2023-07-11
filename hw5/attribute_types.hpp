#ifndef ATTRIBUTE_TYPES_H
#define ATTRIBUTE_TYPES_H

enum TerminalTokenEnum {	tokenNONEXIST,
							tokenFUNC,
							tokenVOID, 
							tokenINT, 
							tokenBYTE,  
							tokenBOOL,
							tokenSTRING,
							tokenENUM,
							tokenEnumVal,

							tokenTRUE, 
							tokenFALSE, 
							tokenID
						};

enum BoolOpEnum{ 	BOOL_OP_AND, 
					BOOL_OP_OR, 
					BOOL_OP_NOT, 
					BOOL_OP_EQUAL, 
					BOOL_OP_NOT_EQUAL, 
					BOOL_OP_LEFT_BIG, 
					BOOL_OP_RIGHT_BIG, 
					BOOL_OP_LEFT_BIG_EQUAL,
					BOOL_OP_RIGHT_BIG_EQUAL
				};

enum CmdOpEnum{		CMD_RETURN, 
					CMD_IF, 
					CMD_ELSE, 
					CMD_WHILE, 
					CMD_BREAK, 
					CMD_CONTINUE, 
					CMD_ASSIGN,
					CMD_SC,
					CMD_COMMA
				};

enum ArithmeticOpEnum { 	Artimetic_OP_PLUS,
							Artimetic_OP_MINUS, 
							Artimetic_OP_MULTIPLY, 
							Artimetic_OP_DIVIDE
						};
				
enum ScopeTypeEnum {
					SCOPE_LPAREN,
					SCOPE_RPAREN, 
					SCOPE_LBRACE, 
					SCOPE_RBRACE
				};


#endif