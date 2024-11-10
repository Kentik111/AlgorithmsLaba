#include <iostream>
#include <string>
//#include <cctype>
#include <stdexcept>

using namespace std;

#ifndef HEADER_H
#define HEADER_H



template<typename T>
class Stack {
private:
    struct Node;
    Node* topNode;

public:
    Stack();
    ~Stack();
    bool isEmpty() const;
    void push(T value);
    void pop();
    T top() const;
    void print() const;
};
#endif // HEADER_H
