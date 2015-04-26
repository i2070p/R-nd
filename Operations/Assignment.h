#pragma once

#include "SimpleOperation.h"
#include <stack>
#include "../Elements/NameElement.h"
#include "../StackAdapter.h"
#include "Types.h"
#include "Expression.h"
#include <sstream>

using namespace std;

class Assignment : public SimpleOperation {
public:

    string generate(SpimCodeContainer * spimCode) {

    }

    Assignment(Operation * parent, NameElement* var) : SimpleOperation(parent) {
        this->var = var;
    }

    void addExpression(Expression* exp) {
        this->exp = exp;
    }

    string toString() {
        stringstream ss;
        ss << "Assignment(" << this << ") {\n\tName: " << this->var->getValue();

        if (this->exp) {
            ss << "\n\t" << this->exp->toString();
        }
        ss << "\n\tParent: " << this->getParent();
        ss << endl << "}";
        return ss.str();
    }

protected:
    Expression * exp;
    NameElement * var;
};

