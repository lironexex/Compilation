#include "attributes.hpp"


TypeToken::TypeToken(TerminalTokenEnum _type) : token_type(_type) {}

void TypeToken::setNewType(TerminalTokenEnum type) {
    token_type = type;
}

TerminalTokenEnum TypeToken::getType() {
    return token_type;
}

bool TypeToken::isBool() {
    return (token_type == tokenBOOL);
}

void TypeToken::setReg(int _value) {
    reg_number = _value;
}

int TypeToken::getReg() {
    return reg_number;
}

string TypeToken::regToString() {
    return "%reg" + to_string(reg_number);
}

int TypeToken::genReg(){
    reg_number = next_reg;
    return next_reg++;
}

string TypeToken::newRegString() {
    return to_string(next_reg++);
}

string TypeToken::phiAndZext() {
    CodeBuffer& cb = CodeBuffer::instance();
    // We make 2 labels for true and false bool values.
    // Backpatch the true/false lists to those labels.
    // Than we use 'phi' to assign the correct value. =)
    string true_label = cb.genLabel();
    cb.bpatch(true_list, true_label);
    int phi_true = cb.emitCommand("\tbr label @", "true label: " + true_label);

    string false_label = cb.genLabel();
    cb.bpatch(false_list, false_label);
    int phi_false = cb.emitCommand("\tbr label @", "false label: " + false_label);

    string phi_label = cb.genLabel();
    NextList phi_bp = cb.merge(cb.makelist({phi_true, FIRST}),
                               cb.makelist({phi_false, FIRST}));
    cb.bpatch(phi_bp, phi_label);
    genReg();
    cb.emit("\t" + regToString() +
            " = phi i1 [ 1, %" + true_label +"], [ 0, %" + false_label + " ]");
    TypeToken temp(tokenBOOL);
    temp.genReg(); // needed for new register
    cb.emitCommand("\t" + temp.regToString() +
            " = zext i1 " + regToString() + " to i32", "bool sign extend");
    return temp.regToString();
}

BoolOpToken::BoolOpToken(BoolOpEnum _op) : bool_op_type(_op) {}

string BoolOpToken::opToEmitStr() {
    string ret;
    switch (bool_op_type){
        case BOOL_OP_EQUAL: ret = "eq"; break;
        case BOOL_OP_NOT_EQUAL: ret = "ne"; break;
        case BOOL_OP_LEFT_BIG: ret = "sgt"; break;
        case BOOL_OP_RIGHT_BIG: ret = "slt"; break;
        case BOOL_OP_LEFT_BIG_EQUAL: ret = "sge"; break;
        case BOOL_OP_RIGHT_BIG_EQUAL: ret = "sle"; break;
        default: ret = "*** ERROR BoolOpToken invalid (and, or, not) ***";
    }
    return ret;
}

string BoolOpToken::opSign() {
    string ret;
    switch (bool_op_type){
        case BOOL_OP_EQUAL: ret = " == "; break;
        case BOOL_OP_NOT_EQUAL: ret = " != "; break;
        case BOOL_OP_LEFT_BIG: ret = " > "; break;
        case BOOL_OP_RIGHT_BIG: ret = " < "; break;
        case BOOL_OP_LEFT_BIG_EQUAL: ret = " >= "; break;
        case BOOL_OP_RIGHT_BIG_EQUAL: ret = " <= "; break;
        default: ret = "*** ERROR BoolOpToken invalid (and, or, not) ***";
    }
    return ret;
}

CmdToken::CmdToken(CmdOpEnum _cmd) : cmd_type(_cmd) {}

ArithmeticOp::ArithmeticOp(ArithmeticOpEnum _type) : arithmetic_op_type(_type) {}

string ArithmeticOp::opToEmitStr() {
    string ret;
    switch (arithmetic_op_type){
        case Artimetic_OP_PLUS: ret = "add"; break;
        case Artimetic_OP_MINUS: ret = "sub"; break;
        case Artimetic_OP_MULTIPLY: ret = "mul"; break;
        case Artimetic_OP_DIVIDE: ret = "sdiv"; break;
    }
    return ret;
}

ScopeVal::ScopeVal(ScopeTypeEnum _type) : scope_type(_type) {}

IdNode::IdNode(char *_str) : id_name(_str) {}

string IdNode::getName() {
    return id_name;
}

StrNode::StrNode(char *_type) : str_name(_type) {}

NumNode::NumNode(char *_str) : str_value(_str), value(atoi(_str)){}

int NumNode::getValue() {
    return value;
}

string NumNode::getStrValue() {
    return str_value;
}

bool NumNode::isByteValid() {
    return (getValue() >= 0 && getValue() <= 255);
}

EnumNode::EnumNode(string _str) : TypeToken(tokenENUM), enum_name(_str) {}

string EnumNode::getName() {
    return enum_name;
}

EnumValNode::EnumValNode(string _str) : TypeToken(tokenEnumVal), enum_val_name(_str) {}

string EnumValNode::getEnumValName() {
    return enum_val_name;
}
