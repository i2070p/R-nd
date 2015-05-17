#pragma once

#include "SimpleOperation.h"
#include <stack>
#include "../Elements/LiteralElement.h"
#include "../StackAdapter.h"
#include <sstream>
#include "../Strings.h"

using namespace std;

class Expression : public SimpleOperation {
public:

    Element * getValueLiteral() {

        return this->result;
    }

    Expression(Operation * parent) : SimpleOperation(parent) {

    }

    Expression(Operation * parent, Element* element) : SimpleOperation(parent) {
        this->elements.push(element);
    }

    void addElement(Element* element) {
        this->elements.push(element);
    }

    bool isStringExpression() {
        return this->elements.size() == 1 && ElementUtilities::isString(this->elements.at(0));
    }

protected:
    StackAdapter<Element*> elements;
    Element * result;
    void generate(SpimCodeContainer * spimCode) {

        if (this->isStringExpression()) {

        } else {
            for (int i = 0; i<this->elements.size(); i++) {

                if (ElementUtilities::isString(this->elements.at(i))) {
                    throw string(Strings::getIncorrectExpressionText(this->elements.at(i)->toString()));
                } else {
                    if (ElementUtilities::isSign(this->elements.at(i))) {
                        compute((SignElement*) this->elements.at(i), spimCode);
                    } else {
                        spimCode->mStack.push(this->elements.at(i));
                    }
                }
            }
            this->result = spimCode->mStack.top();
        }
    }

    void compute(SignElement * sign, SpimCodeContainer * spimCode) {
        Element* second = spimCode->mStack.pop();
        Element* first = spimCode->mStack.pop();

        this->generateInstructions(spimCode, sign, first, second);
    }

