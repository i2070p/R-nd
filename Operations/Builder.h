#pragma once

#include "../StackAdapter.h"
#include "Expression.h"
#include <typeinfo>
#include <sstream>
#include "Types.h"
#include "Declaration.h"
#include "Assignment.h"
#include "Runner.h"
#include "Block.h"
#include "If.h"

using namespace std;

class Builder {
public:

    Builder() {
        this->current = this->runner = new Runner();
    }

    void startBlock() {
        Block * newBlock = new Block(this->current);
        this->current->add(newBlock);
        this->current = newBlock;
    }

    void endBlock() {
        if (this->current->getParent() != NULL) {
            this->current = (ComplexOperation*)this->current->getParent();
        }
    }

    void startIf() {
        If * newIf = new If(this->current);
        newIf->addCondition(this->exp);
        
        this->current->add(newIf);
        this->current = newIf;
        this->exp = NULL;
        
        this->startBlock();        
    }

    void addElse() {
        this->endBlock();
        this->startBlock();
    }

    void endIf() {
        this->endBlock();
        this->current = (ComplexOperation*)this->current->getParent();
    }

    void addSimpleOperationToCurrentBlock() {
        this->current->add(this->currentSimple);
    }

    void buildDeclaration(string name) {
        this->currentSimple = new Declaration(this->current, new NameElement(name), this->dataType);
    }

    void buildAssignment(string name) {
        this->currentSimple = new Assignment(this->current, new NameElement(name));
    }

    void addToExpression(Element * element) {
        if (this->exp) {
            this->exp->addElement(element);
        } else {
            this->exp = new Expression(this->current, element);
        }
    }

    void addExpressionToSimpleOperation() {
        if (dynamic_cast<Declaration *> (this->currentSimple)) {
            ((Declaration *)this->currentSimple)->addExpression(this->exp);
        } else if (dynamic_cast<Assignment *> (this->currentSimple)) {
            ((Assignment *)this->currentSimple)->addExpression(this->exp);
        }

        this->exp = NULL;
        this->addSimpleOperationToCurrentBlock();
    }

    void traverse() {
        StackAdapter<Operation*> stack;
        stack.push(this->runner);
        Operation * tmp;
        ComplexOperation * cTmp;
        while (!stack.isEmpty()) {
            tmp = stack.pop();
            
            tmp->generate(&this->spimCode);
            //cout << tmp->toString() << endl;

            if (dynamic_cast<ComplexOperation *> (tmp)) {
                cTmp = (ComplexOperation*) tmp;

                for (int i = 0; i < cTmp->getChildren().size(); i++) {
                    stack.push(cTmp->getChild(i));
                }
            }
        }
        
        
        cout << this->spimCode.toString();
    }

    void setDataType(Types dataType) {
        this->dataType = dataType;
    }

protected:
    StackAdapter<Operation*> building;
    StackAdapter<Operation*> ready;
    Types dataType;

    Expression* exp;

    ComplexOperation * current, * runner;
    SimpleOperation * currentSimple;
    SpimCodeContainer spimCode;
};

