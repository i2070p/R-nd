#pragma once

#include <string>
#include "SignElement.h"
#include "IntElement.h"
#include "FloatElement.h"
#include "StringElement.h"
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

    static Element * createElement(string value, bool isName = true) {

        Element * result = NULL;

        if (isName) {
            result = new NameElement(value);
        } else {
            result = new StringElement(value);
        }

        return result;
    }

    static Element * createElement(string value, int id) {
        return new NameElement(value, id);
    }

    static Element * createElement(SignType value) {
        return new SignElement(value);
    }
};