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
        Type * type2 = spimCode->getVariable(this->var->toString());
        if (this->exp) {
            if (this->exp->isStringExpression()) {
                if (!type2->is(T_STR)) {
                    throw Strings::getIncompatibleTypesText();
                }
                string tmp = spimCode->addTmpStringVar(this->exp->getValueLiteral()->toString());
                //TODO obsluga przypisania do zmiennej string
            } else {
                stringstream line;

                this->exp->startGenerate(spimCode);

                Element * el = this->exp->getValueLiteral();

                if (type2->is(T_STR)) {
                    throw Strings::getIncompatibleTypesText();
                }

                if (type2->is(T_INT)) {
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

                        int * id = ((NameElement*) el)->getArrayId();
                        string idTmp = this->generateIndexComputing(spimCode, line, id);

                        if (type) {
                            if (type->is(T_INT)) {
                                if (id) {
                                    line << "lw $t2, " << idTmp << endl;
                                }
                                line << "lw $t0, " << (id ? "($t2)" : el->toString()) << endl;
                                line << "sw $t0" << ", " << this->var->toString();
                            } else if (type->is(T_FLOAT)) {
                                if (id) {
                                    line << "lw $t2, " << idTmp << endl;
                                }
                                line << "l.s $f0, " << (id ? "($t2)" : el->toString()) << endl;
                                line << "cvt.w.s $f0, $f0" << endl;
                                line << "mfc1 $t0, $f0" << endl;
                                line << "sw " << "$t0" << ", " << this->var->toString();
                            }
                        } else {
                            throw string(Strings::getUndeclaredText(el->toString()));
                        }
                    }

                } else if (type2->is(T_FLOAT)) {

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

                        int * id = ((NameElement*) el)->getArrayId();
                        string idTmp = this->generateIndexComputing(spimCode, line, id);

                        if (type) {
                            if (type->is(T_INT)) {
                                if (id) {
                                    line << "lw $t2, " << idTmp << endl;
                                }
                                line << "lw $t0, " << (id ? "($t2)" : el->toString()) << endl;
                                line << "mtc1 $t0, $f0" << endl;
                                line << "cvt.s.w $f0, $f0" << endl;
                                line << "s.s " << "$f0" << ", " << this->var->toString();
                            } else if (type->is(T_FLOAT)) {
                                if (id) {
                                    line << "lw $t2, " << idTmp << endl;
                                }
                                line << "l.s $f0, " << (id ? "($t2)" : el->toString()) << endl;
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
    }

    string generateIndexComputing(SpimCodeContainer * spimCode, stringstream & line, int * id) {
        string idTmp = "";

        if (id) {
            Element * e = spimCode->mStack.pop();
            Element * m = spimCode->mStack.pop();

            line << "la $t2, " << e->toString() << endl;

            if (ElementUtilities::isInt(m)) {
                line << "li $t4, " << m->toString() << endl;
            } else if (ElementUtilities::isFloat(m)) {
                string tmp = spimCode->addTmpFloatVar(m->toString());
                line << "l.s $f0, " << tmp << endl;
                line << "cvt.w.s $f0, $f0" << endl;
                line << "mfc1 $t4, $f0" << endl;
            } else if (ElementUtilities::isName(m)) {
                Type * type = spimCode->getVariable(m->toString());

                if (type->is(T_INT)) {
                    line << "lw $t4, " << m->toString() << endl;
                } else if (type->is(T_FLOAT)) {
                    line << "l.s $f0, " << m->toString() << endl;
                    line << "cvt.w.s $f0, $f0" << endl;
                    line << "mfc1 $t4, $f0" << endl;
                }
            }
            line << "li $t5, 4" << endl;
            line << "mul $t4, $t4, $t5" << endl;
            line << "add $t2, $t2, $t4" << endl;
            idTmp = spimCode->getNextTmpVar();
            line << "sw $t2, " << idTmp << endl;
            spimCode->addVariable(idTmp, new Type(T_INT));


        }
        return idTmp;
    }
};
