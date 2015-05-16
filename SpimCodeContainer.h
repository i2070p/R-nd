#pragma once

#include <sstream>
#include <map>
#include "StackAdapter.h"
#include "Operations/Type.h"
#include "Strings.h"
using namespace std;

class SpimCodeContainer {
public:

    SpimCodeContainer() {
        this->variables << ".data" << endl;
        this->addVariable("$f_one", new Type(T_FLOAT), 1.0f);
        this->addVariable("$f_zero", new Type(T_FLOAT), 0.0f);
        this->addVariable("$new_line", new Type(T_STR), "\"\\n\"");
        this->operations << ".text" << endl;
        this->tmp = 0;
        this->adr = 0;
    }

    string addTmpFloatVar(string val) {
        string result = this->getNextTmpVar();
        this->addVariable(result, new Type(T_FLOAT), val);
        return result;
    }

    string addTmpStringVar(string val) {
        string result = this->getNextTmpVar();
        this->addVariable(result, new Type(T_STR), val);
        return result;
    }

    void addVariable(string name, Type * type, int init) {
        this->vars.insert(pair<string, Type*>(name, type));
        this->variables << name << ": " << type->toString() << " " << init << endl;
    }

    void addVariable(string name, Type * type, float init) {
        this->vars.insert(pair<string, Type*>(name, type));
        this->variables << name << ": " << type->toString() << " " << init << endl;
    }

    void addVariable(string name, Type * type, string init) {
        this->vars.insert(pair<string, Type*>(name, type));
        this->variables << name << ": " << type->toString() << " " << init << endl;
    }

    void addVariable(string name, Type * type) {
        this->vars.insert(pair<string, Type*>(name, type));
        this->variables << name << ": " << type->toString() << (type->is(T_STR) ? " \"\"" : " 0") << endl;
    }

    void addArray(string name, Type * type, int size) {
        this->vars.insert(pair<string, Type*>(name, type));
        this->arrayLength.insert(pair<string, int>(name, size));
        this->variables << name << ": " << type->toString() << " 0:" << size << endl;
    }

    void addOperation(string operation) {
        this->operations << operation << endl;
    }

    int nextTmp() {
        return this->tmp++;
    }

    int nextAdr() {
        return this->adr++;
    }

    int nextLabel() {
        return ++this->lbl;
    }

    void addLabel(int id) {
        this->operations << "label" << id << ":" << endl;
    }

    void addLabel() {
        this->addLabel(this->lbl);
    }

    string getNextTmpVar() {
        return this->getNextTmpVar(NULL);
    }

    string getNextTmpVar(int * i) {
        stringstream ss;

        if (!i) {
            i = new int(0);
        }

        *i = this->nextTmp();
        ss << "$tmp" << *i;
        return ss.str();
    }

    Type* getVariable(string key) {
        map<string, Type*>::const_iterator it = this->vars.find(key);
        if (it == this->vars.end()) {
            throw string(Strings::getUndeclaredText(key));
        }
        return this->vars[key];
    }

    string toString() {
        stringstream ss;
        ss << this->variables.str() << endl << operations.str() << endl;
        return ss.str();
    }
    StackAdapter<Element*> mStack;
protected:
    stringstream variables, operations;

    map<string, Type*> vars;
    map<string, int> arrayLength;
    int tmp, adr, lbl;
};

