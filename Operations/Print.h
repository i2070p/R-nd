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

                    if (type) {
                        if (type->is(T_INT)) {
                            line << "lw $t0, " << el->toString() << endl;
                            line << "li $v0, 1" << endl;
                            line << "move $a0, $t0" << endl;
                            line << "syscall" << endl;
                        } else if (type->is(T_FLOAT)) {
                            line << "l.s $f12, " << el->toString() << endl;
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
};
