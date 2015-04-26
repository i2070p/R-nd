#pragma once

#include "LiteralElement.h"

using namespace std;

class IntElement : public LiteralElement<int> {
public:

    IntElement(int value) : LiteralElement(value) {
    }
};
