#pragma once

#include "../SpimCodeContainer.h"
#include "../Elements/ElementUtilities.h"
#include <sstream>
#include "../Toolkit.h"
#include "Type.h"
using namespace std;

class Operation {
public:

    Operation(Operation* opr) : parent(opr) {

    }

    virtual void startGenerate(SpimCodeContainer* spimCode) {
        this->beforeGenerate(spimCode);
        this->generate(spimCode);
        this->afterGenerate(spimCode);
    }

    virtual Operation * getParent() {
        return this->parent;
    }

    virtual string toString() {
        return "This is operation";
    }

protected:
    Operation * parent;

    virtual void generate(SpimCodeContainer* spimCode) = 0;

    virtual void beforeGenerate(SpimCodeContainer* spimCode) {

    }

    virtual void afterGenerate(SpimCodeContainer* spimCode) {

    }
};


