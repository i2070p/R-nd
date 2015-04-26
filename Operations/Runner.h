#pragma once

#include "ComplexOperation.h"
#include <sstream>

class Runner : public ComplexOperation {
public:

    Runner() : ComplexOperation(NULL) {

    }

    void generate(SpimCodeContainer * spimCode) {

    }

    string toString() {
        stringstream ss;
        ss << "This is Runner(" << this << ")";
        return ss.str();
    }

};

