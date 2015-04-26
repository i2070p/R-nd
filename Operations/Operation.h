#pragma once

#include "../SpimCodeContainer.h"
#include "../Elements/ElementUtilities.h"
#include <sstream>

using namespace std;

class Operation {
public:

    Operation(Operation* opr) : parent(opr) {

    }

    virtual void generate(SpimCodeContainer* spimCode) = 0;

    virtual Operation * getParent() {
        return this->parent;
    }

    virtual string toString() {
        return "This is operation";
    }

protected:
    Operation * parent;
};


