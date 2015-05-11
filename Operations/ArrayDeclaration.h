#pragma once

#include "SimpleOperation.h"
#include "../Elements/NameElement.h"
#include "../StackAdapter.h"
#include "Type.h"
#include "Expression.h"
#include "../Strings.h"
using namespace std;

class ArrayDeclaration : public SimpleOperation {
public:

    ArrayDeclaration(Operation * parent, NameElement* var, Type * type, int size) : SimpleOperation(parent) {
        this->var = var;
        this->type = type;
        this->size = size;
    }

protected:
    NameElement * var;
    Type * type;
    int size;

    void generate(SpimCodeContainer * spimCode) {
        stringstream line;
       // this->type->setArrayType();

        spimCode->addArray(this->var->toString(), this->type, this->size);
    }
};

