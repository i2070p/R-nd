#pragma once

#include "Element.h"
#include <sstream>

using namespace std;

template <typename T>
class LiteralElement : public Element {
public:

    LiteralElement(T value) {
        this->value = value;
    }

    T getValue() {
        return this->value;
    }
    
    string toString() {
        stringstream ss;
        ss << value;        
        return ss.str();        
    }
    
private:
    T value;
};