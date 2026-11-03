#ifndef STACK_H
#define STACK_H

#include <iostream>
using namespace std;

template <typename T>
class Stack {
private:
    T* arr;
    int capacity;
    int topIndex;

    void resize() {
        int newCapacity = capacity * 2;
        T* newArr = new T[newCapacity];

        for (int i = 0; i <= topIndex; i++) {
            newArr[i] = arr[i];
        }

        delete[] arr;
        arr = newArr;
        capacity = newCapacity;
    }

public:
    Stack() {
        capacity = 2;
        arr = new T[capacity];
        topIndex = -1;
    }

    ~Stack() {
        delete[] arr;
    }

    Stack(const Stack&) = delete;
    Stack& operator=(const Stack&) = delete;

    void push(const T& value) {
        if (topIndex + 1 == capacity) {
            resize();
        }

        arr[++topIndex] = value;
    }

    bool pop(T& value) {
        if (isEmpty()) {
            return false;
        }

        value = arr[topIndex--];
        return true;
    }

    bool peek(T& value) const {
        if (isEmpty()) {
            return false;
        }

        value = arr[topIndex];
        return true;
    }

    bool isEmpty() const {
        return topIndex == -1;
    }

    int size() const {
        return topIndex + 1;
    }
};

#endif

