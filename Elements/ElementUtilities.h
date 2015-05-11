#pragma once

#include "Element.h"
#include "SignElement.h"
#include "FloatElement.h"
#include "NameElement.h"
#include "IntElement.h"
#include <sstream>
#include "../SpimCodeContainer.h"

using namespace std;

class ElementUtilities {
public:

    static bool isInt(Element * element) {
        return dynamic_cast<IntElement *> (element);
    }

    static bool isFloat(Element * element) {
        return dynamic_cast<FloatElement *> (element);
    }

    static bool isName(Element * element) {
        return dynamic_cast<NameElement *> (element);
    }

    static bool isString(Element * element) {
        return dynamic_cast<StringElement *> (element);
    }
        
    static bool isSign(Element * element) {
        return dynamic_cast<SignElement *> (element);
    }

private:

    ElementUtilities() {
    }

    ~ElementUtilities() {
    }
};


