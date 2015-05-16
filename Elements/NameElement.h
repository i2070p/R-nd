#pragma once

#include "LiteralElement.h"
#include <string>


using namespace std;

class NameElement : public LiteralElement<string> {
public:

    NameElement(string value) : LiteralElement(value) {
        this->ids = NULL;
    }

    NameElement(string value, int id) : LiteralElement(value) {
        this->ids = &id;
    }

    int * getArrayId() {
        return this->ids;
    }
    
    Element * _getArrayId() {
        return this->id;
    }

    void setArrayId(Element * id) {
        cout << id << "SDfsdfsdfs" << endl;
        this->id = id;
    }
    
protected:
    int * ids;
    Element * id;
};