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

//------------------Команды режима debug
void print_file(const char* name){

    char symbol = '!';
    FILE* file = fopen(name, "r");

    fscanf(file, "%c", &symbol);

    while (symbol != EOF){

        printf("%c", symbol);
        symbol = fgetc(file);
    }
}

void dump_file(){

    char name[MAXLEN] = {'!'};

    scanf("%s", name);
    printf("in dump\n");
    FILE* base = fopen(name, "rb");
    assert(base != NULL);

    Akinator tmp_Akinator(base);
    printf("in dump 2\n");
    tmp_Akinator.dump();
}

void Akinator::print_path(knot* last_elem){
    
    int size_of_stack = last_elem->knot_depth + 1;
    int i = 1;
    knot** stack_of_elem = (knot**) calloc(size_of_stack, sizeof(knot*));
    knot* tmp = last_elem;

    while (tmp != root){

        stack_of_elem[size_of_stack - i] = tmp;
        i++;
        tmp = tmp->prev;
    }
    
    stack_of_elem[0] = root;

    for (i = 0; i < size_of_stack - 1; i++){

        assert(stack_of_elem[i]->data != NULL);
        if (stack_of_elem[i]->L_brunch == stack_of_elem[i + 1]){

            printf("%s --> ", stack_of_elem[i]->data);
        } else{

            printf("не %s --> ", stack_of_elem[i]->data);
        }  
    }
    
    if ((stack_of_elem[size_of_stack - 1]->L_brunch == NULL) && (stack_of_elem[size_of_stack - 1]->R_brunch == NULL)){

        printf("%s --> NULL\n", stack_of_elem[size_of_stack - 1]->data);
    } else{

        printf("...\n");
    }
    
    free(stack_of_elem);
}

void Akinator::fix_elements_positions(knot* tmp, int cur_depth, int cur_hor_position){

    if (tmp != NULL){

        tmp->knot_depth = cur_depth;
        tmp->knot_horizontal_position = cur_hor_position;
        
        fix_elements_positions(tmp->R_brunch, cur_depth + 1, cur_hor_position + 1);
        fix_elements_positions(tmp->R_brunch, cur_depth + 1, cur_hor_position - 1);
        CHECK_RIGHTEST;
        CHECK_LEFTEST;
        CHECK_DEEPEST;
    }
}

void Akinator::selected_elem(knot* elem){

    char word[MAXLEN] = {'!'};
    unsigned char first_elem[MAXLEN] = {'!'};
    knot* tmp = NULL;
    int loop_status = 0;

    printf("\t");
    scanf("%s", word);

    while ((strcmp(word, "drop") != 0) && (loop_status == 0)){

        if (strcmp(word, "add") == 0){

            scanf("%s", word);

            if ((strcmp(word, "before") == 0) && (elem != root)){

                scanf("%s", first_elem);
                
                if (elem->prev->R_brunch == elem){
                    
                    INIT_R_BRN_DATA(elem->prev);
                    elem->prev->R_brunch->data = my_memcpy(first_elem);
                    elem->prev->R_brunch->R_brunch = elem;
                    elem->prev = elem->prev->R_brunch;

                    fix_elements_positions(elem, elem->knot_depth + 1, elem->knot_horizontal_position + 1);
                } else{
                    
                    INIT_L_BRN_DATA(elem->prev);
                    elem->prev->L_brunch->data = my_memcpy(first_elem);
                    elem->prev->L_brunch->L_brunch = elem;
                    elem->prev = elem->prev->L_brunch;

                    fix_elements_positions(elem, elem->knot_depth + 1, elem->knot_horizontal_position - 1);
                }

                loop_status = 2;
            } else{

                if (strcmp(word, "before") == 0){

                    printf("\tCan't add before root\n");
                }

                loop_status = 2;
            }

            if (strcmp(word, "right") == 0){

                scanf("%s", first_elem);

                tmp = elem->R_brunch;
                INIT_R_BRN_DATA(elem);
                elem->R_brunch->data = my_memcpy(first_elem);
                elem->R_brunch->R_brunch = tmp;

                if (tmp != NULL){

                    tmp->prev = elem->R_brunch;
                }
                
                fix_elements_positions(elem->R_brunch, elem->knot_depth + 1, elem->knot_horizontal_position + 1);
                loop_status = 2;
            }

            if (strcmp(word, "left") == 0){

                scanf("%s", first_elem);

                tmp = elem->L_brunch;
                INIT_L_BRN_DATA(elem);
                elem->L_brunch->data = my_memcpy(first_elem);
                elem->L_brunch->L_brunch = tmp;

                if (tmp != NULL){

                    tmp->prev = elem->L_brunch;
                }

                fix_elements_positions(elem->R_brunch, elem->knot_depth + 1, elem->knot_horizontal_position - 1);
                loop_status = 2;
            }

            if (loop_status == 0){

                printf("strange command for add\n");
                loop_status = 2;
            }

        }

        if (strcmp(word, "delete") == 0){

            if  ((elem->R_brunch == NULL) && (elem->L_brunch == NULL)){

                free(elem->data);

                if (elem->prev->R_brunch == elem){

                    elem->prev->R_brunch = NULL;
                } else{

                    elem->prev->L_brunch = NULL;
                }

                free(elem);

                loop_status = 1;
            } else{

                printf("\tCan't delete brunch with descendants\n");

                loop_status = 2;
            }
        }

        if (strcmp(word, "change") == 0){

            scanf("%s", first_elem);

            free(elem->data);
            elem->data = my_memcpy(first_elem);

            loop_status = 2;
        }

        if (strcmp(word, "merge") == 0){

            scanf("%s", word);

            FILE* second_tree = fopen(word, "rb");

            if (second_tree != NULL){
                
                Akinator tmp_Akinator(second_tree);

                if (tmp_Akinator.root->data == NULL){
                    
                    printf("\tSecond tree is empty\n");
                }

                if ((strcmp((char*)tmp_Akinator.root->data, (char*)elem->data) == 0) && (elem->L_brunch == NULL) && (elem->R_brunch == NULL)){

                    elem->R_brunch = tmp_Akinator.root->R_brunch;
                    elem->L_brunch = tmp_Akinator.root->L_brunch;
                    elem->R_brunch->prev = elem;
                    elem->L_brunch->prev = elem;

                    fix_elements_positions(elem, elem->knot_depth, elem->knot_horizontal_position);

                    free(tmp_Akinator.root->data);
                    tmp_Akinator.root = NULL;

                } else{

                    printf("\tElemnts don't fit\n");
                }
                loop_status = 2;
            } else{

                printf("\tCan't open file\n");
                loop_status = 2;
            }
        }

        if (loop_status == 2){

            printf("\t");
            scanf("%s", word);
            loop_status = 0;
        } else{

            if (loop_status == 0){

                printf("\tStrange command\n");
                loop_status = 0;
            }
        }
    }
}

