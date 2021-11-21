//создается файл в который записывается имя на каждом шагу и его ветки; в конструктор может подаваться этот файл
// и при чтении бужет восстанавливаться дерево (надо сделать его двусвязным и подниматься пока нету поворота)
#pragma once

#define MAXLEN 128

#include <iostream>

enum Akinators_code{

    MY_WIN = 0,
    MY_FAULT = 1,
    FIRST_TRY = 2,
};

enum Answers{

    YES = 0,
    NO = 1,
    DEBUG = 2,
};

struct knot{

    unsigned char* data = nullptr;
    struct knot* L_brunch = nullptr;
    struct knot* R_brunch = nullptr;
    struct knot* prev = nullptr;
    int knot_depth = 0;
    int knot_horizontal_position = 0;
};


class Tree{

public:
    
    Tree();
    Tree(FILE* base);
    ~Tree();

    void init_knot(knot*& cur_knot, bool l_brn_exist, bool r_brn_exist, knot* undef_brunch);
    void dump_tree();
    void create_base_file(char* input_name);

    bool is_leaf(knot* cur_knot){

        if (cur_knot == nullptr){ return true; }
        if ((cur_knot->L_brunch != nullptr) || (cur_knot->R_brunch != nullptr)){ return false; }
        return true;
    }

    Tree& operator = (const Tree&) = delete;
    Tree(const Tree& old__tree) = delete;

protected:

    knot* root = nullptr;
    int depth = 0;
    int rightest_knot = 0;
    int leftest_knot = 0;
    int data_size = 0;
};

void init_L_brn_data(knot* cur_knot); 
void init_R_brn_data(knot* cur_knot);
unsigned char* my_memcpy(unsigned char*);
int length(unsigned char*);
