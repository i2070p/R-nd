#pragma once

#include "LiteralElement.h"
#include <string>


using namespace std;

class NameElement : public LiteralElement<string> {
public:

    NameElement(string value) : LiteralElement(value) {
        this->id = NULL;
    }

    NameElement(string value, int id) : LiteralElement(value) {
        this->id = &id;
    }

    int * getArrayId() {
        return this->id;
    }

    
    
protected:
    int * id;

};