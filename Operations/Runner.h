#pragma once

#include "ComplexOperation.h"
#include <sstream>

class Runner : public ComplexOperation {
public:

    Runner() : ComplexOperation(NULL) {

    }

    string generate(SpimCodeContainer * spimCode) {

    }

    string toString() {
        stringstream ss;
        ss << "This is Runner(" << this << ")";
        return ss.str();
    }

};

