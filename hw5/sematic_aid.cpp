#include "sematic_aid.hpp"
#include "SymbolTable.hpp"
#include "output.hpp"
#include "MemoryManager.hpp"
#include <stdlib.h>

using namespace std;

using namespace output;

extern char* yytext;


int TypeToken::next_reg = 0;
int Scope::ptr_generate = 0;

void checkMainExistance(SymbolTable& symbol_table){
	if(!symbol_table.isMainLocated()){
		output:errorMainMissing();
		exit(0);
	}
}

void ExitFunctionScope(SymbolTable& symbol_table, RegAllocator& reg_allocator ,CodeBuffer& code_buffer, MemoryManager& memory_manager){

	memory_manager.freeMemory(reg_allocator);


    // if(!symbol_table.getCurrentScope().getDoesRetExists()){
    CodeBuffer& cd = CodeBuffer::instance();
    cd.emit("\tret i32 0");
    cd.emit("}");
    // }


	// //temp



	symbol_table.DestroyCurrentScope();
}

/* FuncDecl -> RetType ID ..... */

void EnterFunctionScope(SymbolTable& symbol_table, Node* node1, Node* node2, RegAllocator& reg_allocator ,CodeBuffer& code_buffer){
	TypeToken* ret_node = (TypeToken*)node1;
	IdNode* Id_node = (IdNode*)node2;

    Symbol* symbol = symbol_table.getSymbolByName(Id_node->getName());

    if(symbol != NULL){
    	errorDef(yylineno,Id_node->getName());
        exit(0);
    }

    //TODO DANIEL: deal with more than 0 parameters in a main function



    if(Id_node->getName() == "main" && ret_node->getType() == tokenVOID && symbol_table.isFunctionInputEmpty()){
        symbol_table.markMainExist();
    }

    vector<dataObj>& input_func = symbol_table.getFunctionInput();

    string input_str_print = "(";

    for(int i = 0; i < input_func.size(); i++){
        input_str_print = input_str_print + "i32";
        if(i + 1 != input_func.size()){
            input_str_print = input_str_print + ", ";
        }
    }

    input_str_print = input_str_print + ")";


    //TODO DANIEL: need to get proper input!

    string func_name = Id_node->getName();

    code_buffer.emit("define i32 @"+func_name+input_str_print+" {");
    code_buffer.emit("\t%stack = alloca [50 x i32]");

	symbol_table.NewFuncScope(Id_node->getName(), ret_node->getType());


}




/* FormalDecl */

/* FormalDecl -> Type ID */
void FormalDecl_To_Type_ID(SymbolTable& symbol_table, Node* node1, Node* node2){
	TypeToken* type_node = (TypeToken*)node1;
	IdNode* id_node = (IdNode*)node2;
	//TODO DANIEL: check shadowing + if name is already defined somewhere.

	Symbol* symbol = symbol_table.getSymbolByName(id_node->getName());
	if(symbol != NULL){
		errorDef(yylineno, id_node->getName());
		exit(0);
	}

	symbol_table.appendFunctionInput(type_node->getType(), id_node->getName());
}

/* RetType */

// RetType -> Type
Node* RetType_To_Type(Node* token){return token;}
// RetType -> VOID
Node* RetType_To_VOID(Node* token){return token;}


/* Type */

// Type -> INT
Node* Type_To_INT(Node* token){return token;}
// Type -> BYTE
Node* Type_To_BYTE(Node* token){return token;}
// Type -> BOOL
Node* Type_To_BOOL(Node* token){return token;}


/* Statement */

/* Statement -> LBRACE Statements RBRACE */
Node* Statement01(Node* lbrace, Node* Statements, Node* Rbrace){

}
/* Statement -> Type ID SC */
void Statement02(SymbolTable& symbol_table, Node* Type, Node* ID, Node* SC, CodeBuffer& code_buffer, RegAllocator& reg_allocator, MemoryManager& memory_manager){
	string id_name = ((IdNode*)ID)->getName();
	Symbol* symbol = symbol_table.getSymbolByName(id_name);
	if(symbol){
		errorDef(yylineno, id_name);
		exit(0);
	}
	TerminalTokenEnum type = (((TypeToken*)Type)->getType());
	symbol_table.addVar(id_name, type);
 	
 	symbol = symbol_table.getSymbolByName(id_name);
}

