#pragma once

#include "LiteralElement.h"

using namespace std;

string commands[4] = {
    "div", "add", "sub", "mul"
};

enum SignType {
    DIVISION,
    ADDITION,
    SUBTRACTION,
    MULTIPLICATION,
    EQUAL,
    NOTEQUAL,
    LESSTHAN,
    LESSEQUAL,
    GREATERTHAN,
    GREATEREQUAL
};

class SignElement : public LiteralElement<SignType> {
public:

    SignElement(SignType value) : LiteralElement(value) {
    }
    
    string getCommand() {
        return commands[this->getValue()];
    }
};
