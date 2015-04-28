#pragma once

#include "ComplexOperation.h"
#include "Expression.h"

enum IfBlocks {
    IF_BLOCK = 0,
    ELSE_BLOCK = 1
};

class If : public ComplexOperation {
public:

    If(Operation * parent) : ComplexOperation(parent) {

    }

    void addCondition(Expression* opr) {
        this->condition = opr;
    }

    string toString() {
        stringstream ss;
        ss << "If(" << this << ") {";
        ss << "\n\tParent: " << this->getParent();
        ss << endl << "}";
        return ss.str();
    }

protected:
    Expression *condition;

    void beforeGenerate(SpimCodeContainer* spimCode) {
        this->condition->startGenerate(spimCode);

        stringstream line;
        line << "li $t0, 0";
        spimCode->addOperation(line.str());
        line.str("");

        line << "li $t1, " << this->condition->getValueLiteral()->toString();
        spimCode->addOperation(line.str());
        line.str("");

        line << "beq $t0, $t1, label" << spimCode->nextLabel();
        spimCode->addOperation(line.str());
    }

    void generate(SpimCodeContainer * spimCode) {

        bool existElse = this->children.size() > 1; 
        
        int tmp = spimCode->getLabel();
        
        this->children.at(IF_BLOCK)->startGenerate(spimCode);
        if (existElse) {
            this-
        }
        
        spimCode->addLabel(false);
        
        if (existElse) {
            stringstream line;            
            line << "b label" << spimCode->nextLabel();
            
            spimCode->addOperation(line.str());
            

            ((Block*) this->children.at(ELSE_BLOCK))->startGenerate(spimCode);
        }

    }

    void afterGenerate(SpimCodeContainer* spimCode) {

    }
};

