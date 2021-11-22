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

class Tree{

private:

    struct knot{

        unsigned char* data = nullptr;
        struct knot* L_brunch = nullptr;
        struct knot* R_brunch = nullptr;
        struct knot* prev = nullptr;
        int knot_depth = 0;
        int knot_horizontal_position = 0;
    };

    knot* root = nullptr;
    int depth = 0;
    int rightest_knot = 0;
    int leftest_knot = 0;

    void init_L_brn_data(knot* cur_knot); 
    void init_R_brn_data(knot* cur_knot);
    void init_knot(knot*& cur_knot, bool l_brn_exist, bool r_brn_exist, knot* undef_brunch);
    void print_knot(knot* cur_knot);
    void add_to_file(knot* tmp, FILE* output);
    void delete_knot(knot* cur_knot);
    knot* find_elem(unsigned char* elem, knot* brunch);
    void print_brunch(int* position_of_line, int number_of_lines, knot* cur_knot);
    int calc_data_size(knot* cur_tree_root);

    void debug_add(int& loop_status, knot* cur_elem);
    void debug_delete(int& loop_status, knot* cur_elem);
    void debug_merge(int& loop_status, knot* cur_elem, FILE* file);
    void debug_show(int& loop_status);

    void fix_knots_positions(knot*, int, int);
    void selected_elem(knot*);
    void print_path(knot*);
    void find_shared_knot(knot*& link_to_fir_elem, knot*& link_to_sec_elem);
    void print_similarities_and_differences(knot* link_to_fir_elem, knot* link_to_sec_elem);

public:
    
    class knot_ptr{

        knot* knot_of_tree = nullptr;
        friend Tree;
    };

    Tree(){ root = new knot; }
    Tree(FILE* base);
    ~Tree();    

    bool is_valid(knot_ptr cur_knot){

        if (cur_knot.knot_of_tree == nullptr) return false;
        return true;
    }
    bool is_leaf(knot_ptr cur_knot){

        if (cur_knot.knot_of_tree == nullptr){ return true; }
        if (   (cur_knot.knot_of_tree->L_brunch != nullptr) 
            || (cur_knot.knot_of_tree->R_brunch != nullptr)){ return false; }

        return true;
    }
    bool tree_is_empty(){

        if (root->data == nullptr){ return true; }
        return false;
    }
    knot_ptr get_root(){
        
        knot_ptr tmp;
        tmp.knot_of_tree = root;
        return tmp; 
    }
    void print_data(knot_ptr cur_knot){

        printf("%s", cur_knot.knot_of_tree->data);
    }
    knot_ptr get_L_brn(knot_ptr cur_knot){

        knot_ptr tmp;
        tmp.knot_of_tree = cur_knot.knot_of_tree->L_brunch;
        return tmp;
    }
    knot_ptr get_R_brn(knot_ptr cur_knot){
        
        knot_ptr tmp;
        tmp.knot_of_tree = cur_knot.knot_of_tree->R_brunch;
        return tmp;
    }
    knot_ptr get_prev_brn(knot_ptr cur_knot){

        knot_ptr tmp;
        tmp.knot_of_tree = cur_knot.knot_of_tree->prev;
        return tmp;
    }
    
    void add_L_leaf(knot_ptr cur_knot, unsigned char* leaf_data);
    void add_R_leaf(knot_ptr cur_knot, unsigned char* leaf_data);
    void add_before_left(knot_ptr cur_knot, unsigned char* leaf_data);
    void add_before_right(knot_ptr cur_knot, unsigned char* leaf_data);
    void change_data(knot_ptr cur_knot, unsigned char* new_data);

    void dump_common();
    void dump();
    void debug();
    void show_matches(unsigned char*, unsigned char*);
    void create_base_file(char* input_name);

    Tree& operator = (const Tree&) = delete;
    Tree(const Tree& old__tree) = delete;

    struct Names_stack{

        Tree::knot** knots = nullptr;
        int size = 0;
        void push(int index, knot* new_knot);
        bool if_left_branch(int index);
        unsigned char* get_name(int index);
        bool is_leaf(int index);
        void destroy();
    };
};

unsigned char* my_memcpy(unsigned char*);
int length(unsigned char*);
