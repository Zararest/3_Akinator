/**
 * @file Akinator.cpp
 * @author Zararest
 * @brief Функции класса Акинатор
 * @version 0.1
 * @date 2021-11-22
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "headers/Akinator.hpp"
#include <cassert>
#include <cstring>
#include <iostream>

/**
 * @brief Construct a new Akinator:: Akinator object
 * 
 */
Akinator::Akinator(): tree_container(){

    setlocale(LC_ALL, "Russian");
}

/**
 * @brief Construct a new Akinator:: Akinator object
 * 
 * @param base 
 */
Akinator::Akinator(FILE* base): tree_container(base) {

    setlocale(LC_ALL, "Russian");
}

bool is_yes(char* answer){
    
    if ((strcmp("y", answer) == 0) || (strcmp("Y", answer) == 0)){
            
        return true;
    }

    return false;
}

bool is_no(char* answer){
    
    if ((strcmp("n", answer) == 0) || (strcmp("N", answer) == 0)){
            
        return true;
    }

    return false;
}

/**
 * @brief Циклический запрос ответа на вопрос
 * 
 * @return Код ответа 
 */
int Akinator::find_answer(){

    unsigned char answer[MAXLEN] = {'!'};

    while (true){

        printf("[Y/N]\n");
        scanf("%s", answer);

        if (strcmp("exit", (char*)answer) == 0){

            exit(0);
        }

        if (strcmp("debug", (char*)answer) == 0){
            
            debug();
            printf("Вспомни прошлый вопрос и ответь на него\n");
        }

        if (is_yes((char*)answer)){
            
            return YES;
        }

        if (is_no((char*)answer)){
            
            return NO;
        }
    }
}

/**
 * @brief Добавление нового персонажа в базу Акинатора после неудачной попытки угадать
 * 
 * @param old_leaf 
 * @param answer 
 */
void Akinator::add_object(knot_ptr old_leaf, int answer){

    unsigned char name[MAXLEN] = {'!'};
    printf("Введи имя своего персонажа:\n");
    scanf("%s", name);

    if (answer == YES){

        add_L_leaf(old_leaf, name);
    }

    if (answer == NO){

        add_R_leaf(old_leaf, name);
    }
}

/**
 * @brief Добавление нового вопроса и персонажа в базу Акинатора после неудачной попытки угадать 
 * 
 * @param old_leaf 
 */
void Akinator::add_question(knot_ptr old_leaf){
    
    unsigned char name[MAXLEN] = {'!'};
    unsigned char attribute[MAXLEN] = {'!'};
    int answer = -1;

    printf("\nКак зовут твоего персонажа?\n");
    scanf("%s", name);

    printf("\nВведи признак, который отличает твоего персонажа от ");
    tree_container.print_data(old_leaf);
    printf(":\n");

    scanf("%s", attribute);

    printf("\nТвой персонаж отвечает этому признаку?\n");
    answer = find_answer();

    if (answer == YES){

        tree_container.add_before_right(old_leaf, attribute);
        tree_container.add_L_leaf(tree_container.get_prev_brn(old_leaf), name);
    }

    if (answer == NO){

        tree_container.add_before_left(old_leaf, attribute);
        tree_container.add_R_leaf(tree_container.get_prev_brn(old_leaf), name);
    }
}
/**
 * @brief Добавление вопроса и персонажа в базу Акинатора, если изначально она была пуста
 * 
 */
void Akinator::init_tree_data(){

    unsigned char attribute[MAXLEN] = {'!'};
    unsigned char name[MAXLEN] = {'!'};
    int answer = -1;
    printf("\nБаза акинатора пуста, поэтому введите первый признак:\n");
    scanf("%s", attribute);

    printf("\nВаш персонаж отвечает этому признаку?\n");
    answer = find_answer();

    printf("\nВведите его имя\n");
    scanf("%s", name);

    if (answer == YES){

        tree_container.add_L_leaf(tree_container.get_root(), name);
    } 

    if (answer == NO){

        tree_container.add_R_leaf(tree_container.get_root(), name);
    }

    tree_container.change_data(tree_container.get_root(), attribute);
}

/**
 * @brief Задание вопросов и обработка ответов на них
 * 
 * @return Исход игры 
 */
int Akinator::guess_and_add(){
    
    knot_ptr cur_knot = tree_container.get_root();
    knot_ptr prev_knot = cur_knot;
    int answer = -1;
    
    if (tree_container.tree_is_empty()){

        init_tree_data();
        return FIRST_TRY;
    }

    while (!is_leaf(cur_knot)){

        printf("\nВаш персонаж ");
        tree_container.print_data(cur_knot);
        printf("?\n");
        answer = find_answer();

        prev_knot = cur_knot;
        if (answer == YES){

            cur_knot = tree_container.get_L_brn(cur_knot);
        }

        if (answer == NO){

            cur_knot = tree_container.get_R_brn(cur_knot);
        }
    }

    if (tree_container.is_valid(cur_knot)){

        printf("\nЭто ");
        tree_container.print_data(cur_knot);
        printf("?\n");
        answer = find_answer();

        if (answer == YES){

            printf("Я победил\n");
            return MY_WIN;
        }

        if (answer == NO){

            add_question(cur_knot);
            return MY_FAULT;  
        }

    } else { 

        printf("Не знаю такого предмета\n");
        add_object(prev_knot, answer);
        return MY_FAULT;
    }

    return MY_FAULT;
}

/**
 * @brief Создание файла с базой Акинатора
 * 
 * @param filename 
 */
void Akinator::create_base_file(char* filename){

    tree_container.create_base_file(filename);
}

/**
 * @brief Вызов режима редактирования дерева
 * 
 */
void Akinator::debug(){

    printf("\n_______________________________");
    printf("_________________________________");
    printf("_________________________________\n\n");
    printf("Akinator debug mode\n");
    printf("(for more information enter 'info')\n");

    tree_container.debug();
}

