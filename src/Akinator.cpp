#include "headers/Akinator.hpp"
#include <cassert>
#include <cstring>
#include <iostream>

//------------------Конструктор без входного файла(вызывает конструктор дерева)               
Akinator::Akinator(): tree_container(){

    setlocale(LC_ALL, "Russian");
}
//------------------

//------------------Конструктор с входным файлом
Akinator::Akinator(FILE* base): tree_container(base) {

    setlocale(LC_ALL, "Russian");
}
//------------------

//------------------Деструктор(вроде должен вызываться деструктор материнского класса)
Akinator::~Akinator(){}
//------------------

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

//------------------Задание вопроса
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
//------------------

//------------------Добавление предмета
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
//------------------

//------------------Добавление вопроса и ответа
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

        tree_container.add_before_right(old_leaf, attribute);//делает прошлый лист правым
        tree_container.add_L_leaf(tree_container.get_prev_brn(old_leaf), name);
    }

    if (answer == NO){

        tree_container.add_before_left(old_leaf, attribute);
        tree_container.add_R_leaf(tree_container.get_prev_brn(old_leaf), name);
    }
}
//------------------

//------------------Добавление первых данных в пустое дерево
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
//------------------

//------------------Угадывание и добавление в дерево
int Akinator::guess_and_add(){//если да то идем влево
    
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

    } else{

        printf("Не знаю такого предмета\n");
        add_object(prev_knot, answer);
        return MY_FAULT;
    }

    return MY_FAULT;
}
//------------------

void Akinator::debug(){

    printf("\n_______________________________");
    printf("_________________________________");
    printf("_________________________________\n\n");
    printf("Akinator debug mode\n");
    printf("(for more information enter 'info')\n");

    tree_container.debug();
}