/* Statement -> Type ID ASSIGN Exp SC */
void Statement05(SymbolTable& symbol_table, Node* Type, Node* ID, Node* ASSIGN, Node* Exp, Node* SC, CodeBuffer& code_buffer, RegAllocator& reg_allocator, MemoryManager& memory_manager){
    auto exp_token = (TypeToken*)Exp;
    auto type_token = (TypeToken*)Type;

    string id_name = ((IdNode*)ID)->getName();
	Symbol* symbol = symbol_table.getSymbolByName(id_name);
	if(symbol){
		errorDef(yylineno, id_name);
		exit(0);
	}
	TerminalTokenEnum type_ID = type_token->getType();
	TerminalTokenEnum type_Exp = exp_token->getType();

	if(type_ID != type_Exp && !(type_ID == tokenINT && type_Exp == tokenBYTE)){
		errorMismatch(yylineno);
		exit(0);
	}

	/* hw5 store symbol into a memory slot */

    string value;
    if(type_Exp == tokenBOOL){
        //DANIEL ASDF 01
        value = exp_token->phiAndZext();
    } else {
        value = exp_token->regToString();
    }
    symbol_table.addVar(id_name, type_ID);
    symbol = symbol_table.getSymbolByName(id_name);
    symbol->store(value);

}


/* Statement -> ID ASSIGN Exp SC */
void Statement07(SymbolTable& symbol_table, Node* ID, Node* ASSIGN, Node* Exp, Node* SC,CodeBuffer& code_buffer, RegAllocator& reg_allocator, MemoryManager& memory_manager){
    auto exp_token = (TypeToken*)Exp;
    string id_name = ((IdNode*)ID)->getName();
	Symbol* symbol = symbol_table.getSymbolByName(id_name);

    TerminalTokenEnum type_ID = symbol->getType();
    TerminalTokenEnum type_Exp = exp_token->getType();

	if(symbol == NULL || symbol->getType() == tokenFUNC){
		errorUndef(yylineno, id_name);
		exit(0);
	}

	if(type_ID == tokenEnumVal && type_Exp == tokenEnumVal){
	    string enum1 = ((VariableSymbol*)symbol)->getEnumType();
        string temp = ((EnumValNode*)Exp)->getEnumValName();
        string enum2 = symbol_table.getEnumType(temp)->getName();
        if(enum1 != enum2){
            errorUndefEnumValue(yylineno, id_name);
            exit(0);
        }

        symbol->enumInnerParam = temp;
	}

	if(type_ID != type_Exp && !(type_ID == tokenINT && type_Exp == tokenBYTE)){
		errorMismatch(yylineno);
		exit(0);
	}

	/* hw5 store symbol into a memory slot */

    string value;
    if(type_Exp == tokenBOOL){
        value = exp_token->phiAndZext();
    } else {
        value = exp_token->regToString();
    }

    symbol = symbol_table.getSymbolByName(id_name);
    symbol->store(value);
}
/* Statement -> Call SC */
void Statement08(Node* Call, Node* SC){
    auto exp_token = (TypeToken*)Call;
    TerminalTokenEnum type_Exp = exp_token->getType();
    string value;
    if(type_Exp == tokenBOOL){
        value = exp_token->phiAndZext();
    } else {
        value = exp_token->regToString();
    }
}
/* Statement -> RETURN SC */
void Statement09(SymbolTable& symbol_table, Node* RETURN, Node* SC){
	TerminalTokenEnum ret_type = symbol_table.getCurrentScopeRetType();

	if(ret_type != tokenVOID){
		errorMismatch(yylineno);
		exit(0);
	}
    
    CodeBuffer& cd = CodeBuffer::instance();

    //temp
    cd.emit("\tret i32 0");
    // cd.emit("}");

    symbol_table.getCurrentScope().setRetExists();
}
/* Statement -> RETURN Exp SC */
void Statement10(SymbolTable& symbol_table, Node* RETURN, Node* Exp, Node* SC){
	TerminalTokenEnum ret_type = symbol_table.getCurrentScopeRetType();
	TerminalTokenEnum type_Exp = (((TypeToken*)Exp)->getType());

	if((ret_type != type_Exp) && !(ret_type == tokenINT && type_Exp == tokenBYTE)){
		errorMismatch(yylineno);
		exit(0);	
	}

        CodeBuffer& cd = CodeBuffer::instance();

    string regString = ((TypeToken*)Exp)->regToString();

    if(ret_type == tokenBOOL){
        regString = ((TypeToken*)Exp)->phiAndZext();
    }

    //temp
    cd.emit("\tret i32 "+regString);
    // cd.emit("}");

    symbol_table.getCurrentScope().setRetExists();


}
/* Statement -> IF LPAREN Exp RPAREN Statement */
void Statement11(Node* IF, Node* Lparen, Node* Exp, Node* Rparen, Node* Statement){

}
/* Statement -> IF LPAREN Exp RPAREN Statement ELSE Statement */
void Statement12(Node* IF, Node* Lparen, Node* Exp, Node* Rparen, Node* Statement0, Node* ELSE, Node* Statement1){

}
/* Statement -> WHILE LPAREN Exp RPAREN Statement */
void Statement13(Node* WHILE, Node* Lparen, Node* Exp, Node* Rparen, Node* Statement){

}

