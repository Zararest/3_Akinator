#include "headers/Akinator.hpp"
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <cassert>
#include <cstring>

#define SIZE_OF_SEGMENT 64

enum Loop_problems{

    Init_status = 0,
    Next_loop_itter = 2,
    Drop_knot = 1
};

void copy_to_stream(int from, int to){

    int read_ret = -1;
    char segment[SIZE_OF_SEGMENT];

    while ((read_ret = read(from, segment, SIZE_OF_SEGMENT)) != 0){
        
        if (read_ret == -1){

            printf("Problems with pipe\n");
            exit(0);
        }
        write(to, segment, read_ret);
    }
}

void print_file(const char* name){

    int fp = open(name, O_RDONLY);
    assert(fp != -1);

    copy_to_stream(fp, STDOUT_FILENO);
}

void dump_file(){

    char name[MAXLEN] = {'!'};

    scanf("%s", name);
    FILE* base = fopen(name, "rb");
    assert(base != nullptr);

    Akinator tmp_Akinator(base);
    tmp_Akinator.dump();
}

struct Names_satck{

    knot** knots = nullptr;
    int size = 0;

    void push(int index, knot* new_knot){

        assert((size - index >= 0) && (size - index < size));
        knots[size - index] = new_knot;
    }

    bool if_left_branch(int index){

        assert((index >= 0) && (index < size));
        if (knots[index]->L_brunch == knots[index + 1]){

            return true;
        } else{

            return false;
        }
    }

    unsigned char* get_name(int index){

        assert((index >= 0) && (index < size));
        return knots[index]->data;
    }

    bool is_leaf(int index){

        assert((index >= 0) && (index < size));
        if ((knots[index]->L_brunch == nullptr) && (knots[index]->R_brunch == nullptr)){

            return true;
        } else{

            return false;
        }
    }

    void destroy(){

        delete[] knots;
    }
};

void Akinator::print_path(knot* last_elem){
    
    assert(last_elem != nullptr);

    int size_of_stack = last_elem->knot_depth + 1;
    int i = 1;
    Names_satck stack = { new knot*[size_of_stack], size_of_stack };
    knot* cur_knot = last_elem;
    
    while (cur_knot != root){

        stack.push(i, cur_knot);
        cur_knot = cur_knot->prev;
        i++;
    }
    stack.push(size_of_stack, root);

    for (i = 0; i < size_of_stack - 1; i++){

        assert(stack.knots[i]->data != nullptr);
        if (stack.if_left_branch(i)){

            printf("%s --> ", stack.get_name(i));
        } else{

            printf("не %s --> ", stack.get_name(i));
        }  
    }
    
    if (stack.is_leaf(size_of_stack - 1)){

        assert(stack.knots[size_of_stack - 1]->data != nullptr);
        printf("%s --> nullptr\n", stack.get_name(size_of_stack - 1));
    } else{
        printf("...\n");
    }
    
    stack.destroy();
}

void Akinator::fix_knots_positions(knot* tmp, int cur_depth, int cur_hor_position){

    if (tmp != nullptr){

        tmp->knot_depth = cur_depth;
        tmp->knot_horizontal_position = cur_hor_position;
        
        fix_knots_positions(tmp->R_brunch, cur_depth + 1, cur_hor_position + 1);
        fix_knots_positions(tmp->L_brunch, cur_depth + 1, cur_hor_position - 1);

        if (tmp->knot_horizontal_position > rightest_knot) rightest_knot = tmp->knot_horizontal_position ;
        if (tmp->knot_horizontal_position < leftest_knot) leftest_knot = tmp->knot_horizontal_position;
        if (tmp->knot_depth > depth) depth = tmp->knot_depth;
    }
}

