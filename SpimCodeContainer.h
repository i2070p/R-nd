#pragma once

#include <sstream>
#include <map>
#include "StackAdapter.h"
#include "Operations/Type.h"
using namespace std;

class SpimCodeContainer {
public:

    SpimCodeContainer() {
        this->variables << ".data" << endl;
        this->operations << ".text" << endl;
        this->tmp = 0;
        this->adr = 0;
    }

    string addTmpFloatVar(string val) {
        string result = this->getNextTmpVar();
        this->addVariable(result, new Type(T_FLOAT), val);
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
        this->variables << name << ": " << type->toString() << endl;
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
        map<string,Type*>::const_iterator it = this->vars.find(key);
        if (it==this->vars.end()) {
            return NULL;
        }
        return this->vars[key];
    }

    string toString() {
        stringstream ss;
        ss << this->variables.str() << endl << operations.str() << endl;
        return ss.str();
    }

protected:
    stringstream variables, operations;

    map<string, Type*> vars;

    int tmp, adr, lbl;
};

