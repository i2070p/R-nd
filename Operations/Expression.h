#pragma once

#include "SimpleOperation.h"
#include <stack>
#include "../Elements/LiteralElement.h"
#include "../StackAdapter.h"
#include <sstream>

using namespace std;

class Expression : public SimpleOperation {
public:

    Element * getValueLiteral() {
        return this->elements.pop();
    }

    Expression(Operation * parent) : SimpleOperation(parent) {

    }

    Expression(Operation * parent, Element* element) : SimpleOperation(parent) {
        this->elements.push(element);
    }

    void addElement(Element* element) {
        this->elements.push(element);
    }

protected:
    StackAdapter<Element*> elements;

    void generate(SpimCodeContainer * spimCode) {
        bool change = true;

        while (change) {
            change = false;
            for (int i = 0; i<this->elements.size(); i++) {
                if (dynamic_cast<SignElement *> (this->elements.at(i))) {

                    compute((SignElement*) this->elements.getAndErase(i), spimCode, i);
                    change = true;
                    break;
                }
            }
        }
    }

    void compute(SignElement * sign, SpimCodeContainer * spimCode, int i) {


        Element* second = this->elements.getAndErase(i - 1);
        Element* first = this->elements.getAndErase(i - 2);
        
        cout << sign->toString() << ", first: " << first->toString() << ", second: " << second->toString() << endl;


        this->generateInstructions(spimCode, sign, first, second, i-2);

    }

