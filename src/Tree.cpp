#include "headers/Tree.hpp"

#include <cassert>
#include <cstring>

void init_L_brn_data(knot* tmp){

    tmp->L_brunch = new knot;
    tmp->L_brunch->knot_depth = tmp->knot_depth + 1;
    tmp->L_brunch->knot_horizontal_position = tmp->knot_horizontal_position - 1;
    tmp->L_brunch->prev = tmp;
}

void init_R_brn_data(knot* tmp){

    tmp->R_brunch = new knot;
    tmp->R_brunch->knot_depth = tmp->knot_depth + 1;
    tmp->R_brunch->knot_horizontal_position = tmp->knot_horizontal_position - 1;
    tmp->R_brunch->prev = tmp;
}

int length(unsigned char* line){

    assert(line != nullptr);
    int i = 0, len = 0;
    while (line[i] != '\0'){

        len++;
        i += 1 + ((line[i] & 0x80) == 128); 
    }

    return len;
}

unsigned char* my_memcpy(unsigned char* line){

    if (line == nullptr){
        return nullptr;
    }

    int len = 0;
    unsigned char* new_line = nullptr;

    while (line[len] != '\0'){
        len++;
    }

    new_line = (unsigned char*)calloc(len + 1, sizeof(char));

    for(int i = 0; i <= len; i++){
        new_line[i] = line[i];
    }

    return new_line;
}

Tree::Tree(){

    root = new knot;
}

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

        if (cur_knot->R_brunch == undef_brunch){ //!!!
            
            init_R_brn_data(cur_knot);
            cur_knot = cur_knot->R_brunch;
        }
    }
}


Tree::Tree(FILE* base){

    root = new knot;

    assert(base != nullptr);
    fread(&data_size, sizeof(int), 1, base);
    unsigned char* buffer = new unsigned char[data_size];
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
//------------------

//------------------Рекурсивная простая распечатка 
void print_knot(knot* tmp){

    if (tmp != nullptr){

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

    if (tmp != nullptr){

        fwrite(tmp->data, sizeof(char), length(tmp->data), output);
        if (tmp->L_brunch != nullptr){

            fwrite("|Y|", sizeof(char), 3, output);
        } else{

            fwrite("|N|", sizeof(char), 3, output);
        }

        if (tmp->R_brunch != nullptr){

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

    if (tmp != nullptr){

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