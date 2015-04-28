#pragma once

#include "LiteralElement.h"

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

    string getCommand() {
        return commands[this->getValue()];
    }
};
