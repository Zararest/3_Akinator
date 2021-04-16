#include "headers/tree.h"

#define INIT_L_BRN_DATA(tmp) tmp->L_brunch = (knot*) calloc(1, sizeof(knot));\
                             tmp->L_brunch->knot_depth = tmp->knot_depth + 1;\
                             tmp->L_brunch->knot_horizontal_position = tmp->knot_horizontal_position - 1;\
                             tmp->L_brunch->prev = tmp;\
                             tmp->L_brunch->L_brunch = NULL;\
                             tmp->L_brunch->R_brunch = NULL;\

#define INIT_R_BRN_DATA(tmp) tmp->R_brunch = (knot*) calloc(1, sizeof(knot));\
                             tmp->R_brunch->knot_depth = tmp->knot_depth + 1;\
                             tmp->R_brunch->knot_horizontal_position = tmp->knot_horizontal_position + 1;\
                             tmp->R_brunch->prev = tmp;\
                             tmp->R_brunch->L_brunch = NULL;\
                             tmp->R_brunch->R_brunch = NULL;\
                            
Akinator::Akinator():Tree(){

    setlocale(LC_ALL, "Russian");
    printf("akinator construct was called\n");
    log = fopen("Akinator_log.txt", "w");
}


Akinator::Akinator(FILE* base):Tree(base) {

    printf("akinator construct(FILE*) was called\n");
    log = fopen("Akinator_log.txt", "w");
    add_to_log("Akinator has been created with input data");
}


void Akinator::add_to_log(const char* line){

    fprintf(log, "%s\n", line);
}


void Akinator::add_to_log(const char* line, char* name){

    fprintf(log, "%s |%s|\n", line, name);
}


Akinator::~Akinator(){

    fclose(log);
}


void Akinator::add_leaf(knot* old_leaf, char answer){

    char name[MAXLEN] = {'!'};
    printf("Введи имя своего персонажа:\n");
    scanf("%s", name);

    if ((answer == 'Y') || (answer == 'y')){

        INIT_L_BRN_DATA(old_leaf);
        old_leaf->L_brunch->data = my_memcpy(name);
    }

    if ((answer == 'N') || (answer == 'n')){

        INIT_R_BRN_DATA(old_leaf);
        old_leaf->R_brunch->data = my_memcpy(name);
    }
    
}


void Akinator::add_question(knot* old_leaf){
    
    char name[MAXLEN] = {'!'};
    char attribute[MAXLEN] = {'!'};
    char answer[MAXLEN] = {'!'};
    printf("Введи признак, который отличает твоего персонажа от %s:\n", old_leaf->data);
    scanf("%s", attribute);

    printf("Твой персонаж отвечает этому признаку?\n");
    printf("[Y/N]?\n");
    scanf("%s", answer);

    printf("Как зовут твоего персонажа?\n");
    scanf("%s", name);

    if ((answer[0] == 'Y') || (answer[0] == 'y')){

        old_leaf->L_brunch->data = my_memcpy(name);
        INIT_L_BRN_DATA(old_leaf);

        old_leaf->R_brunch->data = old_leaf->data;
        INIT_R_BRN_DATA(old_leaf);

        old_leaf->data = my_memcpy(attribute);
    } else{

        if ((answer[0] == 'N') || (answer[0] == 'n')){

            INIT_R_BRN_DATA(old_leaf);
            old_leaf->R_brunch->data = my_memcpy(name);

            
            old_leaf->L_brunch->data = old_leaf->data;
            INIT_L_BRN_DATA(old_leaf);

            old_leaf->data = my_memcpy(attribute);
        } else{

            printf("Неправильный ответ, поэтому ты дисквалификацирован\n");
            printf("Я победил(наверное)\n");
        }
    }
}


void Akinator::init_data(){

    char fir_attribute[MAXLEN] = {'!'};
    char name[MAXLEN] = {'!'};
    char answer[MAXLEN] = {'!'};
    printf("База акинатора пуста, поэтому введите первый признак:\n");
    scanf("%s", fir_attribute);

    printf("Ваш персонаж отвечает этому признаку?\n");
    printf("[Y/N]?\n");
    scanf("%s", answer);

    printf("Введите его имя\n");
    scanf("%s", name);

    if ((answer[0] == 'Y') || (answer[0] == 'y')){

        INIT_L_BRN_DATA(root);
        root->data = my_memcpy(fir_attribute);
    } else{

        if ((answer[0] == 'N') || (answer[0] == 'n')){

            INIT_R_BRN_DATA(root);
            root->data = my_memcpy(fir_attribute);
        } else{

            printf("Неправильный ответ\n");
        }
    }
}

int Akinator::guess_and_add(){//если да то идем влево

    knot* tmp = root;
    knot* prev_knot = tmp;
    char answer[MAXLEN] = {'!'};
    
    if (root->data == NULL){

        init_data();
        return FIRST_TRY;
    }

    assert(tmp != NULL);
    int tmp_has_brn = ((tmp->L_brunch != NULL) || (tmp->R_brunch != NULL));

    while ( tmp_has_brn && (tmp != NULL)){

        printf("Ваш персонаж %s?\n", tmp->data);
        printf("[Y/N]?\n");
        scanf("%s", answer);

        prev_knot = tmp;
        if ((answer[0] == 'Y') || (answer[0] == 'y')){

            tmp = tmp->L_brunch;
        } else{

            if ((answer[0] == 'N') || (answer[0] == 'n')){

                printf("in N\n");
                tmp = tmp->R_brunch;
            } else{

                printf("Неправильный ответ\n");
            }
        }
        
        if (tmp == NULL){

            tmp_has_brn = 0;
        } else{

            tmp_has_brn = ((tmp->L_brunch != NULL) || (tmp->R_brunch != NULL));
        }
    }

    if (tmp != NULL){

        printf("Это %s?\n", tmp->data);
        printf("[Y/N]?\n");
        scanf("%s", answer);

        if ((answer[0] == 'Y') || (answer[0] == 'y')){

            printf("Я победил\n");
            return MY_WIN;
        } else{

            if ((answer[0] == 'N') || (answer[0] == 'n')){

                add_question(tmp);
                return MY_FAULT;
            } else{

                printf("Неправильный ответ, поэтому ты дисквалификацирован\n");
                printf("Я победил\n");
                return MY_WIN;
            }
        }

    } else{

        printf("Хз, что ты загадал\n");
        add_leaf(prev_knot, answer[0]);
        return MY_FAULT;
    }
}