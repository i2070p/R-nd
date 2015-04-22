#pragma once

#include <stack>
using namespace std;

template <typename T>
class StackAdapter {
public:

    T pop() {
        T result = st.top();
        st.pop();
        return result;
    }

    T top() {
        return st.top();
    }

    void push(T el) {
        st.push(el);
    }

    bool isEmpty() {
        return st.empty();
    }


private:
    stack<T> st;
};
