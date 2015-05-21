#pragma once

#include "../StackAdapter.h"
#include <iostream>
#include <fstream>
#include "Expression.h"
#include <typeinfo>
#include <sstream>
#include "Type.h"
#include "Declaration.h"
#include "Assignment.h"
#include "Runner.h"
#include "Block.h"
#include "Condition.h"
#include "If.h"
#include "While.h"
#include "Print.h"
#include "Read.h"
#include "ArrayDeclaration.h"

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
        this->currentSimple = new Declaration(this->current, new NameElement(name), new Type(this->dataType));
    }

    void buildAssignment(string name) {
        this->currentSimple = new Assignment(this->current, new NameElement(name));
    }

    void buildArrayAssignment(string name) {
        this->currentSimple = new Assignment(this->current, new NameElement(name, true));
    }    
    
    void buildArrayDeclaration(string name) {
        this->currentSimple = new ArrayDeclaration(this->current, new NameElement(name, true), new Type(this->dataType), this->arraySize);
    }

    void buildPrint() {
        this->currentSimple = new Print(this->current);
    }

    void buildPrint(string str) {
        this->currentSimple = new Print(this->current, str);
    }

    void buildRead(string name) {
        this->currentSimple = new Read(this->current, new NameElement(name));
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

    void finishIndexExpression() {
        if (this->exp) {
            this->exps.push(this->exp);
            this->exp = NULL;
        }
    }

    void popExpression() {
        if (this->exps.size()) {
            this->exp = this->exps.pop();
        }
    }

    void addArrayIndexExpression() {
        if (!this->exps.isEmpty()) {
            Expression * exp = this->exps.pop();
            if (dynamic_cast<Assignment *> (this->currentSimple)) {
                ((Assignment *)this->currentSimple)->addArrayIndexExpression(exp);
            }
        }
    }

    void addExpressionToSimpleOperation() {
        if (!this->exps.isEmpty()) {
            Expression * exp = this->exps.pop();
            if (dynamic_cast<Declaration *> (this->currentSimple)) {
                ((Declaration *)this->currentSimple)->addExpression(exp);
            } else if (dynamic_cast<Assignment *> (this->currentSimple)) {
                ((Assignment *)this->currentSimple)->addExpression(exp);
            } else if (dynamic_cast<Print *> (this->currentSimple)) {
                ((Print *)this->currentSimple)->addExpression(exp);
            }
        }
        this->addSimpleOperationToCurrentBlock();
    }

    void startGenerate() {
        try {
            this->runner->startGenerate(&this->spimCode);
        } catch (string e) {
            cout << e << endl;
        }

        ofstream myfile;
        myfile.open("fin.txt");
        myfile << this->spimCode.toString();
        myfile.close();
    }

    void setDataType(Types dataType) {
        this->dataType = dataType;
    }

    void setArraySize(int size) {
        this->arraySize = size;
    }
protected:
    StackAdapter<Operation*> building;
    StackAdapter<Operation*> ready;
    Types dataType;
    int arraySize;
    StackAdapter<Expression*> exps;
    Expression * exp;
    ComplexOperation * current, * runner;

    SimpleOperation * currentSimple;
    SpimCodeContainer spimCode;
};

