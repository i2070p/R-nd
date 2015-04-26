#pragma once

#include "Operation.h"

class SimpleOperation : public Operation {
public:

    SimpleOperation(Operation* parent) : Operation(parent) {
    };

    virtual void generate(SpimCodeContainer * spimCode) = 0;
};


