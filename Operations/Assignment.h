#pragma once

#include "SimpleOperation.h"
#include <stack>
#include "../Elements/NameElement.h"
#include "../StackAdapter.h"
#include "Type.h"
#include "Expression.h"
#include <sstream>

using namespace std;

class Assignment : public SimpleOperation {
public:

    Assignment(Operation * parent, NameElement* var) : SimpleOperation(parent) {
        this->var = var;
    }

    void addExpression(Expression* exp) {
        this->exp = exp;
    }

protected:
    Expression * exp;
    NameElement * var;

    void generate(SpimCodeContainer * spimCode) {

        this->exp->startGenerate(spimCode);

        Element * el = this->exp->getValueLiteral();

        stringstream line;
        this->exp->startGenerate(spimCode);
        Element * el = this->exp->getValueLiteral();

        if (this->type->is(T_INT)) {
            if (ElementUtilities::isInt(el)) {
                line << "li" << " $t0" << ", " << el->toString() << endl;
                line << "sw " << "$t0" << ", " << this->var->toString();
            } else if (ElementUtilities::isFloat(el)) {
                line << "l.s $f0, " << el->toString() << endl;
                line << "cvt.w.s $f0, $f0" << endl;
                line << "mfc1 $t0, $f0" << endl;
                line << "sw " << "$t0" << ", " << this->var->toString();
            } else if (ElementUtilities::isName(el)) {
                Type * type = spimCode->getVariable(el->toString());

                if (type) {
                    cout << el->toString() << endl;
                    if (type->is(T_INT)) {
                        line << "lw" << " $t0" << ", " << el->toString() << endl;
                        line << "sw " << "$t0" << ", " << this->var->toString();
                    } else if (type->is(T_FLOAT)) {
                        line << "l.s $f0, " << el->toString() << endl;
                        line << "cvt.w.s $f0, $f0" << endl;
                        line << "mfc1 $t0, $f0" << endl;
                        line << "sw " << "$t0" << ", " << this->var->toString();
                    }
                } else {
                    throw string(Strings::getUndeclaredText(el->toString()));
                }
            }

        } else if (this->type->is(T_FLOAT)) {

            if (ElementUtilities::isInt(el)) {
                line << "li $t0, " << el->toString() << endl;
                line << "mtc1 $t0, $f0" << endl;
                line << "cvt.s.w $f0, $f0" << endl;
                line << "s.s " << "$f0" << ", " << this->var->toString();
            } else if (ElementUtilities::isFloat(el)) {
                string tmp = spimCode->addTmpFloatVar(el->toString());
                line << "l.s" << " $f0" << ", " << tmp << endl;
                line << "s.s " << "$f0" << ", " << this->var->toString();
            } else if (ElementUtilities::isName(el)) {
                Type * type = spimCode->getVariable(el->toString());

                if (type) {
                    if (type->is(T_INT)) {
                        line << "lw $t0, " << el->toString() << endl;
                        line << "mtc1 $t0, $f0" << endl;
                        line << "cvt.s.w $f0, $f0" << endl;
                        line << "s.s " << "$f0" << ", " << this->var->toString();
                    } else if (type->is(T_FLOAT)) {
                        string tmp = spimCode->addTmpFloatVar(el->toString());
                        line << "l.s" << " $f0" << ", " << tmp << endl;
                        line << "s.s " << "$f0" << ", " << this->var->toString();
                    }
                } else {
                    throw string(Strings::getUndeclaredText(el->toString()));
                }
            }
        }
        spimCode->addOperation(line.str());
    };

