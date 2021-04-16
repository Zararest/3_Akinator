#include "headers/tree.h"

#define CHECK_RIGHTEST if (tmp->knot_horizontal_position > rightest_knot) rightest_knot = tmp->knot_horizontal_position 
#define CHECK_LEFTEST if (tmp->knot_horizontal_position < leftest_knot) leftest_knot = tmp->knot_horizontal_position
#define CHECK_DEEPEST if (tmp->knot_depth > depth) depth = tmp->knot_depth

char* my_memcpy(char* line){

    int len = 0;
    char* new_line = NULL;

    if (line == NULL){
        return NULL;
    }

    while (line[len] != '\0'){
        len++;
    }

    new_line = (char*)calloc(len + 1, sizeof(char));

    for(int i = 0; i <= len; i++){
        new_line[i] = line[i];
    }

    return new_line;
}


Tree::Tree(){

    undef_brunch = (knot*) calloc(1, sizeof(knot));

    root = (knot*) calloc(1, sizeof(knot));
    root->data = NULL;
    root->knot_depth = 0;
    root->knot_horizontal_position = 0;
    root->L_brunch = (knot*) undef_brunch;
    root->R_brunch = (knot*) undef_brunch;
    root->prev = NULL;

    depth = 0;
    rightest_knot = 0;
    leftest_knot = 0;
}

Tree::Tree(FILE* base){//для бинарного

    undef_brunch = (knot*) calloc(1, sizeof(knot));

    root = (knot*) calloc(1, sizeof(knot));
    root->data = NULL;
    root->knot_depth = 0;
    root->knot_horizontal_position = 0;
    root->L_brunch = (knot*) undef_brunch;
    root->R_brunch = (knot*) undef_brunch;
    root->prev = NULL;
    
    depth = 0;
    rightest_knot = 0;
    leftest_knot = 0;

    knot* tmp = root;
    int buffers_size = 0, i = 0, r_brn_exist = 0, l_brn_exist = 0;
    char* word = NULL;
    char* name = NULL;

    fread(&buffers_size, sizeof(int), 1, base);
    char* buffer = (char*) calloc(buffers_size, sizeof(char));
    printf("buffer size =%i\n", buffers_size);
    fread(buffer, sizeof(char), buffers_size, base);

    word = strtok(buffer, "|");
    
    while (word != NULL){// *имя*|Y|N|*...
                            //    |
        assert(word != NULL);//    \____Существлвание левой ветки(следующее значение - существование правой ветки)
        name = word;         // обход сначала на максимум влево(справа находятся положительные горизонтаьные позиции)
        l_brn_exist = (*strtok(NULL, "|") == 'Y');
        r_brn_exist = (*strtok(NULL, "|") == 'Y');
        word = strtok(NULL, "|");
        
        tmp->data = my_memcpy(name);
        CHECK_RIGHTEST;
        CHECK_LEFTEST;
        CHECK_DEEPEST;

        if (r_brn_exist){

            tmp->R_brunch = undef_brunch;
            tmp->R_brunch->knot_depth = tmp->knot_depth + 1;
            tmp->R_brunch->knot_horizontal_position = tmp->knot_horizontal_position + 1;
        } else{

            tmp->R_brunch = NULL;
            
        }

        if (l_brn_exist){

            tmp->L_brunch = (knot*) calloc(1, sizeof(knot));
            tmp->L_brunch->knot_depth = tmp->knot_depth + 1;
            tmp->L_brunch->knot_horizontal_position = tmp->knot_horizontal_position - 1;
            tmp->L_brunch->prev = tmp;
            tmp = tmp->L_brunch;
        } else{

            tmp->L_brunch = NULL;
            while ((tmp->R_brunch != undef_brunch) && (tmp->prev != NULL)){

                tmp = tmp->prev;
            }

            if ((tmp->prev == NULL) && (tmp->R_brunch != undef_brunch)){

                word = NULL;
            } else{

                tmp->R_brunch = (knot*) calloc(1, sizeof(knot));
                tmp->R_brunch->knot_depth = tmp->knot_depth + 1;
                tmp->R_brunch->prev = tmp;
                tmp = tmp->R_brunch;
            }
        }
        
    }
}

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