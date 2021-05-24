#include "headers/tree.h"

//------------------Конструктор без входного файла(вызывает конструктор дерева)               
Akinator::Akinator():Tree(){

    setlocale(LC_ALL, "Russian");
    
    log = fopen("Akinator_log.txt", "w");
    assert(log != NULL);
    fprintf(log, "akinator construct was called\n");
    //log = stdout;
}
//------------------

//------------------Конструктор с входным файлом
Akinator::Akinator(FILE* base):Tree(base) {

    log = fopen("Akinator_log.txt", "w");
    assert(log != NULL);
    //log = stdout;
    add_to_log("Akinator has been created with input data");
}
//------------------

//------------------Добавление сооьщения в лог дерева
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

//------------------Добавление предмета
void Akinator::add_leaf(knot* old_leaf, unsigned char answer){

    unsigned char name[MAXLEN] = {'!'};
    printf("Введи имя своего персонажа:\n");
    scanf("%s", name);

    assert(old_leaf != NULL);
    if ((answer == 'Y') || (answer == 'y')){

        INIT_L_BRN_DATA(old_leaf);
        assert(old_leaf->L_brunch != NULL);
        old_leaf->L_brunch->data = my_memcpy(name);
        
        data_size += length(old_leaf->L_brunch->data) + 6;

        add_to_log("In add leaf item was added to the left brunch:", old_leaf->L_brunch->data);
    }

    if ((answer == 'N') || (answer == 'n')){

        INIT_R_BRN_DATA(old_leaf);
        assert(old_leaf->R_brunch != NULL);
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
    unsigned char answer[MAXLEN] = {'!'};

    printf("\nКак зовут твоего персонажа?\n");
    scanf("%s", name);

    printf("\nВведи признак, который отличает твоего персонажа от %s:\n", old_leaf->data);
    scanf("%s", attribute);

    printf("\nТвой персонаж отвечает этому признаку? ");
    printf("[Y/N]\n");
    scanf("%s", answer);

    assert(old_leaf != NULL);
    if ((answer[0] == 'Y') || (answer[0] == 'y')){

        INIT_L_BRN_DATA(old_leaf);
        old_leaf->L_brunch->data = my_memcpy(name);

        data_size += length(old_leaf->L_brunch->data) + 6;

        INIT_R_BRN_DATA(old_leaf);
        old_leaf->R_brunch->data = old_leaf->data;

        assert(old_leaf->L_brunch->data != NULL);
        assert(old_leaf->R_brunch->data != NULL);
        add_to_log("In add question item was added to the left brunch:", old_leaf->L_brunch->data);
        old_leaf->data = my_memcpy(attribute);
        assert(old_leaf->data != NULL);

        data_size += length(old_leaf->data) + 6;

    } else{

        if ((answer[0] == 'N') || (answer[0] == 'n')){

            INIT_R_BRN_DATA(old_leaf);
            old_leaf->R_brunch->data = my_memcpy(name);

            data_size += length(old_leaf->R_brunch->data) + 6;

            INIT_L_BRN_DATA(old_leaf);
            old_leaf->L_brunch->data = old_leaf->data;

            assert(old_leaf->L_brunch->data != NULL);
            assert(old_leaf->R_brunch->data != NULL);
            add_to_log("In add question item was added to the right brunch:", old_leaf->R_brunch->data);
            old_leaf->data = my_memcpy(attribute);
            assert(old_leaf->data != NULL);

            data_size += length(old_leaf->data) + 6;
        } else{

            printf("\nНеправильный ответ, поэтому ты дисквалификацирован\n");
            printf("Я победил(наверное)\n");
        }
    }
}
//------------------

//------------------Добавление первых данных в пустое дерево
void Akinator::init_data(){

    unsigned char fir_attribute[MAXLEN] = {'!'};
    unsigned char name[MAXLEN] = {'!'};
    unsigned char answer[MAXLEN] = {'!'};
    printf("\nБаза акинатора пуста, поэтому введите первый признак:\n");
    scanf("%s", fir_attribute);

    printf("\nВаш персонаж отвечает этому признаку? ");
    printf("[Y/N]\n");
    scanf("%s", answer);

    printf("\nВведите его имя\n");
    scanf("%s", name);

    if ((answer[0] == 'Y') || (answer[0] == 'y')){

        INIT_L_BRN_DATA(root);
        root->L_brunch->data = my_memcpy(name);
        root->data = my_memcpy(fir_attribute);
        assert(root->L_brunch->data != NULL);
        assert(root->data != NULL);

        data_size += length(root->L_brunch->data)+ 5 + length(root->data) + 6;

        add_to_log("In init data item was added to the left brunch:", root->L_brunch->data);
    } else{

        if ((answer[0] == 'N') || (answer[0] == 'n')){

            INIT_R_BRN_DATA(root);
            root->R_brunch->data = my_memcpy(name);
            root->data = my_memcpy(fir_attribute);
            assert(root->R_brunch->data != NULL);
            assert(root->data != NULL);

            data_size += length(root->R_brunch->data)+ 5 + length(root->data) + 6;

            add_to_log("In init data item was added to the right brunch:", root->R_brunch->data);
        } else{
            
            printf("Неправильный ответ\n");
        }
    }
}
//------------------

//------------------Угадывание и добавление в дерево
int Akinator::guess_and_add(){//если да то идем влево

    add_to_log("Guess was called:");
    knot* tmp = root;
    knot* prev_knot = tmp;
    unsigned char answer[MAXLEN] = {'!'};
    
    if (root->data == NULL){

        add_to_log("-first init data");
        init_data();
        return FIRST_TRY;
    }

    assert(tmp != NULL);
    int tmp_has_brn = ((tmp->L_brunch != NULL) || (tmp->R_brunch != NULL));

    while ( tmp_has_brn && (tmp != NULL)){

        printf("\nВаш персонаж %s? ", tmp->data);
        printf("[Y/N]?\n");
        scanf("%s", answer);

        prev_knot = tmp;
        if ((answer[0] == 'Y') || (answer[0] == 'y')){//добавить в функцию 

            tmp = tmp->L_brunch;
            add_to_log("-go left");
        } else{

            if ((answer[0] == 'N') || (answer[0] == 'n')){

                tmp = tmp->R_brunch;
                add_to_log("-go right");
            } else{

                printf("\nНеправильный ответ\n");
            }
        }
        
        if (tmp == NULL){

            tmp_has_brn = 0;
        } else{

            tmp_has_brn = ((tmp->L_brunch != NULL) || (tmp->R_brunch != NULL));
        }
    }

    if (tmp != NULL){

        printf("\nЭто %s?", tmp->data);
        printf("[Y/N]?\n");
        scanf("%s", answer);

        add_to_log("-attempt to guess answer:", answer);
        if ((answer[0] == 'Y') || (answer[0] == 'y')){

            printf("Я победил\n");
            add_to_log("-return |MY_WIN|");
            return MY_WIN;
        } else{

            if ((answer[0] == 'N') || (answer[0] == 'n')){
                
                add_to_log("-adding question");
                add_question(tmp);
                add_to_log("-return |MY_FAULT|");
                return MY_FAULT;
            } else{

                printf("\nНеправильный ответ, поэтому ты дисквалификацирован\n");
                printf("Я победил\n");
                add_to_log("-return |MY_WIN|");
                return MY_WIN;
            }
        }

    } else{

        //printf("Хз, что ты загадал\n");
        printf("Не знаю такоо прежмета\n");
        add_to_log("-adding leaf with answer:", answer);
        add_leaf(prev_knot, answer[0]);
        return MY_FAULT;
    }
}
//------------------

//------------------Красивый рекурсивный дамп
void print_brunch(int* position_of_line, int number_of_lines, knot* elem){
    
    int i = 0;

    if (elem != NULL){

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
        
        printf("%s\n", elem->data);

        position_of_line[number_of_lines] = length(elem->data) / 2; //надо сделать русскую версию в которой тут будет / 2

        position_of_line[i] = abs(position_of_line[i]);
        print_brunch(position_of_line, number_of_lines + 1, elem->R_brunch);
        position_of_line[i] = abs(position_of_line[i]) * (-1);
        print_brunch(position_of_line, number_of_lines + 1, elem->L_brunch);
    }
}

void Akinator::dump(){

    int number_of_lines = 1;
    int* position_of_line = (int*) calloc(depth + 1, sizeof(int));
    position_of_line[0] = length(root->data);

    printf("\nDump of tree:\n");
    printf("--structure of dump--\n");
    printf("root\n");
    printf("    |__(NO brunch)\n");
    printf("    |            |\n");
    printf("   ...          ...\n");
    printf("    |__(YES brunch)\n");
    printf("                  |\n");
    printf("                 ...\n");
    printf("---------------------\n");

    printf("%s\n", root->data);
    print_brunch(position_of_line, number_of_lines, root->R_brunch);
    position_of_line[0] = position_of_line[0] * (-1);
    print_brunch(position_of_line, number_of_lines, root->L_brunch);

    free(position_of_line);
}
//------------------

//------------------Вывод сходств и различий
knot* find_elem(unsigned char* elem, knot* brunch){

    if (brunch != NULL){

        if (strcmp((char*)brunch->data, (char*)elem) == 0){

            return brunch;
        } else{

            if (find_elem(elem, brunch->R_brunch) != NULL){

                return find_elem(elem, brunch->R_brunch);
            } else{

                return find_elem(elem, brunch->L_brunch);
            }
        }
    } else{

        return NULL;
    }
}

void Akinator::show_matches(unsigned char* fir_elem, unsigned char* sec_elem){

    knot* link_to_fir_elem = find_elem(fir_elem, root);
    knot* link_to_sec_elem = find_elem(sec_elem, root);
    assert(link_to_fir_elem != NULL);
    assert(link_to_sec_elem != NULL);

    while (link_to_fir_elem->knot_depth != link_to_sec_elem->knot_depth){

        if(link_to_fir_elem->knot_depth > link_to_sec_elem->knot_depth){

            link_to_fir_elem = link_to_fir_elem->prev;
        } else{

            link_to_sec_elem = link_to_sec_elem->prev;
        }
    }

    while (link_to_fir_elem->prev != link_to_sec_elem->prev){

        assert(link_to_fir_elem->prev != NULL);
        assert(link_to_sec_elem->prev != NULL);
        link_to_fir_elem = link_to_fir_elem->prev;
        link_to_sec_elem = link_to_sec_elem->prev;
    }

    if (link_to_fir_elem->prev == root){

        if (link_to_fir_elem->knot_horizontal_position > link_to_sec_elem->knot_horizontal_position){

            unsigned char* tmp = fir_elem;
            fir_elem = sec_elem;
            sec_elem = tmp;            
        }
        printf("Персонажи %s и %s не имеют общих черт,", fir_elem, sec_elem);
        printf("но различаются тем, что %s имеет признак %s, а %s нет\n", fir_elem, link_to_fir_elem->prev->data, sec_elem);
    } else{
        
        int sent_has_and = 0;
        printf("Персонаж %s похож на %s тем, что каждый из них", fir_elem, sec_elem);

        while (link_to_sec_elem->prev != root){

            printf("%s, ", link_to_sec_elem->data);
            sent_has_and = 1;
        }

        if (sent_has_and == 1){

            printf(" и "); ///!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!дописать
        }
    }
}
//------------------
