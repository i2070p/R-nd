#pragma once

#include "SimpleOperation.h"
#include <stack>
#include "../Elements/NameElement.h"
#include "../StackAdapter.h"
#include "Type.h"
#include "Expression.h"
#include <sstream>

using namespace std;

class Read : public SimpleOperation {
public:

    Read(Operation * parent, NameElement * var) : SimpleOperation(parent) {
        this->var = var;
    }

protected:
    NameElement * var;

    void generate(SpimCodeContainer * spimCode) {
        stringstream line;
        string var = this->var->toString();
        Type * type = spimCode->getVariable(var);
        if (type) {
            if (type->is(T_STR)) {

                //TODO opzłóka dżydania zdrinka

            } else if (type->is(T_INT)) {
                line << "li $v0, 5" << endl;
                line << "syscall" << endl;
                line << "move $t0, $v0" << endl;
                line << "sw $t0, " << var;
            } else if (type->is(T_FLOAT)) {
                line << "li $v0, 6" << endl;
                line << "syscall" << endl;
                line << "move $f0, $v0" << endl;
                line << "s.s $f0, " << var;
            }
            spimCode->addOperation(line.str());
        } else {
            throw string(Strings::getUndeclaredText(var));
        }
    }

private:

    void addNewLine(stringstream & line) {
        line << "li $v0, 4" << endl;
        line << "la $a0, $new_line" << endl;
        line << "syscall";
    }
};
