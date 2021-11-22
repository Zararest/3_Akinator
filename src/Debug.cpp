#include "headers/Tree.hpp"
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

/**
 * @brief Копирование одного файла в другой
 * 
 * @param from 
 * @param to 
 */
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

/**
 * @brief Вывод инструкций для режима debug в консоль
 * 
 * @param name 
 */
void print_file(const char* name){

    int fp = open(name, O_RDONLY);
    assert(fp != -1);

    copy_to_stream(fp, STDOUT_FILENO);
}

/**
 * @brief Распечатка содержимого файла как дерева
 * 
 */
void dump_file(){

    char name[MAXLEN] = {'!'};

    scanf("%s", name);
    FILE* base = fopen(name, "rb");
    assert(base != nullptr);

    Tree tmp_tree(base);
    tmp_tree.dump();
}

/**
 * @brief Вывод пути для заданного узла
 * 
 * @param last_elem 
 */
void Tree::print_path(knot* last_elem){
    
    assert(last_elem != nullptr);

    int size_of_stack = last_elem->knot_depth + 1;
    int i = 1;
    Names_stack stack = { new knot*[size_of_stack], size_of_stack };
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

/**
 * @brief Рекурсивное изменение информации о положении узлов
 * 
 * @param cur_knot 
 * @param cur_depth 
 * @param cur_hor_position 
 */
void Tree::fix_knots_positions(knot* cur_knot, int cur_depth, int cur_hor_position){

    if (cur_knot != nullptr){

        cur_knot->knot_depth = cur_depth;
        cur_knot->knot_horizontal_position = cur_hor_position;
        
        fix_knots_positions(cur_knot->R_brunch, cur_depth + 1, cur_hor_position + 1);
        fix_knots_positions(cur_knot->L_brunch, cur_depth + 1, cur_hor_position - 1);

        if (cur_knot->knot_horizontal_position > rightest_knot){ rightest_knot = cur_knot->knot_horizontal_position; }
        if (cur_knot->knot_horizontal_position < leftest_knot){ leftest_knot = cur_knot->knot_horizontal_position; }
        if (cur_knot->knot_depth > depth){ depth = cur_knot->knot_depth; }
    }
}

/**
 * @brief Обработка команд add
 * 
 * @param loop_status 
 * @param cur_elem 
 */
void Tree::debug_add(int& loop_status, knot* cur_elem){

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

/**
 * @brief Удаление листа
 * 
 * @param loop_status 
 * @param cur_elem 
 */
void Tree::debug_delete(int& loop_status, knot* cur_elem){

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

/**
 * @brief Слияние текущего дерева и дерева из указанного файла через выбранную в данный момент вершину
 * 
 * @param loop_status 
 * @param cur_elem 
 * @param second_tree 
 */
void Tree::debug_merge(int& loop_status, knot* cur_elem, FILE* second_tree){

    if (second_tree != nullptr){
                
        Tree tmp_tree(second_tree);

        if (tmp_tree.root->data == nullptr){
            
            printf("\tSecond tree is empty\n");
        } else{

            if ((strcmp((char*)tmp_tree.root->data, (char*)cur_elem->data) == 0)
            && (cur_elem->L_brunch == nullptr) 
            && (cur_elem->R_brunch == nullptr)){

                cur_elem->R_brunch = tmp_tree.root->R_brunch;
                cur_elem->L_brunch = tmp_tree.root->L_brunch;
                cur_elem->R_brunch->prev = cur_elem;
                cur_elem->L_brunch->prev = cur_elem;

                fix_knots_positions(root, 0, 0);

                delete tmp_tree.root->data;
                tmp_tree.root = nullptr;

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

/**
 * @brief Обработка команд, связанных с выбранным элементом
 * 
 * @param cur_elem 
 */
void Tree::selected_elem(knot* cur_elem){

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

/**
 * @brief Обработка команд show
 * 
 * @param loop_status 
 */
void Tree::debug_show(int& loop_status){

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

/**
 * @brief Обработка команд редактирования дерева
 * 
 */
void Tree::debug(){

    char word[MAXLEN] = {'!'};
    unsigned char first_elem[MAXLEN] = {'!'};
    unsigned char second_elem[MAXLEN] = {'!'};
    int loop_status = Init_status;

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
            loop_status = Next_loop_itter;
        }

        if (loop_status == Next_loop_itter){

            loop_status = Init_status;
            scanf("%s", word);
        }
    }
}

/**
 * @brief Рекурсивное нахождение элемента по имени
 * 
 * @param elem 
 * @param brunch 
 * @return Tree::knot* 
 */
Tree::knot* Tree::find_elem(unsigned char* elem, knot* brunch){

    if (brunch != nullptr){

        if (strcmp((char*)brunch->data, (char*)elem) == 0){

            return brunch;
        } else{

            if (find_elem(elem, brunch->R_brunch) != nullptr){

                return find_elem(elem, brunch->R_brunch);
            } else{

                return find_elem(elem, brunch->L_brunch);
            }
        }
    } else{

        return nullptr;
    }
}

/**
 * @brief Нахождение ближайшего общего узла между данными узлами
 * 
 * @param link_to_fir_elem 
 * @param link_to_sec_elem 
 */
void Tree::find_shared_knot(knot*& link_to_fir_elem, knot*& link_to_sec_elem){

    while (link_to_fir_elem->knot_depth != link_to_sec_elem->knot_depth){

        if(link_to_fir_elem->knot_depth > link_to_sec_elem->knot_depth){

            link_to_fir_elem = link_to_fir_elem->prev;
        } else{

            link_to_sec_elem = link_to_sec_elem->prev;
        }
    }

    while (link_to_fir_elem->prev != link_to_sec_elem->prev){

        assert(link_to_fir_elem->prev != nullptr);
        assert(link_to_sec_elem->prev != nullptr);
        link_to_fir_elem = link_to_fir_elem->prev;
        link_to_sec_elem = link_to_sec_elem->prev;
    }
}

/**
 * @brief Вывод сходств и различий данных узлов
 * 
 * @param link_to_fir_elem 
 * @param link_to_sec_elem 
 */
void Tree::print_similarities_and_differences(knot* link_to_fir_elem, knot* link_to_sec_elem){

    unsigned char* fir_elems_name = link_to_fir_elem->data;
    unsigned char* sec_elems_name = link_to_sec_elem->data;

    find_shared_knot(link_to_fir_elem, link_to_sec_elem);
    
    if (link_to_fir_elem->knot_horizontal_position > link_to_sec_elem->knot_horizontal_position){

        std::swap(fir_elems_name, sec_elems_name);
    }

    if ((link_to_fir_elem->prev == root) || (link_to_fir_elem == root)){

        if (link_to_fir_elem->prev == root){

            printf("Персонажи %s и %s не имеют общих черт,", fir_elems_name, sec_elems_name);
            printf("но различаются тем, что %s имеет признак %s, а %s нет\n", fir_elems_name, link_to_fir_elem->prev->data, sec_elems_name);
        } else{

            printf("Один из элементов это корень\n");
        }
        
    } else{ 
    
        printf("Персонаж %s похож на %s тем, что каждый из них: ", fir_elems_name, sec_elems_name);

        link_to_sec_elem = link_to_sec_elem->prev;
        while (link_to_sec_elem != root){
            
            if (link_to_sec_elem->prev->R_brunch == link_to_sec_elem){

                link_to_sec_elem = link_to_sec_elem->prev;
                printf("не %s, ", link_to_sec_elem->data);
            } else{

                link_to_sec_elem = link_to_sec_elem->prev;
                printf("%s, ", link_to_sec_elem->data);
            }
        }

        printf("но различаются тем, что %s имеет признак %s, а %s нет\n", fir_elems_name, link_to_fir_elem->prev->data, sec_elems_name);
    }
}

/**
 * @brief Вывод сходств и различий элементов, соответствующих данным именам
 * 
 * @param fir_elem 
 * @param sec_elem 
 */
void Tree::show_matches(unsigned char* fir_elem, unsigned char* sec_elem){

    knot* link_to_fir_elem = find_elem(fir_elem, root);
    knot* link_to_sec_elem = find_elem(sec_elem, root);

    if (   (link_to_fir_elem == nullptr) 
        || (link_to_sec_elem == nullptr)
        || (link_to_fir_elem == link_to_sec_elem)){

        if ((link_to_fir_elem == link_to_sec_elem) && (link_to_fir_elem != nullptr)){

            printf("Элементы одинаковые\n");
        } else{

            printf("Can't find such elements\n");
        }
        
    } else{

        print_similarities_and_differences(link_to_fir_elem, link_to_sec_elem);
    }
}

void Tree::Names_stack::push(int index, knot* new_knot){

    assert((size - index >= 0) && (size - index < size));
    knots[size - index] = new_knot;
}

bool Tree::Names_stack::if_left_branch(int index){

    assert((index >= 0) && (index < size));
    if (knots[index]->L_brunch == knots[index + 1]){

        return true;
    } else{

        return false;
    }
}

unsigned char* Tree::Names_stack::get_name(int index){

    assert((index >= 0) && (index < size));
    return knots[index]->data;
}

bool Tree::Names_stack::is_leaf(int index){

    assert((index >= 0) && (index < size));
    if ((knots[index]->L_brunch == nullptr) && (knots[index]->R_brunch == nullptr)){

        return true;
    }

    return false;
}

void Tree::Names_stack::destroy(){

    delete[] knots;
}