/* Statement -> WHILE LPAREN Exp RPAREN Statement ELSE Statement*/
void Statement16(Node* WHILE, Node* Lparen, Node* Exp, Node* Rparen, Node* Statement0, Node* ELSE, Node* Statement1) {
	
}


/* Statement -> BREAK SC  */
void Statement14(int& in_while_count){
    CodeBuffer& cb = CodeBuffer::instance();
	if(in_while_count <= 0){
		errorUnexpectedBreak(yylineno);
		exit(0);
	}
	int loc = cb.emitCommand("\tbr label @", "break from while #" + to_string(in_while_count));
	cb.break_list[in_while_count] = cb.merge(cb.makelist({loc, FIRST}), cb.break_list[in_while_count]);
}
/* Statement -> CONTINUE SC */
void Statement15(int& in_while_count){
    CodeBuffer& cb = CodeBuffer::instance();
	if(in_while_count <= 0){
		errorUnexpectedContinue(yylineno);
		exit(0);
	}
    int loc = cb.emitCommand("\tbr label @", "continue from while #" + to_string(in_while_count));
    cb.continue_list[in_while_count] = cb.merge(cb.makelist({loc, FIRST}), cb.continue_list[in_while_count]);
}


/* Exp */

// Exp -> LPAREN Exp RPAREN

Node* Exp_To_Lparen_Exp_Rparen(Node* lparen, Node* exp_node, Node* rparen){

	TypeToken* exp = (TypeToken*)exp_node;

	TerminalTokenEnum type = exp->getType();

	if(type != tokenBOOL){
		output::errorMismatch(yylineno);
		exit(0);
	}
	return new TypeToken(tokenBOOL);
}

// Exp -> Exp BINOP_PLUS Exp
// Exp -> Exp BINOP_MULT exp

