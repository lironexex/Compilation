#include "SymbolTable.hpp"
#include <stdlib.h>
#include "output.hpp"
#include "bp.hpp"

using namespace std;
using namespace output;

bool is_debug = false;
bool print_scope_debug = false;

string dataObj::typeToString() {
	string ret;
	switch(type){
        case tokenVOID: ret = "VOID"; break;
		case tokenINT: ret = "INT"; break;
		case tokenBOOL: ret ="BOOL"; break;
		case tokenBYTE: ret = "BYTE"; break;
		case tokenSTRING: ret = "STRING"; break;
        case tokenEnumVal: ret = "enum " + enumData; break;
		case tokenENUM: ret = "enum " + enumData; break; //might need to get different treatment for ENUM
        default:
            ret = "ERROR SHOULD NOT BE HERE"; break;
	}
	return ret;
}

/*****************************************************************************
                                    Symbol
 *****************************************************************************/


void Symbol::print() {
    printID(name, offset, typeToString());
}

string Symbol::typeToString() {
    string temp;
    switch (type){
        case tokenVOID:
            temp = "VOID";
            break;
        case tokenINT:
            temp = "INT";
            break;
        case tokenBYTE:
            temp = "BYTE";
            break;
        case tokenBOOL:
            temp = "BOOL";
            break;
        case tokenSTRING:
            temp = "STRING";
            break;
        case tokenEnumVal:
            temp = ((VariableSymbol*)this)->getEnumType();
            break;
        case tokenENUM:
            temp = ((EnumSymbol*)this)->getName();
            break;
        default:
            if(is_debug) cout << "symbol print error" << endl;
            break;
    }
    return temp;
}

void Symbol::store(string val) {
	CodeBuffer::instance().emitCommand("\tstore i32 " + val + ", i32* " + reg_ptr_id,
                                       "load: " + name + " = " + val);
}

void Symbol::load(string dest) {
    CodeBuffer::instance().emitCommand("\t" + dest + " = load i32, i32* " + reg_ptr_id,
                                       "load: " + dest + " = " + name);
}

void VariableSymbol::print() {
    if(type == tokenEnumVal){
        printID(name, offset,"enum " + enumType);
        return;
    }
    Symbol::print();
}


void EnumSymbol::insertList(vector<string>& _enumVariables) {
    enum_vector = _enumVariables;
}

int EnumSymbol::getIndex(const string &str) {
    for(int i = 0; i < enum_vector.size(); i++){
        if(enum_vector[i] == str) return i;
    }
    return ENUM_VALUE_NOT_FOUND;
}

bool EnumSymbol::exist(const string &str) {
    return (getIndex(str) != ENUM_VALUE_NOT_FOUND);
}


vector<string> FunctionSymbol::getStrFunctionInput(){
    vector<string> ret = vector<string>();
    for(int i = 0; i < params.size(); i++){
		ret.push_back(params[i].typeToString());
    }
    return ret;
}

void FunctionSymbol::print() {
    dataObj temp = dataObj(ret_type,"");
    string ret = temp.typeToString();
    vector<string> input = vector<string>();
    for(int i = 0; i < params.size(); i++){
        input.push_back(params[i].typeToString());
    }
    ret = makeFunctionType(ret,input);
    printID(name, 0, ret);
}

/*****************************************************************************
                                     Scope
 *****************************************************************************/


Scope::Scope(): symbols(vector<Symbol*>()), current_offset(0), ret_type(tokenNONEXIST), doesRetExists(false){}

Scope::Scope(TerminalTokenEnum rettype, vector<TerminalTokenEnum> params): symbols(vector<Symbol*>()), doesRetExists(false) {
	//TODO: properly get into the scope.
	setScopeRetType(rettype);
}

Scope::~Scope() {
    // for (auto& s: symbols){
    //     delete(s);
    // }
}

