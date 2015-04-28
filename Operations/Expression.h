#pragma once

#include "SimpleOperation.h"
#include <stack>
#include "../Elements/LiteralElement.h"
#include "../StackAdapter.h"
#include <sstream>

using namespace std;

class Expression : public SimpleOperation {
public:

    Element * getValueLiteral() {
        return this->elements.pop();
    }

    Expression(Operation * parent) : SimpleOperation(parent) {

    }

    Expression(Operation * parent, Element* element) : SimpleOperation(parent) {
        this->elements.push(element);
    }

    void addElement(Element* element) {
        this->elements.push(element);
    }

    string toString() {
        stringstream ss;
        ss << "Expression: ";
        for (int i = 0; i < this->elements.size(); i++) {
            ss << this->elements.at(i)->toString() << " ";
        }
        return ss.str();
    }

protected:
    StackAdapter<Element*> elements;

    void generate(SpimCodeContainer * spimCode) {
        bool change = true;

        while (change) {
            change = false;
            for (int i = 0; i<this->elements.size(); i++) {
                if (dynamic_cast<SignElement *> (this->elements.at(i))) {
                    compute((SignElement*) this->elements.getAndErase(i), spimCode, i);
                    change = true;
                    break;
                }
            }
        }
    }

    void compute(SignElement * sign, SpimCodeContainer * spimCode, int i) {
        stringstream sTmp;
        stringstream line;

        Element* first = this->elements.getAndErase(i - 1);
        Element* second = this->elements.getAndErase(i - 2);

        sTmp << "$tmp" << spimCode->nextTmp();
        line << "l" << (ElementUtilities::isInt(first) ? "i" : "w") << " $t0, " << first->toString();
        spimCode->addOperation(line.str());
        line.str("");

        line << "l" << (ElementUtilities::isInt(second) ? "i" : "w") << " $t1, " << second->toString();
        spimCode->addOperation(line.str());
        line.str("");

        line << sign->getCommand() << " $t0, $t0, $t1";
        spimCode->addOperation(line.str());
        line.str("");

        line << "sw $t0, " << sTmp.str();
        spimCode->addOperation(line.str());
        line.str("");

        spimCode->addVariable(sTmp.str(), ".word 1");

        this->elements.push_front(ElementFactory::createElement(sTmp.str()));
    }

};

