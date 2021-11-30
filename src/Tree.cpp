/**
 * @file Tree.cpp
 * @author Zararest
 * @brief 
 * @version 0.1
 * @date 2021-11-22
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "headers/Tree.hpp"

#include <cassert>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>

#define WORKING_DATA_SIZE 6

/**
 * @brief Вычисление размера буфера для записи базы Акинатора в файл
 * 
 * @param cur_tree_root 
 * @return размер буфера 
 */
int Tree::calc_data_size(knot* cur_tree_root){

    int size_of_tree = 0;

    if (cur_tree_root != nullptr){

        size_of_tree += calc_data_size(cur_tree_root->L_brunch);
        size_of_tree += calc_data_size(cur_tree_root->R_brunch);
        return size_of_tree + WORKING_DATA_SIZE + strlen((char*)cur_tree_root->data);
    }
    return 0;
}

/**
 * @brief Создание пустого левого листа
 * 
 * @param tmp 
 */
void Tree::init_L_brn_data(knot* cur_knot){

    cur_knot->L_brunch = new knot;
    cur_knot->L_brunch->knot_depth = cur_knot->knot_depth + 1;
    cur_knot->L_brunch->knot_horizontal_position = cur_knot->knot_horizontal_position - 1;
    cur_knot->L_brunch->prev = cur_knot;
}

/**
 * @brief Создание пустого правого листа
 * 
 * @param cur_knot 
 */
void Tree::init_R_brn_data(knot* cur_knot){

    cur_knot->R_brunch = new knot;
    cur_knot->R_brunch->knot_depth = cur_knot->knot_depth + 1;
    cur_knot->R_brunch->knot_horizontal_position = cur_knot->knot_horizontal_position - 1;
    cur_knot->R_brunch->prev = cur_knot;
}

/**
 * @brief Длина строки с учетом юникода
 * 
 * @param line 
 * @return int 
 */
int length(unsigned char* line){

    if (line == nullptr){ return 0; }

    int i = 0, len = 0;
    while (line[i] != '\0'){

        len++;
        i += 1 + ((line[i] & 0x80) == 128); 
    }

    return len;
}

/**
 * @brief Выделение и копирование памяти
 * 
 * @param line 
 * @return unsigned* char
 */
unsigned char* my_memcpy(unsigned char* line){

    if (line == nullptr){
        return nullptr;
    }

    int len = 0;
    unsigned char* new_line = nullptr;

    while (line[len] != '\0'){
        len++;
    }

    new_line = new unsigned char[len + 1];

    for(int i = 0; i <= len; i++){
        new_line[i] = line[i];
    }

    return new_line;
}

/**
 * @brief Создание узла дерева при создании по файлу
 * 
 * @param cur_knot 
 * @param l_brn_exist 
 * @param r_brn_exist 
 * @param undef_brunch 
 */
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

        if (cur_knot->R_brunch == undef_brunch){
            
            init_R_brn_data(cur_knot);
            cur_knot = cur_knot->R_brunch;
        }
    }
}

/**
 * @brief Construct a new Tree:: Tree object
 * 
 * @param base 
 */
