#ifndef HW3
#define HW3

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cstdlib>
#include "hw3_output.hpp"
using namespace std;

enum TType{
    DEFAULTT,BYTEE,INTT,BOOLL,VOIDD,STRINGG,FUNCC
};
struct func{
    static string IntToString (int a)
    {
        ostringstream tmp;
        tmp<<a;
        return tmp.str();
    }
    static string TTypeToString(TType t){
    switch(t){
    case BYTEE: return "BYTE";
    case INTT: return "INT";
   case  BOOLL: return "BOOL";
    case VOIDD: return "VOID";
   case  STRINGG: return "STRING";
    default: return "DEFAULT";
    }
    }
};
struct Node{
    TType type;
    virtual ~Node(){}
};

struct Num:Node{
    int value;
    Num(char * ytext){
        type=DEFAULTT;
        value=atoi(ytext);
    }
    Num(int v,TType t){
        type=t;
        value=v;
    }
};

struct Bool:Node{
    Bool(){
        type=BOOLL;
    }
};
struct Void:Node{
    Void(){
        type=VOIDD;
    }
};
struct String:Node{
    String(){
        type= STRINGG;
    }
};
struct Type:Node{
    Type(){
        type=DEFAULTT;
    }
    Type(TType t){
        type=t;
    }
};
struct Id:Node{
    string id;
    Id(char * ytext){
        type=DEFAULTT;
        id= string (ytext);
    }
};
struct Func:Node{
    string name;
    Func(string s){
        type=DEFAULTT;
        name=s;
    }
};
struct Explist:Node{
    vector<TType> vec;
    Explist(){}
    Explist(TType t){vec.push_back(t);}
    void insert(TType t){
        vec.insert(vec.begin(),t);
    }
};

struct scopeVal{
    string name;
    TType type;
    int offset;
    bool Function;
    vector<TType> argTypes;
    TType retType;
    scopeVal(){}
    scopeVal(string _name, TType _type, int _offset,bool _Function=false,TType _retType =DEFAULTT,vector<TType> v=vector<TType>()){
        name=_name;
        type=_type;
        offset=_offset;
        Function=_Function;
        retType=_retType;
        argTypes=v;
    }
};
struct Scope{
    vector<scopeVal> values;
    int argOffset;
    Scope(){
    }
    bool isExist(string name ){
        for (int i = values.size()-1 ; i >=0 ; --i) {
            if(values[i].name==name){
                return true;
            }
        }
        return false;
    }
    scopeVal search(string name){
        for (int i = values.size()-1 ; i >=0 ; --i) {
            if(values[i].name==name){
                return values[i];
            }
        }
        return scopeVal();
    }
};


struct SymbolTable{
    vector<int> offsets;
    vector<Scope> scopes;
    TType currRetType;
    int whileNum;
    int preNum;
    SymbolTable(){
        whileNum=0;
        preNum=0;
        currRetType=DEFAULTT;
        Scope s;
        offsets.push_back(0);
        vector<TType> v;
        v.push_back(STRINGG);
        s.values.push_back(scopeVal("print",FUNCC,0,true,VOIDD,v));
        v.clear();
        v.push_back(INTT);
        s.values.push_back(scopeVal("printi",FUNCC,0,true,VOIDD,v));
        scopes.push_back(s);
    }
    void openScope(){
        scopes.push_back(Scope());
        offsets.push_back(offsets[offsets.size()-1]);
        scopes[scopes.size()-1].argOffset= offsets[offsets.size()-1]-1;
    }
    void closeScope(){
        scopes.erase(scopes.begin()+scopes.size()-1);
        offsets.erase(offsets.begin()+offsets.size()-1);
    }
    void addID(TType t,Node* node){
        Id* tmp= dynamic_cast<Id*>(node);
        int offset=offsets[offsets.size()-1]++;
        scopes[scopes.size()-1].values.push_back(scopeVal(tmp->id,t,offset));
    }
    void addArgID(TType t,Node* node){
        Id* tmp= dynamic_cast<Id*>(node);
        int offset=scopes[scopes.size()-1].argOffset--;
        scopes[scopes.size()-1].values.push_back(scopeVal(tmp->id,t,offset));
    }
    void addFunc(Node* retType,Node* id,Node* args){
        Id* name=dynamic_cast<Id*>(id);
        Explist* argvec=dynamic_cast<Explist*>(args);
        scopes[0].values.push_back(scopeVal(name->id,FUNCC,0,true,retType->type,argvec->vec));
    }
    TType ScopeRetType(){
        return currRetType;
    }
    bool mainExist(){
        if(scopes[0].isExist("main")){
           scopeVal sv=scopes[0].search("main");
            if(sv.retType != VOIDD || !sv.argTypes.empty())
                 return false;
            return true;
        }
        return false;
    }
    bool isExist(Node* node){
        Id* tmp= dynamic_cast<Id*>(node);
        bool found=false;
        for (int i = scopes.size()-1 ; i >=0 ; --i) {
            if(scopes[i].isExist(tmp->id))
                return true;
        }
        return false;
    }
    scopeVal search(Node* node){
        Id* tmp= dynamic_cast<Id*>(node);
        for (int i = scopes.size()-1 ; i >=0 ; --i) {
            if(scopes[i].isExist(tmp->id))
                return scopes[i].search(tmp->id);
        }
        return scopeVal();
    }
    bool checkArg(vector<TType> orig,vector<TType> caller){
        if(orig.size()!=caller.size()) return true;
        for (int i = 0; i < orig.size(); ++i) {
            if(orig[i]!=caller[i] && !(orig[i]==INTT&&caller[i]==BYTEE)){
                return true;
            }
        }
        return false;
    }
    vector<string> argToString(vector<TType> orig){
        vector<string> v;
        for (int i = 0; i < orig.size(); ++i) {
            if(orig[i]==INTT) v.push_back("INT");
            else if(orig[i]==BYTEE) v.push_back("BYTE");
            else if(orig[i]==STRINGG) v.push_back("STRING");
            else if(orig[i]==BOOLL) v.push_back("BOOL");
            else v.push_back("prob");
        }
        return v;
    }
    void printScope(string funName="",int preconditions=-1){
        output::endScope();
        int numOfId=scopes[scopes.size()-1].values.size();
        for(int i=0;i<numOfId;i++){
            scopeVal sv=scopes[scopes.size()-1].values[i];
            if(!sv.Function){
                string s=func::TTypeToString(sv.type);
                output::printID(sv.name,sv.offset,s);
            }
            else{
                string ret=func::TTypeToString(sv.retType);
                vector<string> argtypes;
                for(int i=0;i<sv.argTypes.size();i++){
                    argtypes.push_back(func::TTypeToString(sv.argTypes[i]));
                }
                output::printID(sv.name,sv.offset,output::makeFunctionType(ret,argtypes));
            }
                
        }
    }
    

};

#define YYSTYPE Node*
#endif //HW3