Node* Exp_To_Exp_Binop_Exp(Node* node1, Node* arith_op, Node* node2, RegAllocator& reg_allocator, CodeBuffer& code_buffer){
    CodeBuffer& cb = CodeBuffer::instance();
	// if(dynamic_cast<TypeToken*>(exp1_token) == NULL || dynamic_cast<TypeToken*>(exp2_token) == NULL){


	TypeToken* exp1_token = (TypeToken*) node1;
	TypeToken* exp2_token = (TypeToken*) node2;

	TerminalTokenEnum exp1_type = exp1_token->getType();
	TerminalTokenEnum exp2_type = exp2_token->getType();

	if(((exp1_type != tokenINT && exp1_type != tokenBYTE) || (exp2_type != tokenINT && exp2_type != tokenBYTE))){
		errorMismatch(yylineno);
		exit(0);
	}


    string op = ((ArithmeticOp*)arith_op)->opToEmitStr();
    // check div by 0
    if(op == "sdiv"){
        TypeToken temp(tokenBOOL);
        temp.genReg(); // needed for new register
        cb.emitCommand("\t" + temp.regToString() + " = icmp eq i32 " +
                       exp2_token->regToString() + ", 0",
                       "check if 2nd 'div' argument is 0.");
        int loc = cb.emit("\tbr i1 " + temp.regToString() + ", label @, label @");
        string true_label = cb.genLabel();
        cb.bpatch(cb.makelist({loc, FIRST}), true_label);

        TypeToken* dummyType = new TypeToken(tokenNONEXIST);

        dummyType->genReg();

        cb.emitCommand("\t"+dummyType->regToString()+" = getelementptr [23 x i8], [23 x i8]* @.printerror, i32 0, i32 0","save the pointer to the error message");
        cb.emitCommand("\tcall i32 @print(i8* "+dummyType->regToString()+")","print the error messages");
        cb.emitCommand("\tcall void @exit(i32 0)","; call exit(0)");
        cb.emitCommand("\tbr label %"+true_label,"Needed for syntax, will probably never reach here.");

        string false_label = cb.genLabel();
        cb.bpatch(cb.makelist({loc, SECOND}), false_label);
    }

    auto ret = new TypeToken(tokenINT);
    ret->genReg();
    string str = ret->regToString() + " = " + op + " i32 " + exp1_token->regToString() +
                 ", " + exp2_token->regToString();
    code_buffer.emit("\t" + str);
    if(exp1_type == tokenBYTE && exp2_type == tokenBYTE){
        string used_reg_string = ret->regToString();
        ret->genReg();
        code_buffer.emit("\t" + ret->regToString() + " = trunc i32 " + used_reg_string +
                         " to i8");
        used_reg_string = ret->regToString();
        ret->genReg();
        code_buffer.emit("\t" + ret->regToString() + " = zext i8 " + used_reg_string +
                         " to i32");

        ret->setNewType(tokenBYTE);
    }
    return ret;
}


// Exp -> ID

Node* Exp_To_ID(SymbolTable& symbol_table, Node* node, RegAllocator& reg_allocator, CodeBuffer& code_buffer){
    CodeBuffer& cb = CodeBuffer::instance();
	IdNode* token = (IdNode*)node;

	Symbol* symbol_ptr = symbol_table.getEnumType((token->getName()));
	if(symbol_ptr != nullptr){
        EnumValNode* ret = new EnumValNode(token->getName());
        ret->genReg();
        vector<string>& check_vector = ((EnumSymbol*)symbol_ptr)->getEnumVector();
        int value;
        for(int i = 0; i < check_vector.size(); i++){
            if(check_vector[i] == token->getName()){
                value = i;
                break;
            }
        }
        cb.emitCommand("\t" + ret->regToString() + " = " + "add i32 " + to_string(value) + ", 0", ret->regToString() + " = " + to_string(value));
        return ret;
	}

    symbol_ptr = symbol_table.getSymbolByName(token->getName());
	if(symbol_ptr == nullptr){
		errorUndef(yylineno, token->getName());
		exit(0);
	}

	if(symbol_ptr->getType() == tokenFUNC){
		output::errorUndef(yylineno, symbol_ptr->getName()); //can't be function here
	}

    if(symbol_ptr->getType() == tokenEnumVal){

        //DANIEL ASDF ENUM
        // auto enum_ret = new EnumValNode(token->getName());
        auto enum_ret = new EnumValNode(symbol_ptr->enumInnerParam);
        enum_ret->genReg();
        symbol_ptr->load(enum_ret->regToString());


        return enum_ret;
    }

    //TODO: make ret work for bool and enum (should work for var) ((and maybe even bool))
    auto ret = new TypeToken(symbol_ptr->getType());
    ret->genReg();
    symbol_ptr->load(ret->regToString());

    if(ret->getType() == tokenBOOL){
        string old_reg = ret->regToString();
        ret->genReg();
        cb.emitCommand("\t" + ret->regToString() + " = trunc i32 " + old_reg + " to i1",
                "cast bool to i32");
        int loc = cb.emitCommand("\tbr i1 " + ret->regToString() + ", label @, label @",
                                 symbol_ptr->getName());
        ret->true_list = cb.makelist({loc, FIRST});
        ret->false_list = cb.makelist({loc, SECOND});
    }

	return ret;
}

// Exp -> Call
	
Node* Exp_To_Call(Node* token){
	return token;
}

// Exp -> NUM

Node* Exp_To_NUM(Node* token,RegAllocator& reg_allocator, CodeBuffer& code_buffer){
	NumNode* num_node = (NumNode*)token;
    auto ret = new TypeToken(tokenINT);

    ret->genReg();
    code_buffer.emitCommand("\t" + ret->regToString() + " = " + "add i32 " + num_node->getStrValue() + ", 0",
            ret->regToString() + " = " + num_node->getStrValue());

	return ret;
}

