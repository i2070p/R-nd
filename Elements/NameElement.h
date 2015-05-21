#pragma once

#include "LiteralElement.h"
#include <string>


using namespace std;

class NameElement : public LiteralElement<string> {
public:

    NameElement(string value) : LiteralElement(value) {
        this->ids = NULL;
        this->array = false;
    }

    NameElement(string value, bool array) : LiteralElement(value) {
        this->array = array;
    }

    string * getArrayId() {
        return this->ids;
    }

    Element * _getArrayId() {
        return this->id;
    }

    void setArrayId(string * id) {
        this->ids = id;
    }

    bool isArray() {
        return this->array;
    }

protected:
    bool array;
    string * ids;
    Element * id;
};