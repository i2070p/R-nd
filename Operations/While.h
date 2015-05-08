#pragma once

#include "ComplexOperation.h"
#include "Expression.h"

class While : public ComplexOperation {
public:

    While(Operation * parent) : ComplexOperation(parent) {

    }

    void addCondition(Expression* opr) {
        this->condition = opr;
    }

protected:
    Expression *condition;

    void generate(SpimCodeContainer * spimCode) {
        stringstream line;

        int labelUp = spimCode->nextLabel();
        int labelDown = spimCode->nextLabel();
        spimCode->addLabel(labelUp);

        this->condition->startGenerate(spimCode);
        Element el = this->condition->getValueLiteral();
        if (el->)
        line << "li $t0, 0" << endl;
        line << "lw $t1, " << this->condition->getValueLiteral()->toString() << endl;
        line << "bne $t0, $t1, label" << labelDown << endl;
        
        spimCode->addOperation(line.str());
        line.str("");

        this->children.at(IF_BLOCK)->startGenerate(spimCode);

        line << "b label" << labelUp;
        spimCode->addOperation(line.str());
        line.str("");

        spimCode->addLabel(labelDown);

    }

};

