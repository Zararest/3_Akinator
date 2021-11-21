#include "headers/Tree.hpp"

#include <cassert>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>

void Tree::init_L_brn_data(knot* tmp){

    tmp->L_brunch = new knot;
    tmp->L_brunch->knot_depth = tmp->knot_depth + 1;
    tmp->L_brunch->knot_horizontal_position = tmp->knot_horizontal_position - 1;
    tmp->L_brunch->prev = tmp;
}

void Tree::init_R_brn_data(knot* tmp){

    tmp->R_brunch = new knot;
    tmp->R_brunch->knot_depth = tmp->knot_depth + 1;
    tmp->R_brunch->knot_horizontal_position = tmp->knot_horizontal_position - 1;
    tmp->R_brunch->prev = tmp;
}

int length(unsigned char* line){

    if (line == nullptr){ return 0; }

    int i = 0, len = 0;
    while (line[i] != '\0'){

        len++;
        i += 1 + ((line[i] & 0x80) == 128); 
    }

    return len;
}

unsigned char* my_memcpy(unsigned char* line){

    if (line == nullptr){
        return nullptr;
    }

    int len = 0;
    unsigned char* new_line = nullptr;

    while (line[len] != '\0'){
        len++;
    }

    //new_line = (unsigned char*)calloc(len + 1, sizeof(char));
    new_line = new unsigned char[len + 1];

    for(int i = 0; i <= len; i++){
        new_line[i] = line[i];
    }

    return new_line;
}

Tree::Tree(){

    root = new knot;
}

void Tree::init_knot(knot*& cur_knot, bool l_brn_exist, bool r_brn_exist, knot* undef_brunch){

    if (cur_knot->knot_horizontal_position > rightest_knot) rightest_knot = cur_knot->knot_horizontal_position;
    if (cur_knot->knot_horizontal_position < leftest_knot) leftest_knot = cur_knot->knot_horizontal_position;
    if (cur_knot->knot_depth > depth) depth = cur_knot->knot_depth;

    assert(cur_knot != nullptr);
    if (r_brn_exist){

        cur_knot->R_brunch = undef_brunch;
    } else{

        cur_knot->R_brunch = nullptr;
    }

    if (l_brn_exist){

        init_L_brn_data(cur_knot);
        cur_knot = cur_knot->L_brunch;
    } else{

        cur_knot->L_brunch = nullptr;
        while ((cur_knot->R_brunch != undef_brunch) && (cur_knot->prev != nullptr)){

            cur_knot = cur_knot->prev;
        }

        if (cur_knot->R_brunch == undef_brunch){ //!!!
            
            init_R_brn_data(cur_knot);
            cur_knot = cur_knot->R_brunch;
        }
    }
}

Tree::Tree(FILE* base){

    root = new knot;

    assert(base != nullptr);
    fread(&data_size, sizeof(int), 1, base);
    unsigned char* buffer = new unsigned char[data_size];
    fread(buffer, sizeof(char), data_size, base);

    knot* undef_brunch = new knot;
    knot* cur_knot = root;
    bool r_brn_exist = 0, l_brn_exist = 0;
    unsigned char* word = nullptr;
    unsigned char* name = nullptr;

    word = (unsigned char*) strtok( (char*)buffer, "|");
    
    while (word != nullptr){

        name = word;        
        
        word = (unsigned char*) strtok(nullptr, "|");
        assert(word != nullptr);
        l_brn_exist = (word[0] == 'Y');

        word = (unsigned char*) strtok(nullptr, "|");
        assert(word != nullptr);
        r_brn_exist = (word[0] == 'Y');

        word = (unsigned char*) strtok(nullptr, "|");

        cur_knot->data = my_memcpy(name);

        init_knot(cur_knot, l_brn_exist, r_brn_exist, undef_brunch);
    }

    delete undef_brunch;
    delete[] buffer;
}
//------------------

//------------------Рекурсивная простая распечатка 
void Tree::print_knot(knot* cur_knot){

    if (cur_knot != nullptr){

        print_knot(cur_knot->L_brunch);
        printf("%s[%i][%i]\n", cur_knot->data, cur_knot->knot_horizontal_position, cur_knot->knot_depth);
        print_knot(cur_knot->R_brunch);
    }
}