// Exp -> NUM B

Node* Exp_To_NUM_B(Node* node, Node* b_token, RegAllocator& reg_allocator, CodeBuffer& code_buffer){

	NumNode* num_token = (NumNode*)node;

    if(!(num_token->isByteValid())){
        errorByteTooLarge(yylineno, num_token->getStrValue());
        exit(0);
    }
    auto ret = new TypeToken(tokenBYTE);
    ret->genReg();
    code_buffer.emit("\t" + ret->regToString() + " = " + "add i32 " + num_token->getStrValue() + ", 0");

    return ret;
}

// Exp -> STRING

Node* Exp_To_STRING(Node* str_token){
	string text = yytext; //convert to string
	text.erase(0,1);
	text.erase(text.size() - 1);

	TypeToken* ret = new TypeToken(tokenSTRING);

	ret->genReg();
	int reg = ret->getReg();

	CodeBuffer& cb = CodeBuffer::instance();

	cb.emitGlobal("@.str"+to_string(reg)+" = constant ["+to_string(text.size() + 1)+" x i8] c\""+text+"\\00\"");

	cb.emit("\t"+ret->regToString()+" = getelementptr ["+to_string(text.size() + 1)+" x i8], ["+to_string(text.size() + 1)+" x i8]* @.str"+to_string(reg)+", i32 0, i32 0");


	return ret;
}

// Exp -> TRUE

Node* Exp_To_TRUE(Node* node){
    CodeBuffer& cb = CodeBuffer::instance();
	TypeToken* true_token = (TypeToken*)node;

	true_token->setNewType(tokenBOOL);
    int loc = cb.emit("\tbr label @");
	true_token->true_list = cb.makelist({loc, FIRST});
	return true_token;
}

// Exp -> FALSE
Node* Exp_To_FALSE(Node* node){
    CodeBuffer& cb = CodeBuffer::instance();
	TypeToken* false_token = (TypeToken*)node;

	false_token->setNewType(tokenBOOL);
    int loc = cb.emit("\tbr label @");
    false_token->false_list = cb.makelist({loc, FIRST});
	return false_token;
}

// Exp -> NOT Exp
Node* Exp_To_NOT_Exp(Node* not_token , Node* exp_token){
	if(dynamic_cast<TypeToken*>(exp_token) == NULL){
		//TODO: Check if it works as intented.
		errorMismatch(yylineno);
        exit(0);
	}
	TypeToken* cur_exp_token = (TypeToken*)exp_token;

	if(!cur_exp_token->isBool()){
            errorMismatch(yylineno);
            exit(0);
	}
    swap(cur_exp_token->false_list, cur_exp_token->true_list);
	return exp_token;
}

// Exp -> Exp AND Exp
Node* Exp_AND_Exp(Node* exp1_token, Node* and_token, Node* exp2_token){
    CodeBuffer& cb = CodeBuffer::instance();
    if(dynamic_cast<TypeToken*>(exp1_token) == NULL || dynamic_cast<TypeToken*>(exp2_token) == NULL){
		//TODO: Check if it works as intented.
		errorMismatch(yylineno);
        exit(0);
	}


	TypeToken* cur_exp1_token = (TypeToken*)exp1_token;
	TypeToken* cur_exp2_token = (TypeToken*)exp2_token;
    auto op = (BoolOpToken*)and_token;

	if(!cur_exp1_token->isBool() || !cur_exp2_token->isBool()){
            errorMismatch(yylineno);
            exit(0);
	}
//    int loc = cb.emit("\tbr i1 " + cur_exp1_token->regToString() + ", label @, label @");

//    cur_exp1_token->true_list = cb.merge(cur_exp1_token->true_list, cb.makelist({op->br_label, FIRST}));
    cb.bpatch(cur_exp1_token->true_list, op->nextInstr);
    cur_exp1_token->true_list = cur_exp2_token->true_list;
    cur_exp1_token->false_list = cb.merge(cur_exp1_token->false_list, cur_exp2_token->false_list);
//    cur_exp1_token->false_list = cb.merge(cur_exp1_token->false_list, cb.makelist({op->br_label, SECOND}));

	return exp1_token;
}

