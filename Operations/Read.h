#pragma once

#include "SimpleOperation.h"
#include <stack>
#include "../Elements/NameElement.h"
#include "../StackAdapter.h"
#include "Type.h"
#include "Expression.h"
#include <sstream>

using namespace std;

class Read : public SimpleOperation {
public:

    Read(Operation * parent, NameElement * var) : SimpleOperation(parent) {
        this->var = var;
    }

    void addArrayIndexExpression(Expression * exp) {
        this->arrayIndexExp = exp;
    }

protected:
    NameElement * var;
    Expression * arrayIndexExp;

    void generate(SpimCodeContainer * spimCode) {
        stringstream line;
        string var = this->var->toString();
        Type * type = spimCode->getVariable(var);

        bool isArray = this->var->isArray();
        if (isArray) {
            this->arrayIndexExp->startGenerate(spimCode);
            this->generateIndexComputing(spimCode, this->arrayIndexExp->getValueLiteral(), line);
        }

        if (type) {
            if (type->is(T_STR)) {

                //TODO opzłóka dżydania zdrinka

            } else if (type->is(T_INT)) {
                line << "li $v0, 5" << endl;
                line << "syscall" << endl;
                line << "move $t0, $v0" << endl;
                line << "sw $t0, " << (isArray ? "($t6)" : var);
            } else if (type->is(T_FLOAT)) {
                line << "li $v0, 6" << endl;
                line << "syscall" << endl;
                line << "s.s $f0, " << (isArray ? "($t6)" : var);
            }
            spimCode->addOperation(line.str());
        } 
    }

private:

    void generateIndexComputing(SpimCodeContainer * spimCode, Element * m, stringstream & line) {

        line << "la $t2, " << this->var->toString() << endl;

        if (ElementUtilities::isInt(m)) {
            line << "li $t4, " << m->toString() << endl;
        } else if (ElementUtilities::isFloat(m)) {
            string tmp = spimCode->addTmpFloatVar(m->toString());
            line << "l.s $f0, " << tmp << endl;
            line << "cvt.w.s $f0, $f0" << endl;
            line << "mfc1 $t4, $f0" << endl;
        } else if (ElementUtilities::isName(m)) {
            Type * type = spimCode->getVariable(m->toString());
            string id = m->toString();
            if (((NameElement*) m)->isArray()) {
                id = *((NameElement*) m)->getArrayId();
            }
            if (type->is(T_INT)) {
                line << "lw $t4, " << id << endl;
            } else if (type->is(T_FLOAT)) {
                line << "l.s $f0, " << id << endl;
                line << "cvt.w.s $f0, $f0" << endl;
                line << "mfc1 $t4, $f0" << endl;
            }
        }

        line << "li $t5, 4" << endl;
        line << "mul $t4, $t4, $t5" << endl;
        line << "add $t6, $t2, $t4" << endl;
    }
};
