#include "headers/Array.h"

template <typename T>
Array<T>::Array(int init_size){

    data = (T*) calloc(init_size, sizeof(T));

    is_primal_array = 1;
    primal_size = init_size;
    primal_overflow_count = 0;

    size = &primal_size;
    overflow_count = &primal_overflow_count;
}

template <typename T>
Array<T>::~Array(){
    
    if (is_primal_array == 1){

        free(data);

        size = 0;
        overflow_count = 0;
        is_primal_array = 0;
    }
}

template <typename T>
Array<T>::Array(const Array& old_arr){

    free(data);

    data = old_arr.data;

    is_primal_array = 0;
    size = old_arr.size;
    overflow_count = old_arr.overflow_count; 

    primal_size = 0;
    primal_overflow_count = 0;    
}

template <typename T>
Array<T>& Array<T>::operator = (const Array& old_arr){

    data = (T*) calloc(*(old_arr.size), sizeof(T));
    memcpy(data, old_arr.data, *(old_arr.size) * sizeof(T));

    primal_size = *(old_arr.size);
    primal_overflow_count = *(old_arr.overflow_count);
    size = &primal_size;
    overflow_count = &primal_overflow_count;
}

template <typename T>
T& Array<T>::operator [] (int number){
    
    assert(number >= 0);

    if (number < *size){

        return data[number];
    } else{

        (*overflow_count)++;
        resize(number);
        return data[number];
    }
    
} 

template <typename T>
void Array<T>::resize(int new_size){

    data = (T*) realloc(data, sizeof(T) * new_size);
    (*size) = new_size;
}

template <typename T>
int Array<T>::check_overflow(){

    if ((*overflow_count) > 0){

        *overflow_count = 0;
        return 1;
    } else{

        return 0;
    }
}

template <typename T>
void Array<T>::push(T val){

    data[*size - 1] = val;
}

template <typename T>
T Array<T>::pop(){

    return data[*size - 1];
}
