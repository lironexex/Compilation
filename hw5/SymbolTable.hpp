#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "attributes.hpp"
#include "vector"
#include <iostream>
#include <stdlib.h>

#define ENUM_VALUE_NOT_FOUND -1

using namespace std;

class dataObj{
	TerminalTokenEnum type;
	string name;
	string enumData;
public:
	dataObj(TerminalTokenEnum _type, string _name) : type(_type), name(_name){};
    dataObj(TerminalTokenEnum _type, string _name, string enum_name) :
            type(_type), name(_name), enumData(enum_name){};
	TerminalTokenEnum getType(){return type;}
	string getName(){return name;}
	string getEnumData(){return enumData;}
	string typeToString();
};

class Symbol {
protected:
	string name;
	int offset;
	TerminalTokenEnum type;

public:
	Symbol(string _name, int _offset, TerminalTokenEnum _type) : name(_name), offset(_offset), type(_type) {}
	string getName() {return name;}
	int getOffset() {return offset;}
	TerminalTokenEnum getType() {return type;} 
	virtual ~Symbol() {}
	virtual void print();
    string typeToString();
    void store(string val);
    void load(string dest);
    string reg_ptr_id;

    string enumInnerParam;

};


class VariableSymbol : public Symbol {
    string enumType; // only relevant for "type == tokenEnumVal"
public:
    VariableSymbol(string _name, int _offset, TerminalTokenEnum _type) :
            Symbol(_name, _offset, _type) {};
	VariableSymbol(string _name, int _offset, TerminalTokenEnum _type, string _enumType) :
	        Symbol(_name, _offset, _type), enumType(_enumType) {};
    string getEnumType(){ return enumType;};
    void print() override ;
};


class EnumSymbol : public Symbol {
	vector<string> enum_vector;
public:
	EnumSymbol(string _name, vector<string>& _enumVariables) :
	        Symbol(_name, 0, tokenENUM), enum_vector(_enumVariables) {};
	void insertList(vector<string>& _enumVariables);
    int getIndex(const string& str);
    bool exist(const string& str);
    vector<string>& getEnumVector(){ return enum_vector;}
    string getFirstParam() {return enum_vector[0];}
};

class FunctionSymbol : public Symbol {
	vector<dataObj> params;
 //    //TODO DANIEL: 'vector<string>' is needed because 'param' doesn't save the name of the Enum
 //    // its also easier to use because 'makeFunctionType' need it anyways
 //    // i think the best option is to update it together with 'param'
	// vector<string> paramTypes;
	TerminalTokenEnum ret_type;
public:
	FunctionSymbol(string _name, int _offset, TerminalTokenEnum _type,
		vector<dataObj> _params, TerminalTokenEnum _ret_type) :
		Symbol(_name, _offset, _type), params(_params), ret_type(_ret_type) {}
		vector<dataObj> getParams() {return params;}
        // vector<string>& getParamTypes() {return paramTypes;}

	vector<dataObj>& getFunctionInput(){return params;}
	vector<string> getStrFunctionInput();
	TerminalTokenEnum getRetType() {return ret_type;}
	void print() override ;





};


class Scope {
	vector<Symbol*> symbols;
	TerminalTokenEnum ret_type;
	int current_offset;
	int aug_size;
	bool doesRetExists;



public:
	Scope();
	~Scope();
	explicit Scope(TerminalTokenEnum rettype, vector<TerminalTokenEnum> params);
	void addVariableSymbol(const string& str, TerminalTokenEnum type, int reg_id, string enumType = "");
    void addFunctionSymbol(FunctionSymbol fs);
    void addEnumSymbol(EnumSymbol& es);
    Symbol* getSymbol(string name);

    int getAugSize() {return aug_size;}
    void setAugSize(int _size) {aug_size = _size;}

    void setRetExists(){ doesRetExists = true;}
    bool getDoesRetExists(){ return doesRetExists;}

    vector<Symbol*> getAllSymbols() {return symbols;}
    void setAllSymbols(vector<Symbol*> _symbols) {symbols = _symbols;}

    /**
     * @param str - the enum value
     * @return the enum symbol that includes 'str' as a value
     */
    EnumSymbol* getEnumSymbol(const string& str);

    /**
     * check if "str" is defined as a variable (int, char...)
     * excludes: 'enum identifiers', 'enum values'.
     * @param str
     */
    bool isVarDefined(const string& str);

    /**
     * check if "str" is an 'enum value'
     * excludes: 'enum identifiers'
     * @param str
     */
    bool isEnumDefined(const string& str);

    /**
     * check for identifier "str" already exist
     * @param str
     */
    bool isDefined(const string& str);
    void setScopeRetType(TerminalTokenEnum _type);
	TerminalTokenEnum getScopeRetType();

	void setCurrentOffset(int _set) { current_offset=_set;}
    int getCurrentOffset() { return current_offset;}

	void print_scope();

	static int ptr_generate;
};

class SymbolTable {
	vector<Scope> scopes;
	bool isMainExists; 
	vector<dataObj> functionInput;
	vector<string> enumList;
	string enumDecl;
	int next_reg;
public:
	SymbolTable();
	void NewScope(bool first_run = false);
	void NewFuncScope(string name, TerminalTokenEnum ret_type);

	void addVar(const string& str, TerminalTokenEnum type, string enumType = "");
	void addFunc(FunctionSymbol fs);
	void addEnum(EnumSymbol es);

	void DestroyCurrentScope(); /// okay ¯\_(ツ)_/¯
	TerminalTokenEnum getCurrentScopeRetType();

	void markMainExist() {isMainExists = true;}
	bool isMainLocated() {return isMainExists;};
	Symbol* getSymbolByName(string name);
	Scope& getCurrentScope();
	Scope& getGlobalScope();

	void emptyFunctionInput();
	void appendFunctionInput(TerminalTokenEnum type, string name, string enum_name = "");
	void appendFunctionInputScope(string name, TerminalTokenEnum ret_type);
	vector<dataObj>& getFunctionInput();
	vector<string> getStrFunctionInput();
	bool isFunctionInputEmpty() {return functionInput.size() == 0;}

    void emptyEnumList();
    void appendEnumList(string name);
    void appendEnumListScope(string name);
    void setEnumDecl(string str);
    EnumSymbol* getEnumType(string enumVal);

    bool isDefined(const string& str);

    void setRegID();
};

#endif