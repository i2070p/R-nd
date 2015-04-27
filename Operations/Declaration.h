#pragma once

#include "SimpleOperation.h"
#include "../Elements/NameElement.h"
#include "../StackAdapter.h"
#include "Types.h"
#include "Expression.h"

using namespace std;

class Declaration : public SimpleOperation {
public:

    Declaration(Operation * parent, NameElement* var, Types type) : SimpleOperation(parent) {
        this->var = var;
        this->type = type;
    }

    void addExpression(Expression* exp) {
        this->exp = exp;
    }

    string toString() {
        stringstream ss;
        ss << "Declaration(" << this << ") {\n\tType: " << this->type << "\n\tName: " << this->var->getValue();

        if (this->exp) {
            ss << "\n\t" << this->exp->toString();
        }
        ss << "\n\tParent: " << this->getParent();
        ss << endl << "}";
        ;
        return ss.str();
    }

protected:
    Expression * exp;
    NameElement * var;
    Types type;

    void generate(SpimCodeContainer * spimCode) {
        if (this->exp) {
            stringstream line;
            this->exp->startGenerate(spimCode);
            Element * el = this->exp->getValueLiteral();
            line << "l" << (ElementUtilities::isInt(el) ? "i" : "w") << " $t0" << ", " << el->toString();
            spimCode->addOperation(line.str());
            line.str("");

            line << "sw " << "$t0" << ", " << this->var->toString();
            spimCode->addOperation(line.str());
        }
        spimCode->addVariable(this->var->toString(), ".word");

    }
};

