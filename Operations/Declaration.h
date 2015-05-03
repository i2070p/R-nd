#pragma once

#include "SimpleOperation.h"
#include "../Elements/NameElement.h"
#include "../StackAdapter.h"
#include "Type.h"
#include "Expression.h"
#include "../Strings.h"
using namespace std;

class Declaration : public SimpleOperation {
public:

    Declaration(Operation * parent, NameElement* var, Type * type) : SimpleOperation(parent) {
        this->var = var;
        this->type = type;
    }

    void addExpression(Expression* exp) {
        this->exp = exp;
    }

protected:
    Expression * exp;
    NameElement * var;
    Type * type;

    void generate(SpimCodeContainer * spimCode) {
        spimCode->addVariable(this->var->toString(), this->type);
        if (this->exp) {
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
                            line << "l.s" << " $f0" << ", " << el->toString() << endl;
                            line << "s.s " << "$f0" << ", " << this->var->toString();
                        }
                    } else {
                        throw string(Strings::getUndeclaredText(el->toString()));
                    }
                }

            }
            spimCode->addOperation(line.str());
        }
    }
};

