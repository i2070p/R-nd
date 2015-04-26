#pragma once

#include "LiteralElement.h"
#include <string>

using namespace std;

class NameElement : public LiteralElement<string> {
public:

    NameElement(string value) : LiteralElement(value) {
    }
};