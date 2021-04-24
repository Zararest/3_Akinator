#ifndef ARRAY_H
#define ARRAY_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cstring>
#include <cassert>

template <class T>
class Array{

public: 

    Array(int);

    ~Array();

    Array(const Array&);

    Array& operator = (const Array&);

    T& operator [] (int);

    void resize(int);

    int check_overflow();

    void push(T);

    T pop();

private:

    void private_resize(int);

    int is_primal_array;

    T* data;

    int primal_size;

    int primal_overflow_count;

    int* size;

    int* overflow_count;
};


#endif