Tree::Tree(FILE* base){
    
    assert(base != nullptr);
    root = new knot;

    int data_size = 0;
    fread(&data_size, sizeof(int), 1, base);
    unsigned char* buffer = new unsigned char[data_size + 1];
    buffer[data_size] = '\0';
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

/**
 * @brief Рекурсивная распечатка поддерева
 * 
 * @param cur_knot 
 */
void Tree::print_knot(knot* cur_knot){

    if (cur_knot != nullptr){

        print_knot(cur_knot->L_brunch);
        printf("%s[%i][%i]\n", cur_knot->data, cur_knot->knot_horizontal_position, cur_knot->knot_depth);
        print_knot(cur_knot->R_brunch);
    }
}

/**
 * @brief Рекурсивный вывод дерева
 * 
 */
void Tree::dump_common(){

    printf("Dumped tree: *name*[horizontal_position][depth]\n");
    print_knot(root);
}

/**
 * @brief Запись дерева в файл
 * 
 * @param cur_knot 
 * @param output 
 */
void Tree::add_to_file(knot* cur_knot, FILE* output){

    if (cur_knot != nullptr){
        
        fwrite(cur_knot->data, sizeof(char), strlen((char*)cur_knot->data), output);
        
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

/**
 * @brief Создание файла для записи в него дерева
 * 
 * @param name_of_file 
 */
void Tree::create_base_file(char* name_of_file){

    FILE* output = fopen(name_of_file, "wb");
    assert(output != nullptr);
    int data_size = calc_data_size(root);

    fwrite(&data_size, sizeof(int), 1, output);
    add_to_file(root, output);

    fclose(output);
}

/**
 * @brief Рекурсивное удаление поддерева
 * 
 * @param cur_knot 
 */
void Tree::delete_knot(knot* cur_knot){

    if (cur_knot != nullptr){

        delete_knot(cur_knot->L_brunch);
        delete_knot(cur_knot->R_brunch);

        delete[] cur_knot->data;
        delete cur_knot;
    }
}

/**
 * @brief Destroy the Tree:: Tree object
 * 
 */
Tree::~Tree(){

    delete_knot(root);
}

/**
 * @brief Добавление узла с leaf_data до cur_knot. cur_knot становится левой веткой для нового узла
 * 
 * @param cur_knot 
 * @param leaf_data 
 */
void Tree::add_before_left(knot_ptr cur_knot, unsigned char* leaf_data){
    
    assert(cur_knot.knot_of_tree != nullptr);
    knot* prev_knot = cur_knot.knot_of_tree->prev;
    knot* new_knot = new knot;
    new_knot->data = my_memcpy(leaf_data);

    new_knot->L_brunch = cur_knot.knot_of_tree;
    cur_knot.knot_of_tree->prev = new_knot;

    if (prev_knot != nullptr){

        if (prev_knot->L_brunch == cur_knot.knot_of_tree){

            prev_knot->L_brunch = new_knot;
        } else{

            prev_knot->R_brunch = new_knot;
        }
        new_knot->prev = prev_knot;

        fix_knots_positions(new_knot, prev_knot->knot_depth + 1, prev_knot->knot_horizontal_position - 1);
    } else{
        
        assert(cur_knot.knot_of_tree == root);
        root = new_knot;
        fix_knots_positions(new_knot, 0, 0);
    }
}

/**
 * @brief Добавление узла с leaf_data до cur_knot. cur_knot становится правой веткой для нового узла
 * 
 * @param cur_knot 
 * @param leaf_data 
 */
void Tree::add_before_right(knot_ptr cur_knot, unsigned char* leaf_data){

    assert(cur_knot.knot_of_tree != nullptr);
    knot* prev_knot = cur_knot.knot_of_tree->prev;
    knot* new_knot = new knot;
    new_knot->data = my_memcpy(leaf_data);

    new_knot->R_brunch = cur_knot.knot_of_tree;
    cur_knot.knot_of_tree->prev = new_knot;

    if (prev_knot != nullptr){

        if (prev_knot->L_brunch == cur_knot.knot_of_tree){

            prev_knot->L_brunch = new_knot;
        } else{

            prev_knot->R_brunch = new_knot;
        }
        new_knot->prev = prev_knot;

        fix_knots_positions(new_knot, prev_knot->knot_depth + 1, prev_knot->knot_horizontal_position + 1);
    } else{
        
        assert(cur_knot.knot_of_tree == root);
        root = new_knot;
        fix_knots_positions(new_knot, 0, 0);
    }
}

/**
 * @brief Изменение данных в узле
 * 
 * @param cur_knot 
 * @param new_data 
 */
void Tree::change_data(knot_ptr cur_knot, unsigned char* new_data){

    delete cur_knot.knot_of_tree->data;
    cur_knot.knot_of_tree->data = my_memcpy(new_data);
}

/**
 * @brief Добавление листа слева
 * 
 * @param cur_knot 
 * @param leaf_data 
 */
void Tree::add_L_leaf(knot_ptr cur_knot, unsigned char* leaf_data){

    init_L_brn_data(cur_knot.knot_of_tree);//добавить проверку на лист
    cur_knot.knot_of_tree->L_brunch->data = my_memcpy(leaf_data);
}

/**
 * @brief Добавление узла справа
 * 
 * @param cur_knot 
 * @param leaf_data 
 */
void Tree::add_R_leaf(knot_ptr cur_knot, unsigned char* leaf_data){

    init_R_brn_data(cur_knot.knot_of_tree);
    cur_knot.knot_of_tree->R_brunch->data = my_memcpy(leaf_data);
}

/**
 * @brief Рекурсивная распечатка с указанием связей между узлами 
 * 
 * @param position_of_line 
 * @param number_of_lines 
 * @param cur_knot 
 */
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

/**
 * @brief Рекурсивная распечатка узла для graphviz
 * 
 * @param cur_knot 
 * @param outp_file 
 */
void Tree::print_knot_graphviz(knot* cur_knot, FILE* outp_file){

    if (cur_knot->prev != nullptr){ //это убрать

        fprintf(outp_file, "%s -> %s", cur_knot->data, cur_knot->prev->data);
    }

    if ((cur_knot->L_brunch == nullptr) && (cur_knot->R_brunch == nullptr)){

        fprintf(outp_file, "%s[fillcolor=""red""]\n", cur_knot->data);
    } else{

        if (cur_knot->L_brunch != nullptr){

            fprintf(outp_file, "%s -> %s\n", cur_knot->data, cur_knot->L_brunch->data);
            print_knot_graphviz(cur_knot->L_brunch, outp_file);
        }

        if (cur_knot->R_brunch != nullptr){

            fprintf(outp_file, "%s -> %s\n", cur_knot->data, cur_knot->R_brunch->data);
            print_knot_graphviz(cur_knot->R_brunch, outp_file);
        }
    }
}

/**
 * @brief Вывод дерева в файл для построения графа через graphviz
 * 
 * @param outp_file 
 */
void Tree::dump_graphviz(FILE* outp_file){

    assert(outp_file != nullptr);

    fprintf(outp_file, "digraph Dump{\n");
    
    fprintf(outp_file, "node[color=""red"",fontsize=14, style=""filled"",fillcolor=""lightgrey""]\n");
    fprintf(outp_file, "%s[shape=""rectangle""]\n", root->data);
    
    if (root->L_brunch != nullptr){
        fprintf(outp_file, "%s -> %s\n", root->data, root->L_brunch->data);
        print_knot_graphviz(root->L_brunch, outp_file);
    }
    if (root->R_brunch != nullptr){
        fprintf(outp_file, "%s -> %s\n", root->data, root->R_brunch->data);
        print_knot_graphviz(root->R_brunch, outp_file);
    }
    fprintf(outp_file, "}\n");
}

/**
 * @brief Вывод дерева в консоль со связями между узлами
 * 
 */
void Tree::dump(){

    assert(root != nullptr);

    char name[MAXLEN] = {'!'};
    int number_of_lines = 1;
    int* position_of_line = new int[depth + 1];
    position_of_line[0] = length(root->data);

    printf("\nDump of tree:\n");
    if (root-> data != nullptr){

        printf("Укажите название файла в который будет выведена информация для graphviz.(Если хотите вывести дерево в консоль введите N)\n");

        scanf("%s", name);
        if (strcmp(name, "N") != 0){

            FILE* dump_file = fopen(name, "w");

            if (dump_file != nullptr){

                dump_graphviz(dump_file);
                fclose(dump_file);
            } else{

                printf("Невозможно открыть файл\n");
            }
        } else{

            printf("%s\n", root->data);
            print_brunch(position_of_line, number_of_lines, root->R_brunch);
            position_of_line[0] = position_of_line[0] * (-1);
            print_brunch(position_of_line, number_of_lines, root->L_brunch);
        }
        
    } else{

        printf("nothing\n");
    }

    delete[] position_of_line;
}

