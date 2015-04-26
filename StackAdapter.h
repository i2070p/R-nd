#pragma once

#include <vector>
using namespace std;

template <typename T>
class StackAdapter {
public:

    T pop() {
        T result = st.back();
        st.pop_back();
        return result;
    }

    T pop_front() {
        T result = st.at(0);
        st.erase(st.begin());
        return result;
    }

    T top() {
        return st.back();
    }

    T getAndErase(int i) {
        T result = st.at(i);
        st.erase(st.begin() + i);
        return result;
    }

    void push(T el) {
        st.push_back(el);
    }

    void push_front(T el) {
        st.insert(st.begin(), el);
    }

    bool isEmpty() {
        return st.empty();
    }

    int size() {
        return st.size();
    }

    T at(int i) {
        return st.at(i);
    }



private:
    vector<T> st;
};
