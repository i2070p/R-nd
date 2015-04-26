#pragma once

#include "Operation.h"

class ComplexOperation : public Operation{
public:

    ComplexOperation(Operation* parent) : Operation(parent) {

    }
    
    virtual string generate(SpimCodeContainer * spimCode) = 0;

    void add(Operation *opr) {
        this->children.push_front(opr);
    }

    Operation* getChild(int i) {
        return this->children.at(i);
    }

    StackAdapter<Operation*> getChildren() {
        return this->children;
    }
    
protected:
    StackAdapter<Operation*> children;

};

