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
    
    log = fopen("Akinator_log.txt", "w");
    assert(log != NULL);
    fprintf(log, "akinator construct was called\n");
    //log = stdout;
}


Akinator::Akinator(FILE* base):Tree(base) {

    log = fopen("Akinator_log.txt", "w");
    assert(log != NULL);
    //log = stdout;
    add_to_log("Akinator has been created with input data");
}


void Akinator::add_to_log(const char* line){

    fprintf(log, "%s\n", line);
}


void Akinator::add_to_log(const char* line, unsigned char* name){

    fprintf(log, "%s |%s|\n", line, name);
}


Akinator::~Akinator(){

    fclose(log);
}


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


void Akinator::add_question(knot* old_leaf){
    
    unsigned char name[MAXLEN] = {'!'};
    unsigned char attribute[MAXLEN] = {'!'};
    unsigned char answer[MAXLEN] = {'!'};
    printf("\nВведи признак, который отличает твоего персонажа от %s:\n", old_leaf->data);
    scanf("%s", attribute);

    printf("\nТвой персонаж отвечает этому признаку? ");
    printf("[Y/N]\n");
    scanf("%s", answer);

    printf("\nКак зовут твоего персонажа?\n");
    scanf("%s", name);

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
        if ((answer[0] == 'Y') || (answer[0] == 'y')){

            tmp = tmp->L_brunch;
            add_to_log("-go left");
        } else{

            if ((answer[0] == 'N') || (answer[0] == 'n')){

                add_to_log("-go right");
                tmp = tmp->R_brunch;
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

        printf("\nЭто %s?\n", tmp->data);
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

        printf("Хз, что ты загадал\n");
        add_to_log("-adding leaf with answer:", answer);
        add_leaf(prev_knot, answer[0]);
        return MY_FAULT;
    }
}

void print_brunch(int* position_of_line, int number_of_lines, knot* elem){

    if (elem != NULL){

        for (int i = 0; i < number_of_lines; i++){

            for (int j = 0; j < position_of_line[i]; j++){
                printf(" ");
            }

            printf("|");
        }

        printf("__");
        printf("%s\n", elem->data);

        position_of_line[number_of_lines] = length(elem->data); 

        print_brunch(position_of_line, number_of_lines + 1, elem->R_brunch);
        print_brunch(position_of_line, number_of_lines + 1, elem->L_brunch);
    }
}

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

    printf("|%s|\n", link_to_fir_elem->data);
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
    print_brunch(position_of_line, number_of_lines, root->L_brunch);

    free(position_of_line);
}