/* 
 * File:   IfOperation.h
 * Author: meti
 *
 * Created on 22 kwiecień 2015, 09:51
 */

#pragma once

#include "Operation.h"

class ComplexOperation : public Operation {
public:
    string generate() = 0;

    void add(string name, Operation *opr) {
        this->children.insert(pair<string, Operation*>(name, opr));
    }

    void remove(string name) {
        this->children.erase(this->children.find(name));
    }

    Operation* getChild(string name) {
        return this->children.find(name)->second;     
    }
protected:
    map<string, Operation*> children;

};

