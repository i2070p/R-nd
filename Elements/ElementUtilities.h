#pragma once

using namespace std;

class ElementUtilities {
public:

    static bool isInt(Element * element) {
        return dynamic_cast<IntElement *> (element);
    }

private:

    ElementUtilities() {
    }

    ~ElementUtilities() {
    }
};


