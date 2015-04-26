#pragma once

#include "ComplexOperation.h"
#include "Expression.h"

class If : public ComplexOperation {
public:

    string generate(SpimCodeContainer * spimCode) {

    }

    If(Operation * parent) : ComplexOperation(parent) {

    }

    void addCondition(Expression* opr) {
        this->condition = opr;
    }
    
    string toString() {
        stringstream ss;
        ss << "If(" << this << ") {";
        ss << "\n\tParent: " << this->getParent();
        ss << endl << "}";
        return ss.str();
    }

protected:
    Expression *condition;
};

