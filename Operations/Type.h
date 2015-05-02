#pragma once

#include <sstream>
#include "../ToString.h"

enum Types {
    T_INT,
    T_FLOAT,
    T_STR
};

class Type : public ToString {
public:

    Type(Types _type) : type(_type) {

    }

    bool is(Types type) {
        return this->type == type;
    }

    string toString() {
        stringstream ss;

        switch (this->type) {
            case T_INT:
                ss << ".word";
                break;
            case T_FLOAT:
                ss << ".float";
                break;
            case T_STR:
                ss << ".<unknow>";
                break;
        }
        return ss.str();
    }

protected:
    Types type;

};


