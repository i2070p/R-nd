#pragma once

#include "LiteralElement.h"
#include "../Operations/Type.h"

using namespace std;

string commands[10] = {
    "div", "add", "sub", "mul", "seq", "sne", "sgt", "sle", "slt", "sge"
};

enum SignType {
    DIVISION,
    ADDITION,
    SUBTRACTION,
    MULTIPLICATION,
    EQUAL,
    NOTEQUAL,
    GREATERTHAN,
    LESSEQUAL,
    LESSTHAN,
    GREATEREQUAL,
};

class SignElement : public LiteralElement<SignType> {
public:

    SignElement(SignType value) : LiteralElement(value) {
    }

    string getCommand(Types type = T_FLOAT) {
        stringstream line;

        switch (type) {
            case T_INT:
                line << commands[this->getValue()] << " $t0, $t0, $t1";
                break;
            case T_FLOAT:
                switch (this->getValue()) {
                    case DIVISION:
                    case ADDITION:
                    case SUBTRACTION:
                    case MULTIPLICATION:
                        line << commands[this->getValue()] << ".s $f0, $f0, $f1";
                        break;
                    case EQUAL:
                        line << this->getTemplate("c.eq.s $f0, $f1");
                        break;
                    case NOTEQUAL:
                        line << this->getTemplate("c.eq.s $f0, $f1", true);
                        break;
                    case GREATERTHAN:
                        line << this->getTemplate("c.lt.s $f1, $f0");
                        break;
                    case LESSEQUAL:
                        line << this->getTemplate("c.le.s $f0, $f1");
                        break;
                    case LESSTHAN:
                        line << this->getTemplate("c.lt.s $f0, $f1");
                        break;
                    case GREATEREQUAL:
                        line << this->getTemplate("c.le.s $f1, $f0");
                        break;
                }

                break;
        }

        return line.str();
    }
private:

    string getTemplate(string val, bool notEqual = false) {

        stringstream line;

        if (!notEqual) {
            line << "l.s $f2, $f_zero" << endl;
            line << "l.s $f3, $f_one" << endl;
        } else {
            line << "l.s $f2, $f_one" << endl;
            line << "l.s $f3, $f_zero" << endl;
        }
        
        line << val << endl;

        line << "movt.s $f2, $f3" << endl;
        line << "mov.s $f0, $f2";

        return line.str();
    }
};