void Akinator::debug_add(int& loop_status, knot* cur_elem){

    char word[MAXLEN] = {'!'};
    unsigned char first_elem[MAXLEN] = {'!'};
    knot* tmp_knot = nullptr;

    scanf("%s", word);

    if ((strcmp(word, "before") == 0) && (cur_elem != root)){

        scanf("%s", first_elem);
        
        if (cur_elem->prev->R_brunch == cur_elem){
            
            init_R_brn_data(cur_elem->prev);
            cur_elem->prev->R_brunch->data = my_memcpy(first_elem);
            cur_elem->prev->R_brunch->R_brunch = cur_elem;
            cur_elem->prev = cur_elem->prev->R_brunch;

            fix_knots_positions(cur_elem, cur_elem->knot_depth + 1, cur_elem->knot_horizontal_position + 1);
        } else{
            
            init_L_brn_data(cur_elem->prev);
            cur_elem->prev->L_brunch->data = my_memcpy(first_elem);
            cur_elem->prev->L_brunch->L_brunch = cur_elem;
            cur_elem->prev = cur_elem->prev->L_brunch;

            fix_knots_positions(cur_elem, cur_elem->knot_depth + 1, cur_elem->knot_horizontal_position - 1);
        }

        loop_status = Next_loop_itter;
    } else{

        if (strcmp(word, "before") == 0){

            printf("\tCan't add before root\n");
        }

        loop_status = Next_loop_itter;
    }

    if (strcmp(word, "right") == 0){

        scanf("%s", first_elem);

        tmp_knot = cur_elem->R_brunch;
        init_R_brn_data(cur_elem);
        cur_elem->R_brunch->data = my_memcpy(first_elem);
        cur_elem->R_brunch->R_brunch = tmp_knot;

        if (tmp_knot != nullptr){

            tmp_knot->prev = cur_elem->R_brunch;
        }
        
        fix_knots_positions(cur_elem->R_brunch, cur_elem->knot_depth + 1, cur_elem->knot_horizontal_position + 1);
        loop_status = Next_loop_itter;
    }

    if (strcmp(word, "left") == 0){

        scanf("%s", first_elem);

        tmp_knot = cur_elem->L_brunch;
        init_L_brn_data(cur_elem);
        cur_elem->L_brunch->data = my_memcpy(first_elem);
        cur_elem->L_brunch->L_brunch = tmp_knot;

        if (tmp_knot != nullptr){

            tmp_knot->prev = cur_elem->L_brunch;
        }

        fix_knots_positions(cur_elem->L_brunch, cur_elem->knot_depth + 1, cur_elem->knot_horizontal_position - 1);
        loop_status = Next_loop_itter;
    }

    if (loop_status == 0){

        printf("strange command for add\n");
        loop_status = Next_loop_itter;
    }
}

void Akinator::debug_delete(int& loop_status, knot* cur_elem){

    if  ((cur_elem->R_brunch == nullptr) && (cur_elem->L_brunch == nullptr)){

        delete cur_elem->data;

        if (cur_elem->prev->R_brunch == cur_elem){

            cur_elem->prev->R_brunch = nullptr;
        } else{

            cur_elem->prev->L_brunch = nullptr;
        }

        delete cur_elem;

        loop_status = Drop_knot;
    } else{

        printf("\tCan't delete brunch with descendants\n");

        loop_status = Next_loop_itter;
    }
}

void Akinator::debug_merge(int& loop_status, knot* cur_elem, FILE* second_tree){

    if (second_tree != nullptr){
                
        Akinator tmp_Akinator(second_tree);

        if (tmp_Akinator.root->data == nullptr){
            
            printf("\tSecond tree is empty\n");
        } else{

            if ((strcmp((char*)tmp_Akinator.root->data, (char*)cur_elem->data) == 0)
            && (cur_elem->L_brunch == nullptr) 
            && (cur_elem->R_brunch == nullptr)){

                cur_elem->R_brunch = tmp_Akinator.root->R_brunch;
                cur_elem->L_brunch = tmp_Akinator.root->L_brunch;
                cur_elem->R_brunch->prev = cur_elem;
                cur_elem->L_brunch->prev = cur_elem;

                data_size += tmp_Akinator.data_size;
                fix_knots_positions(root, 0, 0);

                delete tmp_Akinator.root->data;
                tmp_Akinator.root = nullptr;

            } else{

                printf("\tElemnts don't fit\n");
            }
        }

        loop_status = Next_loop_itter;
    } else{

        printf("\tCan't open file\n");
        loop_status = Next_loop_itter;
    }
}

