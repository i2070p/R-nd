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
#include "Condition.h"
#include "If.h"
#include "While.h"

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
        
        newIf->addCondition(this->exps.pop());
        
        this->current->add(newIf);
        this->current = newIf;

        this->startBlock();
    }

    void addElse() {
        this->endBlock();
        this->startBlock();
    }

    void startWhile() {
        While * newWhile = new While(this->current);
        
        newWhile->addCondition(this->exps.pop());
        
        this->current->add(newWhile);
        this->current = newWhile;

        this->startBlock();
    }    
    
    
    void endWhile() {
        this->endBlock();
        this->current = (ComplexOperation*)this->current->getParent();
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

    void finishExpression() {
        if (this->exp) {
            this->exps.push(this->exp);
            this->exp = NULL;
        }
    }

    void addExpressionToSimpleOperation() {
        if (!this->exps.isEmpty()) {
            Expression * exp = this->exps.pop();
            if (dynamic_cast<Declaration *> (this->currentSimple)) {
                ((Declaration *)this->currentSimple)->addExpression(exp);
            } else if (dynamic_cast<Assignment *> (this->currentSimple)) {
                ((Assignment *)this->currentSimple)->addExpression(exp);
            }
        }
        this->addSimpleOperationToCurrentBlock();
    }

    void startGenerate() {
        this->runner->startGenerate(&this->spimCode);
        cout << this->spimCode.toString();
    }

    void setDataType(Types dataType) {
        this->dataType = dataType;
    }

protected:
    StackAdapter<Operation*> building;
    StackAdapter<Operation*> ready;
    Types dataType;

    StackAdapter<Expression*> exps;
    Expression * exp;
    ComplexOperation * current, * runner;

    SimpleOperation * currentSimple;
    SpimCodeContainer spimCode;
};

