//создается файл в который записывается имя на каждом шагу и его ветки; в конструктор может подаваться этот файл
// и при чтении бужет восстанавливаться дерево (надо сделать его двусвязным и подниматься пока нету поворота)
#ifndef TREE
#define TREE

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cstring>
#include <cassert>

typedef struct knot_struct{

    char* data;
    struct knot_struct* L_brunch;
    struct knot_struct* R_brunch;
    struct knot_struct* prev;
    int knot_depth;
    int knot_horizontal_position;
} knot;


class Tree{

public:

    Tree();

    Tree(FILE*);

    ~Tree();

    void dump_tree();

    Tree& operator = (const Tree&) = delete;

    Tree(const Tree&) = delete;

private:

    knot* root;
    int depth;
    int rightest_knot;//Я ЗНАЮ ЧТО ТАКИх СЛОВ НЕТУ
    int leftest_knot; //Я ЗНАЮ ЧТО ТАКИх СЛОВ НЕТУ
    knot* undef_brunch;
};

class Akinator: public Tree{

public:

    Akinator();

    ~Akinator();

    Akinator& operator = (const Akinator&) = delete;

    Akinator(const Akinator&) = delete;

    int guess_and_add();

    void dump();

private:

};

#endif