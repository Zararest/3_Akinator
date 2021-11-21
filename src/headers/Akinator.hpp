#pragma once
#include "Tree.hpp"

;

class Akinator: public Tree{

private:

    Tree tree_container;

    int find_answer();
    void init_tree_data();
    void add_question(knot_ptr old_leaf);
    void add_object(knot_ptr old_leaf, int answer);

public:
    
    Akinator();
    Akinator(FILE*);
    ~Akinator();

    int guess_and_add();
    void dump();
    void debug();
    
    Akinator& operator = (const Akinator&) = delete;
    Akinator(const Akinator&) = delete;
};