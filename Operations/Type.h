#pragma once

#include <sstream>
#include "../ToString.h"

enum Types {
    T_INT,
    T_FLOAT,
    T_STR,
    T_AINT,
    T_AFLOAT,
    T_ASTR
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

        switch (this->type % 3) {
            case T_INT:
                ss << ".word";
                break;
            case T_FLOAT:
                ss << ".float";
                break;
            case T_STR:
                ss << ".asciiz";
                break;
        }
        return ss.str();
    }

    void setArrayType() {
        switch (this->type) {
            case T_INT:
                this->type = T_AINT;
                break;
            case T_FLOAT:
                this->type = T_AFLOAT;
                break;
            case T_STR:
                this->type = T_ASTR;
                break;
        }
    }

protected:
    Types type;

};


