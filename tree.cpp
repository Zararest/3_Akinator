#include "headers/tree.h"

//------------------Длина строки в байтах(надо переделать на длину в символах)
int length(unsigned char* line){

    assert(line != NULL);
    int i = 0, len = 0;
    while (line[i] != '\0'){

        len++;
        i += 1 + ((line[i] & 0x80) == 128); 
    }

    return len;
}
//------------------

//------------------Выделение памяти для строки
unsigned char* my_memcpy(unsigned char* line){

    if (line == NULL){
        return NULL;
    }

    int len = 0;
    unsigned char* new_line = NULL;

    while (line[len] != '\0'){
        len++;
    }

    new_line = (unsigned char*)calloc(len + 1, sizeof(char));

    for(int i = 0; i <= len; i++){
        new_line[i] = line[i];
    }

    return new_line;
}
//------------------

//------------------Конструктор дерева без входного файла
Tree::Tree(){

    undef_brunch = (knot*) calloc(1, sizeof(knot));

    root = (knot*) calloc(1, sizeof(knot));
    root->data = NULL;
    root->knot_depth = 0;
    root->knot_horizontal_position = 0;
    root->L_brunch = NULL;
    root->R_brunch = NULL;
    root->prev = NULL;

    depth = 0;
    rightest_knot = 0;
    leftest_knot = 0;
    data_size = 0;
}
//------------------

//------------------Конструктор с входным файлом
Tree::Tree(FILE* base){

    assert(base != NULL);
    undef_brunch = (knot*) calloc(1, sizeof(knot));

    root = (knot*) calloc(1, sizeof(knot));
    root->data = NULL;
    root->knot_depth = 0;
    root->knot_horizontal_position = 0;
    root->L_brunch = NULL;
    root->R_brunch = NULL;
    root->prev = NULL;
    
    depth = 0;
    rightest_knot = 0;
    leftest_knot = 0;

    knot* tmp = root;
    int buffer_size = 0, i = 0, r_brn_exist = 0, l_brn_exist = 0;
    unsigned char* word = NULL;
    unsigned char* name = NULL;

    fread(&buffer_size, sizeof(int), 1, base);
    unsigned char* buffer = (unsigned char*) calloc(buffer_size, sizeof(char));
    fread(buffer, sizeof(char), buffer_size, base);

    data_size = buffer_size;
    word = (unsigned char*) strtok( (char*)buffer, "|");
    
    while (word != NULL){// *имя*|Y|N|*...
                            //    |
                            //    \____Существлвание левой ветки(следующее значение - существование правой ветки)
        name = word;        // обход сначала на максимум влево(справа находятся положительные горизонтаьные позиции)
        
        word = (unsigned char*) strtok(NULL, "|");
        assert(word != NULL);
        l_brn_exist = (word[0] == 'Y');

        word = (unsigned char*) strtok(NULL, "|");
        assert(word != NULL);
        r_brn_exist = (word[0] == 'Y');

        word = (unsigned char*) strtok(NULL, "|");

        tmp->data = my_memcpy(name);
        CHECK_RIGHTEST;
        CHECK_LEFTEST;
        CHECK_DEEPEST;

        assert(tmp != NULL);
        if (r_brn_exist){

            tmp->R_brunch = undef_brunch;
        } else{

            tmp->R_brunch = NULL;
        }
  
        if (l_brn_exist){

            INIT_L_BRN_DATA(tmp);
            tmp = tmp->L_brunch;
        } else{

            tmp->L_brunch = NULL;
            while ((tmp->R_brunch != undef_brunch) && (tmp->prev != NULL)){

                tmp = tmp->prev;
            }

            if ((tmp->prev == NULL) && (tmp->R_brunch != undef_brunch)){

                word = NULL;
            } else{

                INIT_R_BRN_DATA(tmp);
                tmp = tmp->R_brunch;
            }
        }
        
    }

    free(undef_brunch);
    free(buffer);
}
//------------------

//------------------Рекурсивная простая распечатка 
void print_knot(knot* tmp){

    if (tmp != NULL){

        print_knot(tmp->L_brunch);
        printf("%s[%i][%i]\n", tmp->data, tmp->knot_horizontal_position, tmp->knot_depth);
        print_knot(tmp->R_brunch);
    }
}

void Tree::dump_tree(){

    printf("Dumped tree: *name*[horizontal_position][depth]\n");
    print_knot(root);
}
//------------------

//------------------Рекурсивная запись данных в файл
void add_to_file(knot* tmp, FILE* output){

    if (tmp != NULL){

        fwrite(tmp->data, sizeof(char), length(tmp->data), output);
        if (tmp->L_brunch != NULL){

            fwrite("|Y|", sizeof(char), 3, output);
        } else{

            fwrite("|N|", sizeof(char), 3, output);
        }

        if (tmp->R_brunch != NULL){

            fwrite("|Y|", sizeof(char), 3, output);
        } else{

            fwrite("|N|", sizeof(char), 3, output);
        }

        add_to_file(tmp->L_brunch, output);
        add_to_file(tmp->R_brunch, output);
    }
}

void Tree::create_base_file(char* name_of_file){

    FILE* output = fopen(name_of_file, "wb");

    data_size += 2;
    fwrite(&data_size, sizeof(int), 1, output);
    add_to_file(root, output);

    fclose(output);
}
//------------------

//------------------Рекурсивный деструктор
void delete_knot(knot* tmp){

    if (tmp != NULL){

        delete_knot(tmp->L_brunch);
        delete_knot(tmp->R_brunch);

        free(tmp->data);
        free(tmp);
    }
}

Tree::~Tree(){

    delete_knot(root);
}
//------------------