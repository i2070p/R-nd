#pragma once

#include <string>
#include <stdio.h>
#include <sstream>

using namespace std;

enum ElementType {
     EL_INT      = 0,
     EL_FLOAT    = 1,
     EL_STR      = 2     
};

union Type {
      string * sval;
      int ival;
      float fval;      
};

class Element {
	public: 
        Element(int value) {
          type = EL_INT;
          this->value.ival=value;          
        }     
        
        Element(float value) {
          type = EL_FLOAT;
          this->value.fval=value;          
        }
          
        Element(string value) {
          type = EL_STR;
          this->value.sval=new string(value);          
        }
                          
        Element* add(Element* a) {
                 if (this->isInt() && a->isInt()) {
                    return new Element(this->getIntValue()+a->getIntValue());                      
                 } 
                 
                 if (this->isFloat() && a->isFloat()) {
                    return new Element(this->getFloatValue()+a->getFloatValue());                      
                 }  
                                    
                 if (this->isInt() && a->isFloat()) {
                    return new Element((float)this->getIntValue()+a->getFloatValue());                      
                 }                   
                 
                 if (this->isFloat() && a->isInt()) {
                    return new Element(this->getFloatValue()+(float)a->getIntValue());                      
                 }                   
        }		

        Element* mul(Element* a) {
                 if (this->isInt() && a->isInt()) {
                    return new Element(this->getIntValue()*a->getIntValue());                      
                 } 
                 
                 if (this->isFloat() && a->isFloat()) {
                    return new Element(this->getFloatValue()*a->getFloatValue());                      
                 }  
                                    
                 if (this->isInt() && a->isFloat()) {
                    return new Element((float)this->getIntValue()*a->getFloatValue());                      
                 }                   
                 
                 if (this->isFloat() && a->isInt()) {
                    return new Element(this->getFloatValue()*(float)a->getIntValue());                      
                 }                   
        }	

        Element* div(Element* a) {
                 if (this->isInt() && a->isInt()) {
                    return new Element((float)this->getIntValue()/(float)a->getIntValue());                      
                 } 
                 
                 if (this->isFloat() && a->isFloat()) {
                    return new Element(this->getFloatValue()/a->getFloatValue());                      
                 }  
                                    
                 if (this->isInt() && a->isFloat()) {
                    return new Element((float)this->getIntValue()/a->getFloatValue());                      
                 }                   
                 
                 if (this->isFloat() && a->isInt()) {
                    return new Element(this->getFloatValue()/(float)a->getIntValue());                      
                 }                   
        }
        
        Element* sub(Element* a) {
                 if (this->isInt() && a->isInt()) {
                    return new Element(this->getIntValue()-a->getIntValue());                      
                 } 
                 
                 if (this->isFloat() && a->isFloat()) {
                    return new Element(this->getFloatValue()-a->getFloatValue());                      
                 }  
                                    
                 if (this->isInt() && a->isFloat()) {
                    return new Element((float)this->getIntValue()-a->getFloatValue());                      
                 }                   
                 
                 if (this->isFloat() && a->isInt()) {
                    return new Element(this->getFloatValue()-(float)a->getIntValue());                      
                 }  
        }	
        
        Type getValue() {
          return this->value;            
        }
        
        int getIntValue() {
          return this->value.ival;            
        }
        
        float getFloatValue() {
          return this->value.fval;            
        }                
        
        ElementType getType() {
          return this->type;
        }        
        
        bool isInt() {
          return this->type==EL_INT;             
        }
        
        bool isFloat() {
          return this->type==EL_FLOAT;             
        }
        
        bool isString() {
          return this->type==EL_STR;             
        }        
 
        string toString() {
               stringstream s;
               if (this->isString()) {
                  s << *this->value.sval;                      
               }
               
               if (this->isInt()) {
                  s << this->value.ival;                      
               }
                    
               if (this->isFloat()) {
                  s << this->value.fval;                      
               }            
               return s.str();      
        }
 
    private:
        ElementType type;
        Type value;
};
