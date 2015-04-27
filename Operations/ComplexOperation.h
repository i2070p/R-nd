#pragma once

#include "Operation.h"

class ComplexOperation : public Operation {
public:

    ComplexOperation(Operation* parent) : Operation(parent) {

    }

    void add(Operation *opr) {
        this->children.push(opr);
    }

    Operation* getChild(int i) {
        return this->children.at(i);
    }

    StackAdapter<Operation*> getChildren() {
        return this->children;
    }

protected:

    StackAdapter<Operation*> children;

    virtual void generateChildren(SpimCodeContainer * spimCode) {
        for (int i = 0; i < this->children.size(); i++) {
            this->children.at(i)->startGenerate(spimCode);
        }
    }


};

