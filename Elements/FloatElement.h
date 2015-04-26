#pragma once

#include "LiteralElement.h"

using namespace std;

class FloatElement : public LiteralElement<float> {
public:

    FloatElement(float value) : LiteralElement(value) {
    }
    
};