void Tree::dump_common(){

    printf("Dumped tree: *name*[horizontal_position][depth]\n");
    print_knot(root);
}
//------------------

//------------------Рекурсивная запись данных в файл
void Tree::add_to_file(knot* cur_knot, FILE* output){

    if (cur_knot != nullptr){

        fwrite(cur_knot->data, sizeof(char), length(cur_knot->data), output);
        if (cur_knot->L_brunch != nullptr){

            fwrite("|Y|", sizeof(char), 3, output);
        } else{

            fwrite("|N|", sizeof(char), 3, output);
        }

        if (cur_knot->R_brunch != nullptr){

            fwrite("|Y|", sizeof(char), 3, output);
        } else{

            fwrite("|N|", sizeof(char), 3, output);
        }

        add_to_file(cur_knot->L_brunch, output);
        add_to_file(cur_knot->R_brunch, output);
    }
}

void Tree::create_base_file(char* name_of_file){

    FILE* output = fopen(name_of_file, "wb");
    assert(output != nullptr);

    data_size += 2;
    fwrite(&data_size, sizeof(int), 1, output);
    add_to_file(root, output);

    fclose(output);
}
//------------------

//------------------Рекурсивный деструктор
void Tree::delete_knot(knot* cur_knot){

    if (cur_knot != nullptr){

        delete_knot(cur_knot->L_brunch);
        delete_knot(cur_knot->R_brunch);

        delete[] cur_knot->data;
        delete cur_knot;
    }
}

Tree::~Tree(){

    delete_knot(root);
}
//------------------

void Tree::add_before_left(knot_ptr cur_knot, unsigned char* leaf_data){
    
}

void Tree::add_L_leaf(knot_ptr cur_knot, unsigned char* leaf_data){

    init_L_brn_data(cur_knot.knot_of_tree);
    cur_knot.knot_of_tree->L_brunch->data = my_memcpy(leaf_data);//добавить проыерку на nullptr
    
    data_size += length(cur_knot.knot_of_tree->L_brunch->data) + 6;
}

void Tree::add_R_leaf(knot_ptr cur_knot, unsigned char* leaf_data){

    init_R_brn_data(cur_knot.knot_of_tree);
    cur_knot.knot_of_tree->R_brunch->data = my_memcpy(leaf_data);
    
    data_size += length(cur_knot.knot_of_tree->R_brunch->data) + 6;
}

void Tree::print_brunch(int* position_of_line, int number_of_lines, knot* cur_knot){
    
    int i = 0;

    if (cur_knot != nullptr){

        for (i = 0; i < number_of_lines; i++){

            for (int j = 0; j < abs(position_of_line[i]); j++){
                printf(" ");
            }

            if (position_of_line[i] > 0){

               printf("|"); 
            } 
        }

        if (position_of_line[i - 1] < 0){

            printf("|__");
        } else{

            printf("__");
        }
        
        assert(cur_knot->data != nullptr);
        printf("%s\n", cur_knot->data);

        position_of_line[number_of_lines] = length(cur_knot->data); 

        position_of_line[i] = abs(position_of_line[i]);
        print_brunch(position_of_line, number_of_lines + 1, cur_knot->R_brunch);
        position_of_line[i] = abs(position_of_line[i]) * (-1);
        print_brunch(position_of_line, number_of_lines + 1, cur_knot->L_brunch);
    }
}

void Tree::dump(){

    assert(root != nullptr);

    int number_of_lines = 1;
    int* position_of_line = new int[depth + 1];
    position_of_line[0] = length(root->data);

    printf("\nDump of tree:\n");
    if (root-> data != nullptr){

        printf("%s\n", root->data);
        print_brunch(position_of_line, number_of_lines, root->R_brunch);
        position_of_line[0] = position_of_line[0] * (-1);
        print_brunch(position_of_line, number_of_lines, root->L_brunch);
    } else{

        printf("nothing\n");
    }

    delete[] position_of_line;
}

