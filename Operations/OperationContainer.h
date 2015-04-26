#pragma once

#include "Operation.h"
#include "../StackAdapter.h"

using namespace std;

class OperationContainer {
public:

    virtual void add(Operation *opr) = 0;
    virtual Operation* getChild(int id) = 0;
protected:
    StackAdapter<Operation*> children;
};


