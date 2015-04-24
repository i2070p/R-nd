/* 
 * File:   IfOperation.h
 * Author: meti
 *
 * Created on 22 kwiecień 2015, 09:51
 */

#pragma once

#include "ComplexOperation.h"

class IfOperation: public ComplexOperation {
public:       
    string generate() = 0;
    
    void addCondition(Operation* opr) {
        this->add("condition", opr);
    }
    
    void addTrueBlock(Operation* opr) {
        this->add("true_block", opr);
    }
    
    void addElseBlock(Operation* opr) {
        this->add("else_block", opr);
    }
        
};