void Akinator::selected_elem(knot* cur_elem){

    char word[MAXLEN] = {'!'};
    unsigned char first_elem[MAXLEN] = {'!'};
    knot* cur_knot = nullptr;
    int loop_status = Init_status;

    printf("\t");
    scanf("%s", word);

    while (loop_status == Init_status){

        if (strcmp(word, "add") == 0){

            debug_add(loop_status, cur_elem);
        }

        if (strcmp(word, "delete") == 0){

            debug_delete(loop_status, cur_elem);
        }

        if (strcmp(word, "change") == 0){

            scanf("%s", first_elem);

            delete cur_elem->data;
            cur_elem->data = my_memcpy(first_elem);

            loop_status = Next_loop_itter;
        }

        if (strcmp(word, "merge") == 0){

            scanf("%s", word);

            FILE* second_tree = fopen(word, "rb");

            debug_merge(loop_status, cur_elem, second_tree);            
        }

        if (strcmp(word, "drop") == 0){

            loop_status = Drop_knot;
        }

        if (loop_status == Init_status){

            printf("\tStrange command\n");
        }

        if (loop_status == Next_loop_itter){

            printf("\t");
            scanf("%s", word);
            loop_status = Init_status;
        }
    }
}

void Akinator::debug_show(int& loop_status){

    char word[MAXLEN] = {'!'};
    unsigned char first_elem[MAXLEN] = {'!'};
    unsigned char second_elem[MAXLEN] = {'!'};

    scanf("%s", word);

    if (strcmp(word, "tree") == 0){

        dump();
    }

    if (strcmp(word, "file") == 0){

        dump_file();
    }

    if (strcmp(word, "differences") == 0){

        scanf("%s", first_elem);
        scanf("%s", second_elem);
        show_matches(first_elem, second_elem);
    }

    if (strcmp(word, "path") == 0){

        scanf("%s", first_elem);

        if (find_elem(first_elem, root) != nullptr){

            print_path(find_elem(first_elem, root));
        } else{

            printf("Can't find such an element\n");
        }
    }

    loop_status = Next_loop_itter;
}

void Akinator::debug(){

    char word[MAXLEN] = {'!'};
    unsigned char first_elem[MAXLEN] = {'!'};
    unsigned char second_elem[MAXLEN] = {'!'};
    int loop_status = Init_status;

    printf("\n_______________________________");
    printf("_________________________________");
    printf("_________________________________\n\n");
    printf("Akinator debug mode\n");
    printf("(for more information enter 'info')\n");

    scanf("%s", word);

    while (loop_status == Init_status){

        if (strcmp(word, "info") == 0){

            print_file("../bin/instructions.txt");
            loop_status = Next_loop_itter;
        }

        if (strcmp(word, "show") == 0){

            debug_show(loop_status);
        }

        if (strcmp(word, "select") == 0){

            scanf("%s", first_elem);

            if (find_elem(first_elem, root) != nullptr){

                selected_elem(find_elem(first_elem, root));
            } else{

                printf("Can't find elem %s\n", first_elem);
            }
            
            loop_status = Next_loop_itter;
        }

        if (strcmp(word, "stop") == 0){

            loop_status = Drop_knot;
        }

        if (loop_status == Init_status){

            printf("Strange command\n");
        }

        if (loop_status == Next_loop_itter){

            loop_status = Init_status;
            
        }
        
        scanf("%s", word);
    }
}

