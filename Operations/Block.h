#pragma once

#include "ComplexOperation.h"
#include <sstream>

class Block : public ComplexOperation {
public:

    Block(Operation * parent) : ComplexOperation(parent) {

    }

protected:

    void generate(SpimCodeContainer * spimCode) {
        this->generateChildren(spimCode);
    }
};