// Exp -> Exp OR Exp
Node* Exp_OR_Exp(Node* exp1_token, Node* and_token, Node* exp2_token){
    CodeBuffer& cb = CodeBuffer::instance();
    if(dynamic_cast<TypeToken*>(exp1_token) == NULL || dynamic_cast<TypeToken*>(exp2_token) == NULL){
		//TODO: Check if it works as intented.
		errorMismatch(yylineno);
        exit(0);
	}


	TypeToken* cur_exp1_token = (TypeToken*)exp1_token;
	TypeToken* cur_exp2_token = (TypeToken*)exp2_token;
    auto op = (BoolOpToken*)and_token;

	if(!cur_exp1_token->isBool() || !cur_exp2_token->isBool()){
            errorMismatch(yylineno);
            exit(0);
	}
//    cur_exp1_token->false_list = cb.merge(cur_exp1_token->false_list, cb.makelist({op->br_label, SECOND}));
    cb.bpatch(cur_exp1_token->false_list, op->nextInstr);
    cur_exp1_token->false_list = cur_exp2_token->false_list;
    cur_exp1_token->true_list = cb.merge(cur_exp1_token->true_list, cur_exp2_token->true_list);
//    cur_exp1_token->true_list = cb.merge(cur_exp1_token->true_list, cb.makelist({op->br_label, FIRST}));

    return exp1_token;
}

void AND_OR(Node* exp_token, Node* op_token){
    CodeBuffer& cb = CodeBuffer::instance();
    auto exp = (TypeToken*)exp_token;
    auto op = (BoolOpToken*)op_token;
//    op->br_label = cb.emitCommand("\tbr i1 " + exp->regToString() + ", label @, label @", "AND_OR");
    op->nextInstr = cb.genLabel();
}


// Exp -> Exp RELOP_EQ Exp
// Exp -> Exp RELOP_OTHER Exp
Node* Exp_RELOP_Exp(Node* exp1_token, Node* and_token, Node* exp2_token){
    CodeBuffer& cb = CodeBuffer::instance();
    auto op = (BoolOpToken*)and_token;
    if(dynamic_cast<TypeToken*>(exp1_token) == NULL || dynamic_cast<TypeToken*>(exp2_token) == NULL){
		//TODO: Check if it works as intented.
		errorMismatch(yylineno);
        exit(0);
	}


	TypeToken* cur_exp1_token = (TypeToken*)exp1_token;
	TypeToken* cur_exp2_token = (TypeToken*)exp2_token;

	if(!(cur_exp1_token->getType() == tokenINT || cur_exp1_token->getType() == tokenBYTE) ||
		!(cur_exp2_token->getType() == tokenINT || cur_exp2_token->getType() == tokenBYTE)){
	// if(!cur_exp1_token->isBool() || !cur_exp2_token->isBool()){
            errorMismatch(yylineno);
            exit(0);
	}


	//DELETE TODO exp2 
    auto ret = new TypeToken(tokenBOOL);
    ret->genReg();
    cb.emitCommand("\t" + ret->regToString() + " = icmp " + op->opToEmitStr() +" i32 " +
        cur_exp1_token->regToString() + ", " + cur_exp2_token->regToString(), "");
    int loc = cb.emitCommand("\tbr i1 " + ret->regToString() + ", label @, label @",
            cur_exp1_token->regToString() + op->opSign() + cur_exp2_token->regToString());
    ret->true_list = cb.makelist({loc, FIRST});
    ret->false_list = cb.makelist({loc, SECOND});
	return ret;
}


// Exp -> LPAREN Type RPAREN Exp
Node* Exp_To_LPAREN_Type_RPAREN_Exp(Node* type_token, Node* exp_token){
    auto type = (TypeToken*)type_token;
    auto exp = (TypeToken*)exp_token;
    if(type->getType() != tokenINT || exp->getType() != tokenEnumVal){
        errorMismatch(yylineno); //TODO: check if correct error
        exit(0);
    }

    //DELETE type_token
    exp->setNewType(tokenINT);
    return exp;
}


/* Call */

