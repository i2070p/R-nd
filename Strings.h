#pragma once

#include <stdarg.h>
#include <string>
#include <sstream>

#define VAR_UNDECLARED "undeclared."

using namespace std;

class Strings {
public:

    static string getUndeclaredText(string var) {
        stringstream ss;
        ss << "\"" << var << "\" " << VAR_UNDECLARED << endl;
        return ss.str();
    };

private:

    Strings() {
    }

    ~Strings() {
    }
};