void Scope::addVariableSymbol(const string& str, TerminalTokenEnum type, int reg_id, string enumType) {

    VariableSymbol* temp;

    if(type == tokenEnumVal){
        temp = new VariableSymbol(str, current_offset++, tokenEnumVal, enumType);
    } else{
        temp = new VariableSymbol(str, current_offset++, type);
    }

    temp->reg_ptr_id = "%t" + to_string(ptr_generate) + to_string(reg_id);
    ptr_generate++;

    symbols.push_back(temp);
    CodeBuffer& cb = CodeBuffer::instance();
	cb.emitCommand("\t" + temp->reg_ptr_id +
            " = getelementptr inbounds [50 x i32], [50 x i32]* %stack, i32 0, i32 " + to_string(temp->getOffset() + getAugSize()),
            temp->typeToString() + " " + temp->getName() + "; (" + temp->reg_ptr_id + ")");
    cb.emitCommand("\tstore i32 0, i32* " + temp->reg_ptr_id,
            temp->getName() + " = 0 (init with default value)" );

    if(temp->getOffset() < 0){
        cb.emitCommand("\t store i32 %"+to_string(temp->getOffset()*(-1) - 1)+", i32* "+temp->reg_ptr_id, "loading argument number "+to_string(temp->getOffset())+" into stack");
    }

}

void Scope::addFunctionSymbol(FunctionSymbol fs) {
    if(is_debug) cout << "addFunctionSymbol " << fs.getName() << " " << fs.getType() << endl;


    Symbol* temp = new FunctionSymbol(fs.getName(), 0, fs.getType(), fs.getFunctionInput(), fs.getRetType());

    symbols.push_back(temp);
}

void Scope::addEnumSymbol(EnumSymbol& es) {
    auto temp = new EnumSymbol(es);
    symbols.push_back(temp);
}

Symbol* Scope::getSymbol(string name){
    Symbol* cur_symbol;
	for(vector<Symbol*>::iterator it = symbols.begin(); it != symbols.end(); it++){
        cur_symbol = (*it);
		if((*it)->getName() == name){
            if(is_debug) cout << "getSymbol found symbol: " << (*it)->getName() << endl;
			return *it; // TODO: might need to return a copy to make it properly work.
		}
	}
	return NULL;
}

EnumSymbol* Scope::getEnumSymbol(const string &name) {
    for (auto& es: symbols){
        if(es->getType() == tokenENUM){
            auto temp = dynamic_cast<EnumSymbol*>(es); //TODO: check cast warning
            if(temp && temp->exist(name)){
                return temp;
            }
        }
    }
    return nullptr;
}

bool Scope::isVarDefined(const string& str) {
    for(auto& s: symbols){
        if(s->getType() != tokenENUM && s->getName() == str) return true;
    }
    return false;
}

bool Scope::isEnumDefined(const string& str){
    return (bool)getEnumSymbol(str);
}

bool Scope::isDefined(const string& str) {
    for(auto s: symbols){
        if(s->getName() == str) return true;
        if(s->getType() == tokenENUM){
            auto temp = (EnumSymbol*)s;
            if(temp->exist(str)) return true;
        }
    }
    return false;
}

void Scope::setScopeRetType(TerminalTokenEnum type){
	ret_type = type;
}

TerminalTokenEnum Scope::getScopeRetType(){
	return ret_type;
}

void Scope::print_scope() {
    endScope();
    // print vars
    for (int i = 0; i < symbols.size(); i++) {
        auto type = symbols[i]->getType();
        if (type == tokenENUM || type == tokenFUNC) {
            continue;
        }
        symbols[i]->print();
    }
    // TODO: check order of printing...
    // print functions
    for (int i = 0; i < symbols.size(); i++) {
        auto type = symbols[i]->getType();
        if (type != tokenFUNC) {
            continue;
        }
        symbols[i]->print();
    }
    // print enums
    for (int i = 0; i < symbols.size(); i++) {
        auto type = symbols[i]->getType();
        if (type != tokenENUM) {
            continue;
        }
        printEnumType(symbols[i]->getName(), ((EnumSymbol*)symbols[i])->getEnumVector());
    }
}