    string generateInstructions(SpimCodeContainer * spimCode, SignElement* sign, Element * e1, Element * e2, int i) {
        stringstream line;

        string tmp1 = spimCode->getNextTmpVar();

        if (ElementUtilities::isInt(e1) && ElementUtilities::isInt(e2)) {

            line << "li $t0, " << e1->toString() << endl;
            line << "li $t1, " << e2->toString() << endl;
            line << sign->getCommand(T_INT) << endl;
            line << "sw $t0, " << tmp1;
            spimCode->addOperation(line.str());

            spimCode->addVariable(tmp1, new Type(T_INT));
            this->elements.insert(ElementFactory::createElement(tmp1), i);
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
            this->elements.insert(ElementFactory::createElement(tmp1), i);

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
            this->elements.insert(ElementFactory::createElement(tmp1), i);
        } else if (ElementUtilities::isFloat(e1) && ElementUtilities::isFloat(e2)) {
            string tmp2 = spimCode->addTmpFloatVar(e1->toString());
            string tmp3 = spimCode->addTmpFloatVar(e2->toString());


            line << "l.s $f0, " << tmp2 << endl;
            line << "l.s $f1, " << tmp3 << endl;
            line << sign->getCommand() << endl;
            line << "s.s $f0, " << tmp1;

            spimCode->addOperation(line.str());

            spimCode->addVariable(tmp1, new Type(T_FLOAT));
            this->elements.insert(ElementFactory::createElement(tmp1), i);
        } else if (ElementUtilities::isName(e1)) {
            Type * type = spimCode->getVariable(e1->toString());
            
            if (type) {
                if (type->is(T_INT)) {
                    if (ElementUtilities::isInt(e2)) {
                        line << "lw $t0, " << e1->toString() << endl;
                        line << "li $t1, " << e2->toString() << endl;
                        line << sign->getCommand(T_INT) << endl;
                        line << "sw $t0, " << tmp1;
                        spimCode->addOperation(line.str());
                        spimCode->addVariable(tmp1, new Type(T_INT));
                        this->elements.insert(ElementFactory::createElement(tmp1), i);
                    } else if (ElementUtilities::isFloat(e2)) {
                        string tmp2 = spimCode->addTmpFloatVar(e2->toString());

                        line << "lw $t0, " << e1->toString() << endl;
                        line << "mtc1 $t0, $f0" << endl;
                        line << "cvt.s.w $f0, $f0" << endl;
                        line << "l.s $f1, " << tmp2 << endl;
                        line << sign->getCommand() << endl;
                        line << "s.s $f0, " << tmp1;

                        spimCode->addOperation(line.str());

                        spimCode->addVariable(tmp1, new Type(T_FLOAT));
                        this->elements.insert(ElementFactory::createElement(tmp1), i);
                    } else if (ElementUtilities::isName(e2)) {
                        Type * type2 = spimCode->getVariable(e2->toString());

                        if (type2) {
                            if (type2->is(T_INT)) {

                                line << "lw $t0, " << e1->toString() << endl;
                                line << "lw $t1, " << e2->toString() << endl;
                                line << sign->getCommand(T_INT) << endl;
                                line << "sw $t0, " << tmp1;
                                spimCode->addOperation(line.str());
                                spimCode->addVariable(tmp1, new Type(T_INT));
                                this->elements.insert(ElementFactory::createElement(tmp1), i);  
                            } else if (type2->is(T_FLOAT)) {

                                line << "lw $t0, " << e1->toString() << endl;
                                line << "mtc1 $t0, $f0" << endl;
                                line << "cvt.s.w $f0, $f0" << endl;
                                line << "l.s $f1, " << e2->toString() << endl;
                                line << sign->getCommand() << endl;
                                line << "s.s $f0, " << tmp1;

                                spimCode->addOperation(line.str());

                                spimCode->addVariable(tmp1, new Type(T_FLOAT));
                                this->elements.insert(ElementFactory::createElement(tmp1), i);
                            }
                        }
                    }
                } else if (type->is(T_FLOAT)) {
                    if (ElementUtilities::isInt(e2)) {
                        line << "li $t0, " << e2->toString() << endl;
                        line << "mtc1 $t0, $f1" << endl;
                        line << "cvt.s.w $f1, $f1" << endl;
                        line << "l.s $f0, " << e1->toString() << endl;
                        line << sign->getCommand() << endl;
                        line << "s.s $f0, " << tmp1;

                        spimCode->addOperation(line.str());

                        spimCode->addVariable(tmp1, new Type(T_FLOAT));
                        this->elements.insert(ElementFactory::createElement(tmp1), i);
                    } else if (ElementUtilities::isFloat(e2)) {
                        string tmp2 = spimCode->addTmpFloatVar(e2->toString());


                        line << "l.s $f0, " << e1->toString() << endl;
                        line << "l.s $f1, " << tmp2 << endl;
                        line << sign->getCommand() << endl;
                        line << "s.s $f0, " << tmp1;

                        spimCode->addOperation(line.str());

                        spimCode->addVariable(tmp1, new Type(T_FLOAT));
                        this->elements.insert(ElementFactory::createElement(tmp1), i);
                    } else if (ElementUtilities::isName(e2)) {
                        Type * type2 = spimCode->getVariable(e2->toString());
                        if (type2) {
                            if (type2->is(T_INT)) {

                                line << "lw $t0, " << e2->toString() << endl;
                                line << "mtc1 $t0, $f1" << endl;
                                line << "cvt.s.w $f1, $f1" << endl;
                                line << "l.s $f0, " << e1->toString() << endl;
                                line << sign->getCommand() << endl;
                                line << "s.s $f0, " << tmp1;

                                spimCode->addOperation(line.str());

                                spimCode->addVariable(tmp1, new Type(T_FLOAT));
                                this->elements.insert(ElementFactory::createElement(tmp1), i);
                            } else if (type2->is(T_FLOAT)) {


                                line << "l.s $f0, " << e1->toString() << endl;
                                line << "l.s $f1, " << e2->toString() << endl;
                                line << sign->getCommand() << endl;
                                line << "s.s $f0, " << tmp1;

                                spimCode->addOperation(line.str());

                                spimCode->addVariable(tmp1, new Type(T_FLOAT));
                                this->elements.insert(ElementFactory::createElement(tmp1), i);
                            }
                        }
                    }
                }
            }
        } else if (ElementUtilities::isName(e2)) {
            Type * type = spimCode->getVariable(e2->toString());

            if (type) {
                if (type->is(T_INT)) {
                    if (ElementUtilities::isInt(e1)) {
                        line << "li $t0, " << e1->toString() << endl;
                        line << "lw $t1, " << e2->toString() << endl;
                        line << sign->getCommand(T_INT) << endl;
                        line << "sw $t0, " << tmp1;
                        spimCode->addOperation(line.str());
                        spimCode->addVariable(tmp1, new Type(T_INT));
                        this->elements.insert(ElementFactory::createElement(tmp1), i);
                    } else if (ElementUtilities::isFloat(e1)) {
                        string tmp2 = spimCode->addTmpFloatVar(e1->toString());

                        line << "lw $t0, " << e2->toString() << endl;
                        line << "mtc1 $t0, $f1" << endl;
                        line << "cvt.s.w $f1, $f1" << endl;
                        line << "l.s $f0, " << tmp2 << endl;
                        line << sign->getCommand() << endl;
                        line << "s.s $f0, " << tmp1;

                        spimCode->addOperation(line.str());

                        spimCode->addVariable(tmp1, new Type(T_FLOAT));
                        this->elements.insert(ElementFactory::createElement(tmp1), i);
                    } else if (ElementUtilities::isName(e1)) {
                        Type * type2 = spimCode->getVariable(e1->toString());
                        if (type2) {
                            if (type2->is(T_INT)) {
                                line << "lw $t0, " << e1->toString() << endl;
                                line << "lw $t1, " << e2->toString() << endl;
                                line << sign->getCommand(T_INT) << endl;
                                line << "sw $t0, " << tmp1;
                                spimCode->addOperation(line.str());
                                spimCode->addVariable(tmp1, new Type(T_INT));
                                this->elements.insert(ElementFactory::createElement(tmp1), i);
                            } else if (type2->is(T_FLOAT)) {
                                line << "lw $t0, " << e2->toString() << endl;
                                line << "mtc1 $t0, $f1" << endl;
                                line << "cvt.s.w $f1, $f1" << endl;
                                line << "l.s $f0, " << e1->toString() << endl;
                                line << sign->getCommand() << endl;
                                line << "s.s $f0, " << tmp1;

                                spimCode->addOperation(line.str());

                                spimCode->addVariable(tmp1, new Type(T_FLOAT));
                                this->elements.insert(ElementFactory::createElement(tmp1), i);
                            }
                        }
                    }
                } else if (type->is(T_FLOAT)) {
                    if (ElementUtilities::isInt(e1)) {

                        line << "li $t0, " << e1->toString() << endl;
                        line << "mtc1 $t0, $f0" << endl;
                        line << "cvt.s.w $f0, $f0" << endl;
                        line << "l.s $f1, " << e2->toString() << endl;
                        line << sign->getCommand() << endl;
                        line << "s.s $f0, " << tmp1;

                        spimCode->addOperation(line.str());

                        spimCode->addVariable(tmp1, new Type(T_FLOAT));
                        this->elements.insert(ElementFactory::createElement(tmp1), i);
                    } else if (ElementUtilities::isFloat(e1)) {
                        string tmp2 = spimCode->addTmpFloatVar(e1->toString());


                        line << "l.s $f1, " << e2->toString() << endl;
                        line << "l.s $f0, " << tmp2 << endl;
                        line << sign->getCommand() << endl;
                        line << "s.s $f0, " << tmp1;

                        spimCode->addOperation(line.str());

                        spimCode->addVariable(tmp1, new Type(T_FLOAT));
                        this->elements.insert(ElementFactory::createElement(tmp1), i);
                    } else if (ElementUtilities::isName(e1)) {
                        Type * type2 = spimCode->getVariable(e1->toString());
                        if (type2) {
                            if (type2->is(T_INT)) {

                                line << "lw $t0, " << e1->toString() << endl;
                                line << "mtc1 $t0, $f0" << endl;
                                line << "cvt.s.w $f0, $f0" << endl;
                                line << "l.s $f1, " << e2->toString() << endl;
                                line << sign->getCommand() << endl;
                                line << "s.s $f0, " << tmp1;

                                spimCode->addOperation(line.str());

                                spimCode->addVariable(tmp1, new Type(T_FLOAT));
                                this->elements.insert(ElementFactory::createElement(tmp1), i);
                            } else if (type2->is(T_FLOAT)) {


                                line << "l.s $f1, " << e2->toString() << endl;
                                line << "l.s $f0, " << e1->toString() << endl;
                                line << sign->getCommand() << endl;
                                line << "s.s $f0, " << tmp1;

                                spimCode->addOperation(line.str());

                                spimCode->addVariable(tmp1, new Type(T_FLOAT));
                                this->elements.insert(ElementFactory::createElement(tmp1), i);
                            }
                        }
                    }
                }
            }
        }
        return line.str();
    }

};