void Akinator::debug(){

    char word[MAXLEN] = {'!'};
    unsigned char first_elem[MAXLEN] = {'!'};
    unsigned char second_elem[MAXLEN] = {'!'};
    int right_command = 0;

    printf("\n_______________________________");
    printf("_________________________________");
    printf("_________________________________\n\n");
    printf("Akinator debug mode\n");
    printf("(for more information enter 'info')\n");

    scanf("%s", word);

    while (strcmp(word, "stop") != 0){

        if (strcmp(word, "info") == 0){

            print_file("instructions.txt");
            right_command = 1;
        }

        if (strcmp(word, "show") == 0){

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

                if (find_elem(first_elem, root) != NULL){

                    print_path(find_elem(first_elem, root));
                } else{

                    printf("Can't find such an element\n");
                }
            }

            right_command = 1;
        }

        if (strcmp(word, "select") == 0){

            scanf("%s", first_elem);

            if (find_elem(first_elem, root) != NULL){

                selected_elem(find_elem(first_elem, root));
            } else{

                printf("Can't find elem %s\n", first_elem);
            }
            
            right_command = 1;
        }

        if (right_command == 0){

            printf("Strange command\n");
        }

        right_command = 0;
        scanf("%s", word);
    }
}
//------------------

//------------------Задание вопроса
int Akinator::find_answer(const char* question){

    unsigned char answer[MAXLEN] = {'!'};
    printf("%s?\n", question);
    
    while (NULL != 10){

        printf("[Y/N]\n");
        scanf("%s", answer);

        if (strcmp("exit", (char*)answer) == 0){

            exit(0);
        }

        if (strcmp("debug", (char*)answer) == 0){
            
            debug();
            printf("Вспомни прошлый вопрос и ответь на него\n");
        }

        if ((strcmp("y", (char*)answer) == 0) || (strcmp("Y", (char*)answer) == 0)){
            
            return YES;
        }

        if ((strcmp("n", (char*)answer) == 0) || (strcmp("N", (char*)answer) == 0)){
            
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

    assert(old_leaf != NULL);
    if (answer == YES){

        INIT_L_BRN_DATA(old_leaf);
        assert(old_leaf->L_brunch != NULL);
        old_leaf->L_brunch->data = my_memcpy(name);
        
        data_size += length(old_leaf->L_brunch->data) + 6;

        add_to_log("In add leaf item was added to the left brunch:", old_leaf->L_brunch->data);
    }

    if (answer == NO){

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
    int answer = -1;

    printf("\nКак зовут твоего персонажа?\n");
    scanf("%s", name);

    printf("\nВведи признак, который отличает твоего персонажа от %s:\n", old_leaf->data);
    scanf("%s", attribute);

    answer = find_answer("\nТвой персонаж отвечает этому признаку");

    assert(old_leaf != NULL);
    if (answer == YES){

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
    }

    if (answer == NO){

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
    }
}
//------------------

//------------------Добавление первых данных в пустое дерево
void Akinator::init_data(){

    unsigned char fir_attribute[MAXLEN] = {'!'};
    unsigned char name[MAXLEN] = {'!'};
    int answer = -1;
    printf("\nБаза акинатора пуста, поэтому введите первый признак:\n");
    scanf("%s", fir_attribute);

    /*printf("\nВаш персонаж отвечает этому признаку? ");
    printf("[Y/N]\n");
    scanf("%s", answer);*/
    answer = find_answer("\nВаш персонаж отвечает этому признаку");

    printf("\nВведите его имя\n");
    scanf("%s", name);

    if (answer == YES){

        INIT_L_BRN_DATA(root);
        root->L_brunch->data = my_memcpy(name);
        root->data = my_memcpy(fir_attribute);
        assert(root->L_brunch->data != NULL);
        assert(root->data != NULL);

        data_size += length(root->L_brunch->data)+ 5 + length(root->data) + 6;

        add_to_log("In init data item was added to the left brunch:", root->L_brunch->data);
    } 

    if (answer == NO){

        INIT_R_BRN_DATA(root);
        root->R_brunch->data = my_memcpy(name);
        root->data = my_memcpy(fir_attribute);
        assert(root->R_brunch->data != NULL);
        assert(root->data != NULL);

        data_size += length(root->R_brunch->data)+ 5 + length(root->data) + 6;

        add_to_log("In init data item was added to the right brunch:", root->R_brunch->data);
    }
}
//------------------

//------------------Угадывание и добавление в дерево
int Akinator::guess_and_add(){//если да то идем влево

    add_to_log("Guess was called:");
    knot* tmp = root;
    knot* prev_knot = tmp;
    int answer = -1;
    
    if (root->data == NULL){

        add_to_log("-first init data");
        init_data();
        return FIRST_TRY;
    }

    assert(tmp != NULL);
    int tmp_has_brn = ((tmp->L_brunch != NULL) || (tmp->R_brunch != NULL));

    while ( tmp_has_brn && (tmp != NULL)){

        printf("\nВаш персонаж ");
        answer = find_answer((char*)tmp->data);

        prev_knot = tmp;
        if (answer == YES){

            tmp = tmp->L_brunch;
            add_to_log("-go left");
        }

        if (answer == NO){

            tmp = tmp->R_brunch;
            add_to_log("-go right");
        }
        
        
        if (tmp == NULL){

            tmp_has_brn = 0;
        } else{

            tmp_has_brn = ((tmp->L_brunch != NULL) || (tmp->R_brunch != NULL));
        }
    }

    if (tmp != NULL){

        printf("\nЭто ");
        answer = find_answer((char*)tmp->data);

        if (answer == YES){

            printf("Я победил\n");
            add_to_log("-return |MY_WIN|");
            return MY_WIN;
        }

        if (answer == NO){
                
            add_to_log("-adding question");
            add_question(tmp);
            add_to_log("-return |MY_FAULT|");
            return MY_FAULT;  
        }

    } else{

        printf("Не знаю такого предмета\n");
        add_leaf(prev_knot, answer);
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

        position_of_line[number_of_lines] = length(elem->data); 

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

    printf("%s\n", root->data);
    print_brunch(position_of_line, number_of_lines, root->R_brunch);
    position_of_line[0] = position_of_line[0] * (-1);
    print_brunch(position_of_line, number_of_lines, root->L_brunch);

    free(position_of_line);
}
//------------------

//------------------Вывод сходств и различий   (не работает с корнем и не распознает одинаковые элементы)
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

    if ((link_to_fir_elem == NULL) || (link_to_sec_elem == NULL) || (link_to_fir_elem == link_to_sec_elem)){

        if ((link_to_fir_elem == link_to_sec_elem) && (link_to_fir_elem != NULL)){

            printf("Элементы одинаковые\n");
        } else{

            printf("Can't find such elements\n");
        }
        
    } else{

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
        
        if (link_to_fir_elem->knot_horizontal_position > link_to_sec_elem->knot_horizontal_position){

                unsigned char* tmp = fir_elem;
                fir_elem = sec_elem;
                sec_elem = tmp;            
        }

        if ((link_to_fir_elem->prev == root) || (link_to_fir_elem == root)){

            if (link_to_fir_elem->prev == root){

                printf("Персонажи %s и %s не имеют общих черт,", fir_elem, sec_elem);
                printf("но различаются тем, что %s имеет признак %s, а %s нет\n", fir_elem, link_to_fir_elem->prev->data, sec_elem);
            } else{

                printf("Один из элементов это корень\n");
            }
            
        } else{ 
        
            printf("Персонаж %s похож на %s тем, что каждый из них: ", fir_elem, sec_elem);

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

            printf("но различаются тем, что %s имеет признак %s, а %s нет\n", fir_elem, link_to_fir_elem->prev->data, sec_elem);
        }
    }
}
//------------------
