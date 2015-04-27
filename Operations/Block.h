#pragma once

#include "ComplexOperation.h"
#include <sstream>

class Block : public ComplexOperation {
public:

    Block(Operation * parent) : ComplexOperation(parent) {

    }

    string toString() {
        stringstream ss;
        ss << "Block(" << this << ") {";
        ss << "\n\tParent: " << this->getParent();
        ss << endl << "}";
        return ss.str();
    }

protected:

    void generate(SpimCodeContainer * spimCode) {
        this->generateChildren(spimCode);
    }
};

