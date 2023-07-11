#ifndef SEMATIC_AID_H
#define SEMATIC_AID_H

#include "attributes.hpp"
#include "SymbolTable.hpp"
#include "output.hpp"
#include "RegAllocator.hpp"
#include "bp.hpp"
#include "MemoryManager.hpp"
#include <stdlib.h>

using namespace std;


extern int yylineno;
extern Node* yylval;

using namespace output;

void checkMainExistance(SymbolTable& symbol_table);
void EnterFunctionScope(SymbolTable& symbol_table, Node* node1, Node* node2, RegAllocator& reg_allocator ,CodeBuffer& code_buffer);
void ExitFunctionScope(SymbolTable& symbol_table, RegAllocator& reg_allocator ,CodeBuffer& code_buffer, MemoryManager& memory_manager);


/* FormalDecl */

/* FormalDecl -> Type ID */
void FormalDecl_To_Type_ID(SymbolTable& symbol_table, Node* node1, Node* node2);

/* RetType */

// RetType -> Type
Node* RetType_To_Type(Node* token);
// RetType -> VOID
Node* RetType_To_VOID(Node* token);


/* Type */

// Type -> INT
Node* Type_To_INT(Node* token);
// Type -> BYTE
Node* Type_To_BYTE(Node* token);
// Type -> BOOL
Node* Type_To_BOOL(Node* token);

/* Statement */

/* Statement -> LBRACE Statements RBRACE */
Node* Statement01(Node* lbrace, Node* Statements, Node* Rbrace);
/* Statement -> Type ID SC */
void Statement02(SymbolTable& symbol_table, Node* Type, Node* ID, Node* SC, CodeBuffer& code_buffer, RegAllocator& reg_allocator, MemoryManager& memory_manager);

/* Statement -> Type ID ASSIGN Exp SC */
void Statement05(SymbolTable& symbol_table, Node* Type, Node* ID, Node* ASSIGN, Node* Exp, Node* SC, CodeBuffer& code_buffer, RegAllocator& reg_allocator, MemoryManager& memory_manager);

/* Statement -> ID ASSIGN Exp SC */
void Statement07(SymbolTable& symbol_table, Node* ID, Node* ASSIGN, Node* Exp, Node* SC,CodeBuffer& code_buffer, RegAllocator& reg_allocator, MemoryManager& memory_manager);
/* Statement -> Call SC */
void Statement08(Node* Call, Node* SC);
/* Statement -> RETURN SC */
void Statement09(SymbolTable& symbol_table, Node* RETURN, Node* SC);
/* Statement -> RETURN Exp SC */
void Statement10(SymbolTable& symbol_table, Node* RETURN, Node* Exp, Node* SC);
/* Statement -> IF LPAREN Exp RPAREN Statement */
void Statement11(Node* IF, Node* Lparen, Node* Exp, Node* Rparen, Node* Statement);
/* Statement -> IF LPAREN Exp RPAREN Statement ELSE Statement */
void Statement12(Node* IF, Node* Lparen, Node* Exp, Node* Rparen, Node* Statement0, Node* ELSE, Node* Statement1);
/* Statement -> WHILE LPAREN Exp RPAREN Statement */
void Statement13(Node* WHILE, Node* Lparen, Node* Exp, Node* Rparen, Node* Statement);
/* Statement -> WHILE LPAREN Exp RPAREN Statement ELSE Statement*/
void Statement16(Node* WHILE, Node* Lparen, Node* Exp, Node* Rparen, Node* Statement0, Node* ELSE, Node* Statement1);
/* Statement -> BREAK SC  */
void Statement14(int& in_while_count);
/* Statement -> CONTINUE SC */
void Statement15(int& in_while_count);

/* Exp */

// Exp -> LPAREN Exp RPAREN

Node* Exp_To_Lparen_Exp_Rparen(Node* lparen, Node* exp_node, Node* rparen);

// Exp -> Exp BINOP_PLUS Exp
// Exp -> Exp BINOP_MULT exp

Node* Exp_To_Exp_Binop_Exp(Node* node1, Node* arith_op, Node* node2, RegAllocator& reg_allocator, CodeBuffer& code_buffer);


// Exp -> ID

Node* Exp_To_ID(SymbolTable& symbol_table, Node* node, RegAllocator& reg_allocator, CodeBuffer& code_buffer);

// Exp -> Call
	
Node* Exp_To_Call(Node* token);

// Exp -> NUM
Node* Exp_To_NUM(Node* token,RegAllocator& reg_allocator, CodeBuffer& code_buffer);

// Exp -> NUM B

Node* Exp_To_NUM_B(Node* node, Node* b_token, RegAllocator& reg_allocator, CodeBuffer& code_buffer);

// Exp -> STRING

Node* Exp_To_STRING(Node* str_token);

// Exp -> TRUE

Node* Exp_To_TRUE(Node* node);

// Exp -> FALSE
Node* Exp_To_FALSE(Node* node);

// Exp -> NOT Exp
Node* Exp_To_NOT_Exp(Node* not_token , Node* exp_token);

// Exp -> Exp AND Exp
Node* Exp_AND_Exp(Node* exp1_token, Node* and_token, Node* exp2_token);

// Exp -> Exp OR Exp
Node* Exp_OR_Exp(Node* exp1_token, Node* and_token, Node* exp2_token);

void AND_OR(Node* exp, Node* op);

// Exp -> Exp RELOP_EQ Exp
// Exp -> Exp RELOP_OTHER Exp
Node* Exp_RELOP_Exp(Node* exp1_token, Node* and_token, Node* exp2_token);


// Exp -> LPAREN Type RPAREN Exp
Node* Exp_To_LPAREN_Type_RPAREN_Exp(Node* type_token, Node* exp_token);



/* Call */

// Call -> ID LPAREN ExpList RPAREN
Node* Call_With_Params(SymbolTable& symbol_table, Node* ID, Node* LPAREN, Node* ExpList, Node* RPAREN);

// Call -> ID LPAREN RPAREN
Node* Call_No_Params(SymbolTable& symbol_table, Node* ID, Node* LPAREN, Node* RPAREN);

/* ExpList */

Node* Deal_If_Bool(Node* Exp);

// ExpList -> Exp
void ExpList_To_Exp(SymbolTable& symbol_table, Node* Exp);

// ExpList -> Exp COMMA ExpList
void ExpList_To_Exp_Comma_Explist(SymbolTable& symbol_table, Node* Exp, Node* Comma, Node* ExpList);


void enterWhile(int& in_while_count, Node* M);

void exitWhile(int& in_while_count, Node* N);

Node* checkBOOLInIf(Node* node);

Node* bp_if(Node* exp);

Node* bp_if_else(Node* exp, Node* M);

#endif