// Call -> ID LPAREN ExpList RPAREN
Node* Call_With_Params(SymbolTable& symbol_table, Node* ID, Node* LPAREN, Node* ExpList, Node* RPAREN){
	Node* ret_token = Call_No_Params(symbol_table, ID, LPAREN, RPAREN);

	symbol_table.emptyFunctionInput();

	return ret_token;
}

// Call -> ID LPAREN RPAREN
Node* Call_No_Params(SymbolTable& symbol_table, Node* ID, Node* LPAREN, Node* RPAREN){
	string func_name = ((IdNode*)ID)->getName();
	Symbol* symbol = symbol_table.getSymbolByName(func_name);

	if(symbol == NULL || symbol->getType() != tokenFUNC){
		errorUndefFunc(yylineno, func_name);
		exit(0);
	}
	vector<string> symbol_strfunc_params = ((FunctionSymbol*)symbol)->getStrFunctionInput();
	vector<string> input_str_params = symbol_table.getStrFunctionInput();
	vector<string> tmp_revert = vector<string>();

	for(vector<string>::reverse_iterator it = input_str_params.rbegin(); it != input_str_params.rend(); it++){
		tmp_revert.push_back(*it);
	}



	if(symbol_strfunc_params.size() != tmp_revert.size()){
		errorPrototypeMismatch(yylineno, func_name, symbol_strfunc_params);
		exit(0);		
	}



	for(int i = 0; i < tmp_revert.size() ; i++){
		if((symbol_strfunc_params[i] != tmp_revert[i]) && !(symbol_strfunc_params[i] == "INT" && tmp_revert[i] == "BYTE")){
			errorPrototypeMismatch(yylineno, func_name, symbol_strfunc_params);
			exit(0);		
		}
	}

	TypeToken* ret = new TypeToken(((FunctionSymbol*)symbol)->getRetType());


	CodeBuffer& cb = CodeBuffer::instance();

	string func_params_str = "(";
	vector<dataObj>& functionInputData = symbol_table.getFunctionInput();
	for(int i = functionInputData.size()-1; i >= 0; i--){
		// Notice that the functionInputData[i].getName() actually saves the register name in that case as it was returning "NONE" before.
		if(functionInputData[i].getType() == tokenSTRING){
			func_params_str = func_params_str + "i8* " + functionInputData[i].getName();

		}
		else{
			func_params_str = func_params_str + "i32 " + functionInputData[i].getName();

		}
		if(i != 0){
			func_params_str = func_params_str + ",";
		}
	}
	func_params_str = func_params_str + ")";
    ret->genReg();
	cb.emitCommand("\t"+ret->regToString()+" = call i32 @"+func_name+func_params_str, "function "+func_name+" is called with params: "+ func_params_str);

    if(ret->getType() == tokenBOOL){
        string old_reg = ret->regToString();
        ret->genReg();
        cb.emitCommand("\t" + ret->regToString() + " = trunc i32 " + old_reg + " to i1",
                "cast bool to i32");
        int loc = cb.emitCommand("\tbr i1 " + ret->regToString() + ", label @, label @",
                                 func_name);
        ret->true_list = cb.makelist({loc, FIRST});
        ret->false_list = cb.makelist({loc, SECOND});
    }

    // if(ret->getType() == tokenBOOL){
    //     string prev_reg = ret->regToString();
    //     ret->genReg();

    //     cd.emitCommand("\t"+ret->regToString()+" = icmp eq i32 "+prev_reg+", 1", "convert i32 to bool before");
    // }

	return ret;
}

/* ExpList */

/*
	void emptyFunctionInput();
	void appendFunctionInput(TerminalTokenEnum type, string name);
*/

// ExpList -> Exp
void ExpList_To_Exp(SymbolTable& symbol_table, Node* Exp){
    auto exp = (TypeToken*)Exp;
    TerminalTokenEnum type = ((TypeToken*)Exp)->getType();

    string reg_string = ((TypeToken*)Exp)->regToString();

    if(type != tokenEnumVal){
        symbol_table.appendFunctionInput(type, reg_string);
        return;
    }
    string name = ((EnumValNode*)Exp)->getEnumValName();
    Symbol* enum_type = symbol_table.getEnumType(name);
    if(enum_type != nullptr){
        symbol_table.appendFunctionInput(type, reg_string, enum_type->typeToString());
        return;
    }
    enum_type = symbol_table.getSymbolByName(name);
    symbol_table.appendFunctionInput(type, reg_string, ((VariableSymbol*)enum_type)->typeToString());
}

