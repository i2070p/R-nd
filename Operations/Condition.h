#pragma once

#include "SimpleOperation.h"
#include <stack>
#include "../Elements/LiteralElement.h"
#include "../StackAdapter.h"
#include "Expression.h"
#include <sstream>

using namespace std;

class Condition : public SimpleOperation {
public:

    Condition(Operation * parent) : SimpleOperation(parent) {

    }

    void addExpression(Expression* exp) {
        this->exps.push(exp);
    }

    void addSign(SignElement* sign) {
        this->sign = sign;
    }


protected:
    StackAdapter<Expression*> exps;
    SignElement * sign;

    void generate(SpimCodeContainer * spimCode) {
        Expression * left = exps.pop();
        Expression * right = exps.pop();
        
        left->startGenerate(spimCode);
        right->startGenerate(spimCode);
        
        stringstream line;
        line << this->sign->getCommand() << " " << left->getValueLiteral()->toString() << ", " << right->getValueLiteral()->toString() << ", label" << spimCode->nextLabel();
        spimCode->addOperation(line.str());
    }

};