/*****************************************************************************
                                   SymbolTable
 *****************************************************************************/
SymbolTable::SymbolTable() : scopes(vector<Scope>()), isMainExists(false), functionInput(vector<dataObj>()),
                            enumList(vector<string>()), enumDecl(), next_reg(0) {
	if (is_debug) cout << "SymbolTable init" << endl;

    scopes.push_back(Scope());



    vector<dataObj> tmp_input = vector<dataObj>();
    tmp_input.push_back(dataObj(tokenSTRING, "string_input"));
    addFunc(FunctionSymbol("print", 0, tokenFUNC, tmp_input, tokenVOID));

    vector<dataObj> tmp_input2 = vector<dataObj>();
    tmp_input2.push_back(dataObj(tokenINT, "string_input"));
    addFunc(FunctionSymbol("printi", 0, tokenFUNC, tmp_input2, tokenVOID));


    CodeBuffer& cb = CodeBuffer::instance();

    cb.emitGlobal("declare i32 @printf(i8*, ...)        ; declare printf from C");
    cb.emitGlobal("declare void @exit(i32)              ; declare exit from C");

    cb.emitGlobal("@.int_specific = constant [4 x i8] c\"%d\\0A\\00\"");
    cb.emitGlobal("@.str_specific = constant [4 x i8] c\"%s\\0A\\00\"");

    cb.emitGlobal("define i32 @printi(i32) {");
    cb.emitGlobal("call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @.int_specific, i32 0, i32 0), i32 %0)");
    cb.emitGlobal("ret i32 0");
    cb.emitGlobal("}");

    cb.emitGlobal("define i32 @print(i8*) {");
    // cb.emitGlobal("define i32 @print(i32*) {");
    cb.emitGlobal("call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @.str_specific, i32 0, i32 0), i8* %0)");
    cb.emitGlobal("ret i32 0");
    cb.emitGlobal("}");

    cb.emitGlobal("@.printerror = constant [23 x i8] c\"Error division by zero\\00\"");

    if (is_debug) cout << "SymbolTable init Ended" << endl;
}

void SymbolTable::NewScope(bool first_run){
	if (is_debug) cout << "NewScope" << endl;

    TerminalTokenEnum ret_type;
    int prev_off_set;
    int prev_aug_size;
    vector<Symbol*> prev_symbols;

    if(first_run){
        ret_type = getCurrentScopeRetType();
        prev_off_set = getCurrentScope().getCurrentOffset();
        prev_aug_size = getCurrentScope().getAugSize();
        prev_symbols = getCurrentScope().getAllSymbols();
    }

    scopes.push_back(Scope());

    if(first_run){
        getCurrentScope().setScopeRetType(ret_type);
        getCurrentScope().setCurrentOffset(prev_off_set);
        getCurrentScope().setAugSize(prev_aug_size);
        getCurrentScope().setAllSymbols(prev_symbols);
    }

	// Scope temp = Scope();
	// Scope last = getCurrentScope();
	// temp.setCurrentOffset(last.getCurrentOffset());
	// scopes.push_back(temp);
}

void SymbolTable::NewFuncScope(string name, TerminalTokenEnum ret_type){
	appendFunctionInputScope(name, ret_type);

    NewScope();

    int input_size = functionInput.size();
    int cur_number = 0;

    getCurrentScope().setCurrentOffset(input_size);
    getCurrentScope().setScopeRetType(ret_type);
    getCurrentScope().setAugSize(functionInput.size());

    for(vector<dataObj>::iterator it = functionInput.begin(); it != functionInput.end(); it++){
        cur_number--;
        getCurrentScope().setCurrentOffset(cur_number);
        //DANIEL START HERE FUNCTION
        if(it->getType() == tokenENUM){
            addVar(it->getName(), tokenEnumVal, it->getEnumData());
        }
        else{
            addVar(it->getName(), it->getType());
        }
    }
    emptyFunctionInput();
    getCurrentScope().setCurrentOffset(0);


}

