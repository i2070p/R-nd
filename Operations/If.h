#pragma once

#include "ComplexOperation.h"
#include "Expression.h"

enum IfBlocks {
    IF_BLOCK = 0,
    ELSE_BLOCK = 1
};

class If : public ComplexOperation {
public:

    If(Operation * parent) : ComplexOperation(parent) {

    }

    void addCondition(Condition* opr) {
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
    Condition *condition;

    void beforeGenerate(SpimCodeContainer* spimCode) {
        this->condition->startGenerate(spimCode);
    }

    void generate(SpimCodeContainer * spimCode) {

        this->children.at(IF_BLOCK)->startGenerate(spimCode);

        if (this->children.size() > 1) {
            ((Block*) this->children.at(ELSE_BLOCK))->startGenerate(spimCode);
        }
        
    }

    void afterGenerate(SpimCodeContainer* spimCode) {
        spimCode->addLabel();
    }
};

