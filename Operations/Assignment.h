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

    void addArrayIndexExpression(Expression * exp) {
        this->arrayIndexExp = exp;
    }

protected:
    Expression * exp, * arrayIndexExp;
    NameElement * var;

    void generate(SpimCodeContainer * spimCode) {

        Type * type2 = spimCode->getVariable(this->var->toString());

        if (this->exp) {
            this->exp->startGenerate(spimCode);
            if (this->exp->isStringExpression()) {
                if (!type2->is(T_STR)) {
                    throw Strings::getIncompatibleTypesText();
                }
                spimCode->changeStrVarValue(this->var->toString(), this->exp->getValueLiteral()->toString());
                //TODO obsluga przypisania do zmiennej string
            } else {
                stringstream line;

                Element * el = this->exp->getValueLiteral();

                Type * type3 = spimCode->getVariable(el->toString());

                if (type3->is(T_STR)) {
                    spimCode->changeStrVarValue(this->var->toString(), spimCode->getStringVarValue(el->toString()));
                } else if (type2->is(T_STR)) {
                    throw Strings::getIncompatibleTypesText();
                }

                bool isArray = this->var->isArray();
                if (isArray) {
                    this->arrayIndexExp->startGenerate(spimCode);
                    this->generateIndexComputing(spimCode, this->arrayIndexExp->getValueLiteral(), line);
                }

                if (type2->is(T_INT)) {

                    if (ElementUtilities::isInt(el)) {
                        line << "li" << " $t0" << ", " << el->toString() << endl;
                        line << "sw " << "$t0" << ", " << (isArray ? "($t6)" : this->var->toString());
                    } else if (ElementUtilities::isFloat(el)) {
                        string tmp = spimCode->addTmpFloatVar(el->toString());
                        line << "l.s $f0, " << tmp << endl;
                        line << "cvt.w.s $f0, $f0" << endl;
                        line << "mfc1 $t0, $f0" << endl;
                        line << "sw " << "$t0" << ", " << (isArray ? "($t6)" : this->var->toString());
                    } else if (ElementUtilities::isName(el)) {
                        Type * type = spimCode->getVariable(el->toString());

                        string * id = ((NameElement*) el)->getArrayId();

                        if (type) {
                            if (type->is(T_INT)) {
                                if (id) {
                                    line << "lw $t2, " << *id << endl;
                                }
                                line << "lw $t0, " << (id ? "($t2)" : el->toString()) << endl;
                                line << "sw $t0" << ", " << (isArray ? "($t6)" : this->var->toString());
                            } else if (type->is(T_FLOAT)) {
                                if (id) {
                                    line << "lw $t2, " << *id << endl;
                                }
                                line << "l.s $f0, " << (id ? "($t2)" : el->toString()) << endl;
                                line << "cvt.w.s $f0, $f0" << endl;
                                line << "mfc1 $t0, $f0" << endl;
                                line << "sw " << "$t0" << ", " << (isArray ? "($t6)" : this->var->toString());
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
                        line << "s.s " << "$f0" << ", " << (isArray ? "($t6)" : this->var->toString());
                    } else if (ElementUtilities::isFloat(el)) {
                        string tmp = spimCode->addTmpFloatVar(el->toString());
                        line << "l.s" << " $f0" << ", " << tmp << endl;
                        line << "s.s " << "$f0" << ", " << (isArray ? "($t6)" : this->var->toString());
                    } else if (ElementUtilities::isName(el)) {
                        Type * type = spimCode->getVariable(el->toString());

                        string * id = ((NameElement*) el)->getArrayId();

                        if (type) {
                            if (type->is(T_INT)) {
                                if (id) {
                                    line << "lw $t2, " << *id << endl;
                                }
                                line << "lw $t0, " << (id ? "($t2)" : el->toString()) << endl;
                                line << "mtc1 $t0, $f0" << endl;
                                line << "cvt.s.w $f0, $f0" << endl;
                                line << "s.s " << "$f0" << ", " << (isArray ? "($t6)" : this->var->toString());
                            } else if (type->is(T_FLOAT)) {
                                if (id) {
                                    line << "lw $t2, " << *id << endl;
                                }
                                line << "l.s $f0, " << (id ? "($t2)" : el->toString()) << endl;
                                line << "s.s " << "$f0" << ", " << (isArray ? "($t6)" : this->var->toString());
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
