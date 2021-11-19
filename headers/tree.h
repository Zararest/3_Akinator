//создается файл в который записывается имя на каждом шагу и его ветки; в конструктор может подаваться этот файл
// и при чтении бужет восстанавливаться дерево (надо сделать его двусвязным и подниматься пока нету поворота)
#ifndef TREE
#define TREE

// TODO: перекинуть максимальное число инклюдов в cpp файлы
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <cstring>
#include <cassert>

#define MAXLEN 100

// TODO: написать функцию
#define INIT_L_BRN_DATA(tmp) tmp->L_brunch = (knot*) calloc(1, sizeof(knot));\
                             tmp->L_brunch->knot_depth = tmp->knot_depth + 1;\
                             tmp->L_brunch->knot_horizontal_position = tmp->knot_horizontal_position - 1;\
                             tmp->L_brunch->prev = tmp;\
                             tmp->L_brunch->L_brunch = NULL;\
                             tmp->L_brunch->R_brunch = NULL;\

#define INIT_R_BRN_DATA(tmp) tmp->R_brunch = (knot*) calloc(1, sizeof(knot));\
                             tmp->R_brunch->knot_depth = tmp->knot_depth + 1;\
                             tmp->R_brunch->knot_horizontal_position = tmp->knot_horizontal_position + 1;\
                             tmp->R_brunch->prev = tmp;\
                             tmp->R_brunch->L_brunch = NULL;\
                             tmp->R_brunch->R_brunch = NULL;\

#define CHECK_RIGHTEST if (tmp->knot_horizontal_position > rightest_knot) rightest_knot = tmp->knot_horizontal_position 
#define CHECK_LEFTEST if (tmp->knot_horizontal_position < leftest_knot) leftest_knot = tmp->knot_horizontal_position
#define CHECK_DEEPEST if (tmp->knot_depth > depth) depth = tmp->knot_depth

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

// TODO remove typedef
typedef struct knot_struct{

    unsigned char* data;
    struct knot_struct* L_brunch;
    struct knot_struct* R_brunch;
    struct knot_struct* prev;
    int knot_depth;
    int knot_horizontal_position;
} knot;

unsigned char* my_memcpy(unsigned char*);
int length(unsigned char*);
knot* find_elem(unsigned char*, knot*);
void delete_knot(knot* tmp);

class Tree{

public:
    
    // TODO: copy-ctor, move-ctor, operator = , movable operator =
    Tree();

    // TODO: где названия пременных?
    Tree(FILE*);

    ~Tree();

    void dump_tree();

    void create_base_file(char*);//добавить название

    Tree& operator = (const Tree&) = delete;

    Tree(const Tree&) = delete;

protected:

    knot* root;
    int depth;
    int rightest_knot;//Я ЗНАЮ ЧТО ТАКИх СЛОВ НЕТУ
    int leftest_knot; //Я ЗНАЮ ЧТО ТАКИх СЛОВ НЕТУ
    knot* undef_brunch;
    int data_size;
};

class Akinator: public Tree{

public:
    
    Akinator();

    Akinator(FILE*);

    ~Akinator();

    Akinator& operator = (const Akinator&) = delete;

    Akinator(const Akinator&) = delete;

    int guess_and_add();

    int find_answer(const char*);

    void init_data();

    void add_question(knot*);

    void add_leaf(knot*, int);

    void dump();

    void debug();

    void fix_elements_positions(knot*, int, int);

    void selected_elem(knot*);

    void print_path(knot*);

    void show_matches(unsigned char*, unsigned char*);

    void add_to_log(const char*);

    void add_to_log(const char*, unsigned char*);

private:

    FILE* log;
};

#endif