Node* Deal_If_Bool(Node* Exp){
    auto exp = (TypeToken*)Exp;
    TerminalTokenEnum type = ((TypeToken*)Exp)->getType();
    string tmp_reg;
    TypeToken* tempBool = NULL;
    if(type == tokenBOOL){
        tmp_reg = exp->phiAndZext();
        tempBool = new TypeToken(tokenBOOL);
        tempBool->genReg();

        CodeBuffer& cb = CodeBuffer::instance();

        cb.emitCommand("\t"+tempBool->regToString()+" = add i32 "+tmp_reg+", 0","special cmd");
        return tempBool;
    }

    return Exp;
}

// ExpList -> Exp COMMA ExpList
void ExpList_To_Exp_Comma_Explist(SymbolTable& symbol_table, Node* Exp, Node* Comma, Node* ExpList){
    auto exp = (TypeToken*)Exp;
    TerminalTokenEnum type = ((TypeToken*)Exp)->getType();

    string reg_string = ((TypeToken*)Exp)->regToString();


    if(type != tokenEnumVal){
        symbol_table.appendFunctionInput(type, reg_string);
        return;
    }
    string name = ((EnumValNode*)Exp)->getEnumValName();
    Symbol* enum_type = symbol_table.getEnumType(name);
    if(enum_type != nullptr){
        symbol_table.appendFunctionInput(type, reg_string, enum_type->typeToString());
        return;
    }
    enum_type = symbol_table.getSymbolByName(name);
    symbol_table.appendFunctionInput(type, reg_string, ((VariableSymbol*)enum_type)->typeToString());
}

void enterWhile(int& in_while_count, Node* M){
    in_while_count++;
    CodeBuffer& cb = CodeBuffer::instance();
    int loc = cb.emit("\tbr label @");
	string temp = cb.genLabel();
	M->nextInstr = temp;
    cb.emitCommand("", "while #" + to_string(in_while_count) + " start");
    cb.bpatch(cb.makelist({loc, FIRST}), temp);
    cb.continue_list.push_back(NextList());
    cb.break_list.push_back(NextList());
}

void exitWhile(int& in_while_count, Node* Exp){
    auto exp = (TypeToken*)Exp;
    CodeBuffer& cb = CodeBuffer::instance();
    int loc = cb.emitCommand("\tbr label @", "while #" + to_string(in_while_count) + " end");
    string temp = cb.genLabel();

    NextList break_bp = cb.merge(exp->false_list, cb.break_list[in_while_count]);
    NextList continue_bp = cb.merge(cb.makelist({loc, FIRST}), cb.continue_list[in_while_count]);
    cb.bpatch(continue_bp, exp->nextInstr);
    cb.bpatch(break_bp, temp);

    cb.continue_list.pop_back();
    cb.break_list.pop_back();
    in_while_count--;
}

Node* checkBOOLInIf(Node* node){
    CodeBuffer& cb = CodeBuffer::instance();
	TypeToken* exp = (TypeToken*)node;

	if(!exp->isBool()){
		errorMismatch(yylineno);
		exit(0);
	}
    cb.emitCommand("","true block start");

    string than_label = cb.genLabel();
    cb.bpatch(exp->true_list, than_label);

    return exp;
}

Node* bp_if(Node* Exp){
    CodeBuffer& cb = CodeBuffer::instance();
    auto exp = (TypeToken*)Exp;

    int exit_if = cb.emitCommand("\tbr label @", "exit if");
    string after_if = cb.genLabel();
    NextList temp = cb.merge(cb.makelist({exit_if, FIRST}), exp->false_list);
    cb.bpatch(temp, after_if);

}

Node* bp_if_else(Node* Exp, Node* M){
    CodeBuffer& cb = CodeBuffer::instance();
    auto exp = (TypeToken*)Exp;
    cb.emitCommand("","false block end");
    int exit_if = cb.emitCommand("\tbr label @", "br exit if");
    string after_else = cb.genLabel();
    cb.bpatch(exp->false_list, M->nextInstr);
    NextList temp = cb.merge(cb.makelist({exit_if, FIRST}), M->next_list);
    cb.bpatch(temp, after_else);
}