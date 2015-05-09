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

