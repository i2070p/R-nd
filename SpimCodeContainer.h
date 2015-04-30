#pragma once

#include <sstream>

#include "StackAdapter.h"

using namespace std;

class SpimCodeContainer {
public:

    SpimCodeContainer() {
        this->variables << ".data" << endl;
        this->operations << ".text" << endl;
        this->tmp = 0;
        this->adr = 0;
    }

    void addVariable(string name, string type) {
        this->variables << name << ": " << type << endl;
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

    string toString() {
        stringstream ss;
        ss << this->variables.str() << endl << operations.str() << endl;
        return ss.str();
    }

protected:
    stringstream variables, operations;
    int tmp, adr, lbl;
};

