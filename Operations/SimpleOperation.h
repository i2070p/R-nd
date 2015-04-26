#pragma once

#include "Operation.h"

class SimpleOperation : public Operation {
public:

    SimpleOperation(Operation* parent) : Operation(parent) {
    };

    virtual string generate(SpimCodeContainer * spimCode) = 0;
};


