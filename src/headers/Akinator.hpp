#pragma once
#include "Tree.hpp"

knot* find_elem(unsigned char* elem, knot* brunch);

class Akinator: public Tree{

public:
    
    Akinator();

    Akinator(FILE*);

    ~Akinator();

    Akinator& operator = (const Akinator&) = delete;

    Akinator(const Akinator&) = delete;

    int guess_and_add();

    int find_answer(const char*);

    void init_tree_data();

    void add_question(knot*);

    void add_leaf(knot*, int);

    void dump();

    void debug_add(int& loop_status, knot* cur_elem);

    void debug_delete(int& loop_status, knot* cur_elem);

    void debug_merge(int& loop_status, knot* cur_elem, FILE* file);

    void debug_show(int& loop_status);

    void debug();

    void fix_knots_positions(knot*, int, int);

    void selected_elem(knot*);

    void print_path(knot*);

    void find_shared_knot(knot*& link_to_fir_elem, knot*& link_to_sec_elem);

    void print_similarities_and_differences(knot* link_to_fir_elem, knot* link_to_sec_elem);

    void show_matches(unsigned char*, unsigned char*);

    void add_to_log(const char*);

    void add_to_log(const char*, unsigned char*);

private:

    FILE* log;
};