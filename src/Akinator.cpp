#include "headers/Akinator.hpp"
#include <cassert>
#include <cstring>
#include <iostream>

//------------------Конструктор без входного файла(вызывает конструктор дерева)               
Akinator::Akinator():Tree(){

    setlocale(LC_ALL, "Russian");
    
    log = fopen("Akinator_log.txt", "w");
    assert(log != nullptr);
    fprintf(log, "akinator construct was called\n");
}
//------------------

//------------------Конструктор с входным файлом
Akinator::Akinator(FILE* base):Tree(base) {

    log = fopen("Akinator_log.txt", "w");
    assert(log != nullptr);
    add_to_log("Akinator has been created with input data");
}
//------------------

//------------------Добавление сообщения в лог дерева
void Akinator::add_to_log(const char* line){

    fprintf(log, "%s\n", line);
}

void Akinator::add_to_log(const char* line, unsigned char* name){

    fprintf(log, "%s |%s|\n", line, name);
}
//------------------

//------------------Деструктор(вроде должен вызываться деструктор материнского класса)
Akinator::~Akinator(){

    fclose(log);
}
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
int Akinator::find_answer(const char* question){

    unsigned char answer[MAXLEN] = {'!'};
    printf("%s?\n", question);

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
void Akinator::add_leaf(knot* old_leaf, int answer){

    unsigned char name[MAXLEN] = {'!'};
    printf("Введи имя своего персонажа:\n");
    scanf("%s", name);

    assert(old_leaf != nullptr);
    if (answer == YES){

        init_L_brn_data(old_leaf);
        assert(old_leaf->L_brunch != nullptr);
        old_leaf->L_brunch->data = my_memcpy(name);
        
        data_size += length(old_leaf->L_brunch->data) + 6;

        add_to_log("In add leaf item was added to the left brunch:", old_leaf->L_brunch->data);
    }

    if (answer == NO){

        init_R_brn_data(old_leaf);
        assert(old_leaf->R_brunch != nullptr);
        old_leaf->R_brunch->data = my_memcpy(name);
        
        data_size += length(old_leaf->R_brunch->data) + 6;

        add_to_log("In add leaf item was added to the right brunch:", old_leaf->R_brunch->data);
    }
}
//------------------

//------------------Добавление вопроса и ответа
void Akinator::add_question(knot* old_leaf){
    
    unsigned char name[MAXLEN] = {'!'};
    unsigned char attribute[MAXLEN] = {'!'};
    int answer = -1;

    printf("\nКак зовут твоего персонажа?\n");
    scanf("%s", name);

    printf("\nВведи признак, который отличает твоего персонажа от %s:\n", old_leaf->data);
    scanf("%s", attribute);

    answer = find_answer("\nТвой персонаж отвечает этому признаку");

    assert(old_leaf != nullptr);
    if (answer == YES){

        init_L_brn_data(old_leaf);
        old_leaf->L_brunch->data = my_memcpy(name);

        data_size += length(old_leaf->L_brunch->data) + 6;

        init_R_brn_data(old_leaf);
        old_leaf->R_brunch->data = old_leaf->data;

        assert(old_leaf->L_brunch->data != nullptr);
        assert(old_leaf->R_brunch->data != nullptr);
        add_to_log("In add question item was added to the left brunch:", old_leaf->L_brunch->data);
        old_leaf->data = my_memcpy(attribute);
        assert(old_leaf->data != nullptr);

        data_size += length(old_leaf->data) + 6;
    }

    if (answer == NO){

        init_R_brn_data(old_leaf);
        old_leaf->R_brunch->data = my_memcpy(name);

        data_size += length(old_leaf->R_brunch->data) + 6;

        init_L_brn_data(old_leaf);
        old_leaf->L_brunch->data = old_leaf->data;

        assert(old_leaf->L_brunch->data != nullptr);
        assert(old_leaf->R_brunch->data != nullptr);
        add_to_log("In add question item was added to the right brunch:", old_leaf->R_brunch->data);
        old_leaf->data = my_memcpy(attribute);
        assert(old_leaf->data != nullptr);

        data_size += length(old_leaf->data) + 6;
    }
}
//------------------

//------------------Добавление первых данных в пустое дерево
void Akinator::init_tree_data(){

    unsigned char fir_attribute[MAXLEN] = {'!'};
    unsigned char name[MAXLEN] = {'!'};
    int answer = -1;
    printf("\nБаза акинатора пуста, поэтому введите первый признак:\n");
    scanf("%s", fir_attribute);

    answer = find_answer("\nВаш персонаж отвечает этому признаку");

    printf("\nВведите его имя\n");
    scanf("%s", name);

    if (answer == YES){

        init_L_brn_data(root);
        root->L_brunch->data = my_memcpy(name);
        root->data = my_memcpy(fir_attribute);
        assert(root->L_brunch->data != nullptr);
        assert(root->data != nullptr);

        data_size += length(root->L_brunch->data)+ 5 + length(root->data) + 6;

        add_to_log("In init data item was added to the left brunch:", root->L_brunch->data);
    } 

    if (answer == NO){

        init_R_brn_data(root);
        root->R_brunch->data = my_memcpy(name);
        root->data = my_memcpy(fir_attribute);
        assert(root->R_brunch->data != nullptr);
        assert(root->data != nullptr);

        data_size += length(root->R_brunch->data)+ 5 + length(root->data) + 6;

        add_to_log("In init data item was added to the right brunch:", root->R_brunch->data);
    }
}
//------------------

//------------------Угадывание и добавление в дерево
int Akinator::guess_and_add(){//если да то идем влево

    add_to_log("Guess was called:");
    knot* cur_knot = root;
    knot* prev_knot = cur_knot;
    int answer = -1;
    
    if (root->data == nullptr){

        add_to_log("-first init data");
        init_tree_data();
        return FIRST_TRY;
    }

    assert(cur_knot != nullptr);

    while (!is_leaf(cur_knot)){

        printf("\nВаш персонаж ");
        answer = find_answer((char*)cur_knot->data);

        prev_knot = cur_knot;
        if (answer == YES){

            cur_knot = cur_knot->L_brunch;
            add_to_log("-go left");
        }

        if (answer == NO){

            cur_knot = cur_knot->R_brunch;
            add_to_log("-go right");
        }
    }

    if (cur_knot != nullptr){

        printf("\nЭто ");
        answer = find_answer((char*)cur_knot->data);

        if (answer == YES){

            printf("Я победил\n");
            add_to_log("-return |MY_WIN|");
            return MY_WIN;
        }

        if (answer == NO){
                
            add_to_log("-adding question");
            add_question(cur_knot);
            add_to_log("-return |MY_FAULT|");
            return MY_FAULT;  
        }

    } else{

        printf("Не знаю такого предмета\n");
        add_leaf(prev_knot, answer);
        return MY_FAULT;
    }

    return MY_FAULT;
}
//------------------

//------------------Красивый рекурсивный дамп
void print_brunch(int* position_of_line, int number_of_lines, knot* cur_knot){
    
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
        
        printf("%s\n", cur_knot->data);

        position_of_line[number_of_lines] = length(cur_knot->data); 

        position_of_line[i] = abs(position_of_line[i]);
        print_brunch(position_of_line, number_of_lines + 1, cur_knot->R_brunch);
        position_of_line[i] = abs(position_of_line[i]) * (-1);
        print_brunch(position_of_line, number_of_lines + 1, cur_knot->L_brunch);
    }
}

void Akinator::dump(){

    int number_of_lines = 1;
    int* position_of_line = new int[depth + 1];
    position_of_line[0] = length(root->data);

    printf("\nDump of tree:\n");
    printf("%s\n", root->data);

    print_brunch(position_of_line, number_of_lines, root->R_brunch);
    position_of_line[0] = position_of_line[0] * (-1);
    print_brunch(position_of_line, number_of_lines, root->L_brunch);

    delete[] position_of_line;
}
//------------------

//------------------Вывод сходств и различий 
knot* find_elem(unsigned char* elem, knot* brunch){

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

void Akinator::find_shared_knot(knot*& link_to_fir_elem, knot*& link_to_sec_elem){

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

void Akinator::print_similarities_and_differences(knot* link_to_fir_elem, knot* link_to_sec_elem){

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

void Akinator::show_matches(unsigned char* fir_elem, unsigned char* sec_elem){

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
//------------------
