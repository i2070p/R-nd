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

    void generate(SpimCodeContainer * spimCode) {
        stringstream line;

        this->condition->startGenerate(spimCode);

        line << "li $t0, 0";
        spimCode->addOperation(line.str());
        line.str("");

        line << "lw $t1, " << this->condition->getValueLiteral()->toString();
        spimCode->addOperation(line.str());
        line.str("");

        bool existElse = this->children.size() > 1;

        int tmp = spimCode->nextLabel();

        line << "beq $t0, $t1, label" << tmp;
        spimCode->addOperation(line.str());
        line.str("");

        int tmp2 = 0;
        
        if (existElse) {
            tmp2 = spimCode->nextLabel();
        }

        this->children.at(IF_BLOCK)->startGenerate(spimCode);

        if (existElse) {

            line << "b label" << tmp2;
            spimCode->addOperation(line.str());
            line.str("");
        }

        spimCode->addLabel(tmp);

        if (existElse) {

            ((Block*) this->children.at(ELSE_BLOCK))->startGenerate(spimCode);

            spimCode->addLabel(tmp2);
        }

    }

};

