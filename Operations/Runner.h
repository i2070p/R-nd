#pragma once

#include "ComplexOperation.h"
#include <sstream>

class Runner : public ComplexOperation {
public:

    Runner() : ComplexOperation(NULL) {

    }
    
protected:

    void generate(SpimCodeContainer * spimCode) {
        this->generateChildren(spimCode);
    }

};

