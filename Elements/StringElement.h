#pragma once

#include "LiteralElement.h"

using namespace std;

class StringElement : public LiteralElement<string> {
public:

    StringElement(string value) : LiteralElement(value) {
    }
    
};
