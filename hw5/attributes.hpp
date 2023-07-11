#ifndef ATTRIBUTES_H
#define ATTRIBUTES_H

#include <iostream>

#include "bp.hpp"
#include "attribute_types.hpp"
#include "string"

using namespace std;

class Node {
public:
    string nextInstr;
    NextList next_list;
	Node(){}
	virtual ~Node() {}
};


class TypeToken : public Node {
protected:
	TerminalTokenEnum token_type;
public:
	explicit TypeToken(TerminalTokenEnum _type);
	void setNewType(TerminalTokenEnum type);
	TerminalTokenEnum getType();
	bool isBool();
	void setReg(int _value);
	int getReg();
	string regToString();

	int genReg();

	string newRegString();
	/**
	 * print the branches and assign after a bool.
	 * TODO: maybe use the 'token' to deside what to store
	 * @return the register that holds the result
	 */
	string phiAndZext();


    int reg_number;
    static int next_reg;
    NextList false_list, true_list;

};

class BoolOpToken : public Node {
	BoolOpEnum bool_op_type;
public:
	explicit BoolOpToken(BoolOpEnum _op);
    string opToEmitStr();
    string opSign();

    int br_label;
};


class CmdToken : public Node {
	CmdOpEnum cmd_type;
public:
	CmdToken(CmdOpEnum _cmd);
};

class ArithmeticOp : public Node {
	ArithmeticOpEnum arithmetic_op_type;
public:
	explicit ArithmeticOp(ArithmeticOpEnum _type);
    string opToEmitStr();
};

class ScopeVal : public Node {
	ScopeTypeEnum scope_type;
public:
	explicit ScopeVal(ScopeTypeEnum _type);
};

class IdNode : public Node {
	string id_name;
public:
	explicit IdNode(char* _str);
	string getName();
};

class StrNode : public Node {
	string str_name;
public:
	explicit StrNode(char* _type);
};

class NumNode : public Node {
	string str_value;
	int value;
public:
	explicit NumNode(char* _str);
	int getValue();
	string getStrValue();

	bool isByteValid();
};

class EnumNode : public TypeToken {
    string enum_name;
public:
    explicit EnumNode(string _str);
    string getName();
};

class EnumValNode : public TypeToken {
    string enum_val_name;
public:
    explicit EnumValNode(string _str);
    string getEnumValName();
};



#define YYSTYPE Node*

#endif
