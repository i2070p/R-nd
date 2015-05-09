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
        Element * el = this->condition->getValueLiteral();

        line << "li $t0, 0" << endl;

        if (ElementUtilities::isInt(el)) {
            line << "li $t1, " << el->toString() << endl;
        } else if (ElementUtilities::isFloat(el)) {
            string tmp = spimCode->addTmpFloatVar(el->toString());
            this->generateConditionForFloat(line, tmp);
            
        } else if (ElementUtilities::isName(el)) {
            Type * type = spimCode->getVariable(el->toString());
            if (type) {
                if (type->is(T_INT)) {
                    line << "lw" << " $t1" << ", " << el->toString() << endl;
                } else if (type->is(T_FLOAT)) {
                    this->generateConditionForFloat(line, el->toString());
                }
            }
        }

        line << "beq $t0, $t1, label" << labelDown << endl;

        spimCode->addOperation(line.str());
        line.str("");

        this->children.at(IF_BLOCK)->startGenerate(spimCode);

        line << "b label" << labelUp;
        spimCode->addOperation(line.str());
        line.str("");

        spimCode->addLabel(labelDown);

    }

private:

    void generateConditionForFloat(stringstream & line, string el) {
        line << "l.s $f0, " << el << endl;
        line << "l.s $f2, $f_one" << endl;
        line << "l.s $f3, $f_zero" << endl;
        line << "c.eq.s $f0, $f3" << endl;
        line << "movt.s $f2, $f3" << endl;
        line << "mov.s $f0, $f2 " << endl;
        line << "cvt.w.s $f0, $f0" << endl;
        line << "mfc1 $t1, $f0" << endl;
    }
};