    string generateInstructions(SpimCodeContainer * spimCode, SignElement* sign, Element * e1, Element * e2) {
        stringstream line;

        string tmp1 = spimCode->getNextTmpVar();

        if (ElementUtilities::isInt(e1) && ElementUtilities::isInt(e2)) {

            line << "li $t0, " << e1->toString() << endl;
            line << "li $t1, " << e2->toString() << endl;
            line << sign->getCommand(T_INT) << endl;
            line << "sw $t0, " << tmp1;
            spimCode->addOperation(line.str());
            spimCode->addVariable(tmp1, new Type(T_INT));
            spimCode->mStack.push(ElementFactory::createElement(tmp1));
        } else if (ElementUtilities::isInt(e1) && ElementUtilities::isFloat(e2)) {
            string tmp2 = spimCode->addTmpFloatVar(e2->toString());

            line << "li $t0, " << e1->toString() << endl;
            line << "mtc1 $t0, $f0" << endl;
            line << "cvt.s.w $f0, $f0" << endl;
            line << "l.s $f1, " << tmp2 << endl;
            line << sign->getCommand() << endl;
            line << "s.s $f0, " << tmp1;

            spimCode->addOperation(line.str());

            spimCode->addVariable(tmp1, new Type(T_FLOAT));
            spimCode->mStack.push(ElementFactory::createElement(tmp1));

        } else if (ElementUtilities::isFloat(e1) && ElementUtilities::isInt(e2)) {
            string tmp2 = spimCode->addTmpFloatVar(e1->toString());

            line << "li $t0, " << e2->toString() << endl;
            line << "mtc1 $t0, $f0" << endl;
            line << "cvt.s.w $f0, $f0" << endl;
            line << "l.s $f1, " << tmp2 << endl;
            line << sign->getCommand() << endl;
            line << "s.s $f0, " << tmp1;

            spimCode->addOperation(line.str());

            spimCode->addVariable(tmp1, new Type(T_FLOAT));
            spimCode->mStack.push(ElementFactory::createElement(tmp1));
        } else if (ElementUtilities::isFloat(e1) && ElementUtilities::isFloat(e2)) {
            string tmp2 = spimCode->addTmpFloatVar(e1->toString());
            string tmp3 = spimCode->addTmpFloatVar(e2->toString());


            line << "l.s $f0, " << tmp2 << endl;
            line << "l.s $f1, " << tmp3 << endl;
            line << sign->getCommand() << endl;
            line << "s.s $f0, " << tmp1;

            spimCode->addOperation(line.str());

            spimCode->addVariable(tmp1, new Type(T_FLOAT));
            spimCode->mStack.push(ElementFactory::createElement(tmp1));
        } else if (ElementUtilities::isName(e1)) {
            Type * type = spimCode->getVariable(e1->toString());

            int * id = ((NameElement*) e1)->getArrayId();
            string idTmp = this->generateIndexComputing(spimCode, e1, line, id);

            if (type) {

                if (type->is(T_INT)) {
                    if (ElementUtilities::isInt(e2)) {
                        if (id) {
                            line << "lw $t2, " << idTmp << endl;
                        }
                        line << "lw $t0, " << (id ? "($t2)" : e1->toString()) << endl;
                        line << "li $t1, " << e2->toString() << endl;
                        line << sign->getCommand(T_INT) << endl;
                        line << "sw $t0, " << tmp1;
                        spimCode->addOperation(line.str());
                        spimCode->addVariable(tmp1, new Type(T_INT));
                        spimCode->mStack.push(ElementFactory::createElement(tmp1));
                    } else if (ElementUtilities::isFloat(e2)) {
                        string tmp2 = spimCode->addTmpFloatVar(e2->toString());

                        if (id) {
                            line << "lw $t2, " << idTmp << endl;
                        }
                        line << "lw $t0, " << (id ? "($t2)" : e1->toString()) << endl;
                        line << "mtc1 $t0, $f0" << endl;
                        line << "cvt.s.w $f0, $f0" << endl;
                        line << "l.s $f1, " << tmp2 << endl;
                        line << sign->getCommand() << endl;
                        line << "s.s $f0, " << tmp1;

                        spimCode->addOperation(line.str());

                        spimCode->addVariable(tmp1, new Type(T_FLOAT));
                        spimCode->mStack.push(ElementFactory::createElement(tmp1));
                    } else if (ElementUtilities::isName(e2)) {
                        Type * type2 = spimCode->getVariable(e2->toString());

                        int * id2 = ((NameElement*) e1)->getArrayId();
                        string idTmp2 = this->generateIndexComputing(spimCode, e2, line, id2);

                        if (type2) {
                            if (type2->is(T_INT)) {

                                if (id) {
                                    line << "lw $t2, " << idTmp << endl;
                                }
                                line << "lw $t0, " << (id ? "($t2)" : e1->toString()) << endl;
                                if (id2) {
                                    line << "lw $t2, " << idTmp2 << endl;
                                }
                                line << "lw $t1, " << (id2 ? "($t2)" : e2->toString()) << endl;
                                line << sign->getCommand(T_INT) << endl;
                                line << "sw $t0, " << tmp1;
                                spimCode->addOperation(line.str());
                                spimCode->addVariable(tmp1, new Type(T_INT));
                                spimCode->mStack.push(ElementFactory::createElement(tmp1));
                            } else if (type2->is(T_FLOAT)) {

                                if (id) {
                                    line << "lw $t2, " << idTmp << endl;
                                }
                                line << "lw $t0, " << (id ? "($t2)" : e1->toString()) << endl;
                                line << "mtc1 $t0, $f0" << endl;
                                line << "cvt.s.w $f0, $f0" << endl;
                                if (id2) {
                                    line << "lw $t2, " << idTmp2 << endl;
                                }
                                line << "l.s $f1, " << (id2 ? "($t2)" : e2->toString()) << endl;
                                line << sign->getCommand() << endl;
                                line << "s.s $f0, " << tmp1;

                                spimCode->addOperation(line.str());

                                spimCode->addVariable(tmp1, new Type(T_FLOAT));
                                spimCode->mStack.push(ElementFactory::createElement(tmp1));
                            }
                        }
                    }
                } else if (type->is(T_FLOAT)) {
                    if (ElementUtilities::isInt(e2)) {
                        line << "li $t0, " << e2->toString() << endl;
                        line << "mtc1 $t0, $f1" << endl;
                        line << "cvt.s.w $f1, $f1" << endl;
                        if (id) {
                            line << "lw $t2, " << idTmp << endl;
                        }
                        line << "l.s $f0, " << (id ? "($t2)" : e1->toString()) << endl;
                        line << sign->getCommand() << endl;
                        line << "s.s $f0, " << tmp1;

                        spimCode->addOperation(line.str());

                        spimCode->addVariable(tmp1, new Type(T_FLOAT));
                        spimCode->mStack.push(ElementFactory::createElement(tmp1));
                    } else if (ElementUtilities::isFloat(e2)) {
                        string tmp2 = spimCode->addTmpFloatVar(e2->toString());


                        if (id) {
                            line << "lw $t2, " << idTmp << endl;
                        }
                        line << "l.s $f0, " << (id ? "($t2)" : e1->toString()) << endl;
                        line << "l.s $f1, " << tmp2 << endl;
                        line << sign->getCommand() << endl;
                        line << "s.s $f0, " << tmp1;

                        spimCode->addOperation(line.str());

                        spimCode->addVariable(tmp1, new Type(T_FLOAT));
                        spimCode->mStack.push(ElementFactory::createElement(tmp1));
                    } else if (ElementUtilities::isName(e2)) {
                        Type * type2 = spimCode->getVariable(e2->toString());

                        int * id2 = ((NameElement*) e1)->getArrayId();
                        string idTmp2 = this->generateIndexComputing(spimCode, e2, line, id2);

                        if (type2) {
                            if (type2->is(T_INT)) {
                                if (id2) {
                                    line << "lw $t2, " << idTmp2 << endl;
                                }
                                line << "lw $t0, " << (id2 ? "($t2)" : e2->toString()) << endl;
                                line << "mtc1 $t0, $f1" << endl;
                                line << "cvt.s.w $f1, $f1" << endl;
                                if (id) {
                                    line << "lw $t2, " << idTmp << endl;
                                }
                                line << "l.s $f0, " << (id ? "($t2)" : e1->toString()) << endl;
                                line << sign->getCommand() << endl;
                                line << "s.s $f0, " << tmp1;

                                spimCode->addOperation(line.str());

                                spimCode->addVariable(tmp1, new Type(T_FLOAT));
                                spimCode->mStack.push(ElementFactory::createElement(tmp1));
                            } else if (type2->is(T_FLOAT)) {


                                if (id) {
                                    line << "lw $t2, " << idTmp << endl;
                                }
                                line << "l.s $f0, " << (id ? "($t2)" : e1->toString()) << endl;
                                if (id2) {
                                    line << "lw $t2, " << idTmp2 << endl;
                                }
                                line << "l.s $f1, " << (id2 ? "($t2)" : e2->toString()) << endl;
                                line << sign->getCommand() << endl;
                                line << "s.s $f0, " << tmp1;

                                spimCode->addOperation(line.str());

                                spimCode->addVariable(tmp1, new Type(T_FLOAT));
                                spimCode->mStack.push(ElementFactory::createElement(tmp1));
                            }
                        }
                    }
                }
            }
        } else if (ElementUtilities::isName(e2)) {
            Type * type = spimCode->getVariable(e2->toString());

            int * id = ((NameElement*) e1)->getArrayId();
            string idTmp = this->generateIndexComputing(spimCode, e1, line, id);

            if (type) {
                if (type->is(T_INT)) {
                    if (ElementUtilities::isInt(e1)) {
                        line << "li $t0, " << e1->toString() << endl;
                        if (id) {
                            line << "lw $t2, " << idTmp << endl;
                        }
                        line << "lw $t1, " << (id ? "($t2)" : e2->toString()) << endl;
                        line << sign->getCommand(T_INT) << endl;
                        line << "sw $t0, " << tmp1;
                        spimCode->addOperation(line.str());
                        spimCode->addVariable(tmp1, new Type(T_INT));
                        spimCode->mStack.push(ElementFactory::createElement(tmp1));
                    } else if (ElementUtilities::isFloat(e1)) {
                        string tmp2 = spimCode->addTmpFloatVar(e1->toString());


                        if (id) {
                            line << "lw $t2, " << idTmp << endl;
                        }
                        line << "lw $t0, " << (id ? "($t2)" : e2->toString()) << endl;
                        line << "mtc1 $t0, $f1" << endl;
                        line << "cvt.s.w $f1, $f1" << endl;

                        line << "l.s $f0, " << tmp2 << endl;
                        line << sign->getCommand() << endl;
                        line << "s.s $f0, " << tmp1;

                        spimCode->addOperation(line.str());

                        spimCode->addVariable(tmp1, new Type(T_FLOAT));
                        spimCode->mStack.push(ElementFactory::createElement(tmp1));
                    } else if (ElementUtilities::isName(e1)) {
                        Type * type2 = spimCode->getVariable(e1->toString());
                        int * id2 = ((NameElement*) e1)->getArrayId();
                        string idTmp2 = this->generateIndexComputing(spimCode, e2, line, id2);

                        if (type2) {
                            if (type2->is(T_INT)) {
                                if (id2) {
                                    line << "lw $t2, " << idTmp2 << endl;
                                }
                                line << "lw $t0, " << (id2 ? "($t2)" : e1->toString()) << endl;

                                if (id) {
                                    line << "lw $t2, " << idTmp << endl;
                                }
                                line << "lw $t1, " << (id ? "($t2)" : e2->toString()) << endl;
                                line << sign->getCommand(T_INT) << endl;
                                line << "sw $t0, " << tmp1;
                                spimCode->addOperation(line.str());
                                spimCode->addVariable(tmp1, new Type(T_INT));
                                spimCode->mStack.push(ElementFactory::createElement(tmp1));
                            } else if (type2->is(T_FLOAT)) {
                                if (id) {
                                    line << "lw $t2, " << idTmp << endl;
                                }
                                line << "lw $t0, " << (id ? "($t2)" : e2->toString()) << endl;
                                line << "mtc1 $t0, $f1" << endl;
                                line << "cvt.s.w $f1, $f1" << endl;
                                if (id2) {
                                    line << "lw $t2, " << idTmp2 << endl;
                                }
                                line << "l.s $f0, " << (id2 ? "($t2)" : e1->toString()) << endl;
                                line << sign->getCommand() << endl;
                                line << "s.s $f0, " << tmp1;

                                spimCode->addOperation(line.str());

                                spimCode->addVariable(tmp1, new Type(T_FLOAT));
                                spimCode->mStack.push(ElementFactory::createElement(tmp1));
                            }
                        }
                    }
                } else if (type->is(T_FLOAT)) {
                    if (ElementUtilities::isInt(e1)) {

                        line << "li $t0, " << e1->toString() << endl;
                        line << "mtc1 $t0, $f0" << endl;
                        line << "cvt.s.w $f0, $f0" << endl;
                        if (id) {
                            line << "lw $t2, " << idTmp << endl;
                        }
                        line << "l.s $f1, " << (id ? "($t2)" : e2->toString()) << endl;
                        line << sign->getCommand() << endl;
                        line << "s.s $f0, " << tmp1;

                        spimCode->addOperation(line.str());

                        spimCode->addVariable(tmp1, new Type(T_FLOAT));
                        spimCode->mStack.push(ElementFactory::createElement(tmp1));
                    } else if (ElementUtilities::isFloat(e1)) {
                        string tmp2 = spimCode->addTmpFloatVar(e1->toString());

                        if (id) {
                            line << "lw $t2, " << idTmp << endl;
                        }
                        line << "l.s $f1, " << (id ? "($t2)" : e2->toString()) << endl;
                        line << "l.s $f0, " << tmp2 << endl;
                        line << sign->getCommand() << endl;
                        line << "s.s $f0, " << tmp1;

                        spimCode->addOperation(line.str());

                        spimCode->addVariable(tmp1, new Type(T_FLOAT));
                        spimCode->mStack.push(ElementFactory::createElement(tmp1));
                    } else if (ElementUtilities::isName(e1)) {
                        Type * type2 = spimCode->getVariable(e1->toString());

                        int * id2 = ((NameElement*) e1)->getArrayId();
                        string idTmp2 = this->generateIndexComputing(spimCode, e2, line, id2);

                        if (type2) {
                            if (type2->is(T_INT)) {
                                if (id2) {
                                    line << "lw $t2, " << idTmp2 << endl;
                                }
                                line << "lw $t0, " << (id2 ? "($t2)" : e1->toString()) << endl;
                                line << "mtc1 $t0, $f0" << endl;
                                line << "cvt.s.w $f0, $f0" << endl;

                                if (id) {
                                    line << "lw $t2, " << idTmp << endl;
                                }
                                line << "l.s $f1, " << (id ? "($t2)" : e2->toString()) << endl;

                                line << sign->getCommand() << endl;
                                line << "s.s $f0, " << tmp1;

                                spimCode->addOperation(line.str());

                                spimCode->addVariable(tmp1, new Type(T_FLOAT));
                                spimCode->mStack.push(ElementFactory::createElement(tmp1));
                            } else if (type2->is(T_FLOAT)) {


                                if (id) {
                                    line << "lw $t2, " << idTmp << endl;
                                }
                                line << "l.s $f1, " << (id ? "($t2)" : e2->toString()) << endl;

                                if (id2) {
                                    line << "lw $t2, " << idTmp2 << endl;
                                }
                                line << "l.s $f0, " << (id2 ? "($t2)" : e1->toString()) << endl;

                                line << sign->getCommand() << endl;
                                line << "s.s $f0, " << tmp1;

                                spimCode->addOperation(line.str());

                                spimCode->addVariable(tmp1, new Type(T_FLOAT));
                                spimCode->mStack.push(ElementFactory::createElement(tmp1));
                            }
                        }
                    }
                }
            }
        }
        return line.str();
    }

    string generateIndexComputing(SpimCodeContainer * spimCode, Element* e, stringstream & line, int * id) {
        string idTmp = "";

        if (id) {
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

