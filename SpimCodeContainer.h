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
        this->labels.push(++this->lbl);
        return this->lbl;
    }

    int currentLabel() {
        return this->labels.pop();
    }

    void addLabel(bool inc) { 
        if (inc) {
            this->nextLabel();
        }
        this->operations << ":label" << this->currentLabel() << endl;
    }

    
    string toString() {
        stringstream ss;
        ss << this->variables.str() << endl << operations.str() << endl;
        return ss.str();
    }

protected:
    stringstream variables, operations;
    StackAdapter<int> labels;
    int tmp, adr, lbl;
};

