#pragma once

#include "SimpleOperation.h"
#include <stack>
#include "../Elements/NameElement.h"
#include "../StackAdapter.h"
#include "Type.h"
#include "Expression.h"
#include <sstream>

using namespace std;

class Print : public SimpleOperation {
public:

    Print(Operation * parent) : SimpleOperation(parent) {
        this->str = NULL;
    }

    Print(Operation * parent, string str) : SimpleOperation(parent) {
        this->str = new string(str);
    }

    void addExpression(Expression* exp) {
        this->exp = exp;
    }

protected:
    Expression * exp;
    string * str;

    void generate(SpimCodeContainer * spimCode) {
        stringstream line;

        if (this->str) {
            line << "li $v0, 4" << endl;
            string tmp = spimCode->addTmpStringVar(*this->str);
            line << "la $a0, " << tmp << endl;
            line << "syscall" << endl;
            this->addNewLine(line);
            spimCode->addOperation(line.str());

        } else if (this->exp) {
            this->exp->startGenerate(spimCode);
            Element * el = this->exp->getValueLiteral();
            if (this->exp->isStringExpression()) {
                line << "li $v0, 4" << endl;
                line << "la $a0, " << el->toString() << endl;
                line << "syscall" << endl;
            } else {
                if (ElementUtilities::isInt(el)) {
                    line << "li $t0, " << el->toString() << endl;
                    line << "li $v0, 1" << endl;
                    line << "move $a0, $t0" << endl;
                    line << "syscall" << endl;
                } else if (ElementUtilities::isFloat(el)) {
                    string tmp = spimCode->addTmpFloatVar(el->toString());
                    line << "l.s $f12, " << tmp << endl;
                    line << "li $v0, 2" << endl;
                    line << "syscall" << endl;
                } else if (ElementUtilities::isName(el)) {
                    Type * type = spimCode->getVariable(el->toString());
                    int * id = ((NameElement*) el)->getArrayId();
                    string idTmp = this->generateIndexComputing(spimCode, el, line, id);

                    if (type) {
                        if (type->is(T_INT)) {
                            if (id) {
                                line << "lw $t2, " << idTmp << endl;
                            }
                            line << "lw $t0, " << (id ? "($t2)" : el->toString()) << endl;
                            line << "li $v0, 1" << endl;
                            line << "move $a0, $t0" << endl;
                            line << "syscall" << endl;
                        } else if (type->is(T_FLOAT)) {
                            if (id) {
                                line << "lw $t2, " << idTmp << endl;
                            }
                            line << "l.s $f12, " << (id ? "($t2)" : el->toString()) << endl;
                            line << "li $v0, 2" << endl;
                            line << "syscall" << endl;
                        }
                    } else {
                        throw string(Strings::getUndeclaredText(el->toString()));
                    }
                }
                this->addNewLine(line);
                spimCode->addOperation(line.str());
            }
        }
    }

private:

    void addNewLine(stringstream & line) {
        line << "li $v0, 4" << endl;
        line << "la $a0, $new_line" << endl;
        line << "syscall";
    }

    string generateIndexComputing(SpimCodeContainer * spimCode, Element* e, stringstream & line, int * id) {
        string idTmp = "";

        if (id) {
            Element * m = spimCode->mStack.pop();
            line << "la $t2, " << e->toString() << endl;

            if (ElementUtilities::isInt(m)) {
                line << "li $t4, " << m->toString() << endl;
            } else if (ElementUtilities::isFloat(m)) {
                line << "l.s $f0, " << m->toString() << endl;
                line << "cvt.w.s $f0, $f0" << endl;
                line << "mfc1 $t4, $f0" << endl;
            } else if (ElementUtilities::isName(m)) {
                line << "lw $t4, " << m->toString() << endl;
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
