#pragma once

#include "LiteralElement.h"
#include <string>


using namespace std;

class NameElement : public LiteralElement<string> {
public:

    NameElement(string value) : LiteralElement(value) {
        this->id = NULL;
        this->array = false;
    }

    NameElement(string value, bool array) : LiteralElement(value) {
        this->array = array;
    }

    string * getArrayId() {
        return this->id;
    }

    void setArrayId(string * id) {
        this->id = id;
    }

    bool isArray() {
        return this->array;
    }

protected:
    bool array;
    string * id;
};