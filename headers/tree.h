//создается файл в который записывается имя на каждом шагу и его ветки; в конструктор может подаваться этот файл
// и при чтении бужет восстанавливаться дерево (надо сделать его двусвязным и подниматься пока нету поворота)
#ifndef TREE
#define TREE

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cstring>
#include <cassert>

#define MAXLEN 100
enum Akinators_code{

    MY_WIN = 0,
    MY_FAULT = 1,
    FIRST_TRY = 2,
};

typedef struct knot_struct{

    char* data;
    struct knot_struct* L_brunch;
    struct knot_struct* R_brunch;
    struct knot_struct* prev;
    int knot_depth;
    int knot_horizontal_position;
} knot;

char* my_memcpy(char*);
int length(char*);

class Tree{

public:

    Tree();

    Tree(FILE*);

    ~Tree();

    void dump_tree();

    void create_base_file(char*);

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

    void init_data();

    void add_question(knot*);

    void add_leaf(knot*, char);

    void dump();

    void add_to_log(const char*);

    void add_to_log(const char*, char*);

private:

    FILE* log;
};

#endif