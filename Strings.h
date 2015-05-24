#pragma once

#include <stdarg.h>
#include <string>
#include <sstream>

#define VAR_UNDECLARED "undeclared."
#define INCORRECT_EPXRESSION "It is impossible to calculate expression which contains string element"
#define INCOMPATIBLE_TYPES "Incompatible types."
#define ALREADY_EXISTS "already exists."
using namespace std;

class Strings {
public:

    static string getUndeclaredText(string var) {
        stringstream ss;
        ss << "\"" << var << "\" " << VAR_UNDECLARED;
        return ss.str();
    };

    static string getAlreadyExistsText(string var) {
        stringstream ss;
        ss << "\"" << var << "\" " << ALREADY_EXISTS;
        return ss.str();
    };

    static string getIncorrectExpressionText(string var) {
        stringstream ss;
        ss << INCORRECT_EPXRESSION << " - \"" << var << "\".";
        return ss.str();
    };
    
    static string getIncompatibleTypesText() {
        stringstream ss;
        ss << INCOMPATIBLE_TYPES;
        return ss.str();
    };

private:

    Strings() {
    }

    ~Strings() {
    }
};
