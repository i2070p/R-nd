#pragma once

#include "Element.h"

using namespace std;

class ElementInt : Element<int> {
	public:  
        ElementInt(int value): Element(value) {}     
        Element<int>* add(Element* a, Element* b) {
                        
        }	
};
