#pragma once

#include <string>
#include "SignElement.h"
#include "IntElement.h"
#include "FloatElement.h"
#include "NameElement.h"

using namespace std;

class ElementFactory {
public:

    static Element * createElement(int value) {
        return new IntElement(value);
    }

    static Element * createElement(float value) {
        return new FloatElement(value);
    }

    static Element * createElement(string value) {
        return new NameElement(value);
    }

    static Element * createElement(SignType value) {
        return new SignElement(value);
    }
};