void SymbolTable::DestroyCurrentScope(){
	if (is_debug) cout << "DestroyScope" << endl;
//	for(vector<Symbol*>::iterator it = scopes_vector.back().begin(); it != scopes_vector.back().end(); it ++){
//		delete (*it);
//	}
    if(print_scope_debug) getCurrentScope().print_scope();

	scopes.pop_back();
}

// void SymbolTable::setScopeRetType(TerminalTokenEnum _type)
// {
// 	if (is_debug) cout << "setScopeRetType" << endl;
// 	ret_type = _type;
// }

TerminalTokenEnum SymbolTable::getCurrentScopeRetType(){
	return getCurrentScope().getScopeRetType();
}

void SymbolTable::addVar(const string& str, TerminalTokenEnum type, string enumType){
    if(type == tokenEnumVal){
        getCurrentScope().addVariableSymbol(str, type, next_reg++, enumType);
    }
    else{
        getCurrentScope().addVariableSymbol(str, type, next_reg++);
    }
}
void SymbolTable::addFunc(FunctionSymbol fs){

	//TODO: check if it's main and flip isMainExists is it is void main();

    //TODO: if(fs.getName() == "main" && fs.getType() == tokenFUNC && fs.getRetType() == tokenVOID){

    if(is_debug) cout << "addFunc " << fs.getName() << " " << fs.getType() << endl;





    //TODO check if function exists.

	getCurrentScope().addFunctionSymbol(fs);
}
void SymbolTable::addEnum(EnumSymbol es){
	getCurrentScope().addEnumSymbol(es);
}


Symbol* SymbolTable::getSymbolByName(string name){
	Symbol* ret = NULL;
	for(vector<Scope>::iterator it = scopes.begin(); it != scopes.end(); it++){
		ret = it->getSymbol(name);
		if(ret){
			break;
		}
	}
	return ret;
}

Scope& SymbolTable::getCurrentScope(){

	return scopes.back();
}

Scope &SymbolTable::getGlobalScope() {
    return scopes[0];
}


void SymbolTable::emptyFunctionInput(){
	functionInput.clear();
}
void SymbolTable::appendFunctionInput(TerminalTokenEnum type, string name, string enum_name){

	functionInput.push_back(dataObj(type, name, enum_name));
}

void SymbolTable::appendFunctionInputScope(string name, TerminalTokenEnum ret_type){

	//TODO DANIEL: Maybe need to flip the list.
	addFunc(FunctionSymbol(name, 0, tokenFUNC, functionInput, ret_type));

}

void SymbolTable::emptyEnumList(){
    enumList.clear();
}
void SymbolTable::appendEnumList(string name){
    enumList.push_back(name);
}

void SymbolTable::appendEnumListScope(string name) {
    addEnum(EnumSymbol(name, enumList));
}

void SymbolTable::setEnumDecl(string str) {
    enumDecl = str;
}

EnumSymbol* SymbolTable::getEnumType(string enumVal){
    for(auto& scope: scopes){
        if(scope.isEnumDefined(enumVal))
            return scope.getEnumSymbol(enumVal);
    }
    return nullptr;
}


bool SymbolTable::isDefined(const string &str) {
    for(auto& scope : scopes){
        if(scope.isDefined(str)) return true;
    }
    // incase of enum declaration check also the enum list
    for(auto& e : enumList) {
        if (e == str) return true;
    }
    if(enumDecl == str) return true;
    return false;
}

vector<dataObj>& SymbolTable::getFunctionInput(){
        return functionInput;
    }

vector<string> SymbolTable::getStrFunctionInput(){
    vector<string> ret = vector<string>();
    for(int i = 0; i < functionInput.size(); i++){
		ret.push_back(functionInput[i].typeToString());
    }
    return ret;
}

void SymbolTable::setRegID() {

}
