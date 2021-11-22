/**
 * @file main.cpp
 * @author Zararest
 * @brief 
 * @version 0.1
 * @date 2021-11-22
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include "headers/Akinator.hpp"

// command line arguments
// add colored printf

void game_without_input(){

    Akinator my_Akinator;

    char answer[MAXLEN] = {'!'};

    while ((answer[0] != 'n') && (answer[0] != 'N')){

        my_Akinator.guess_and_add();
        printf("\nХотите продолжить? [Y/N]\n");
        scanf("%s", answer); // just in case
        // protection
    }

    printf("Введите файл, куда будет загружено дерево(если вы не хотите этого, то введите N)\n");
    scanf("%s", answer);

    if (strcmp("N", answer) != 0){

        my_Akinator.create_base_file(answer);
    }
}

void game_with_input(char* input_name){

    FILE* base = fopen(input_name, "rb");
    
    if (base == nullptr){

        printf("Файл не может быть открыт. Игра запущена без базы с персонажами\n");
        game_without_input();
        printf("Game over\n");
        exit(0);
    }

    Akinator my_Akinator(base);
    char answer[MAXLEN] = {'!'};
    
    while ((answer[0] != 'n') && (answer[0] != 'N')){

        my_Akinator.guess_and_add();
        printf("\nХотите продолжить? [Y/N]\n");
        scanf("%s", answer);
    }
    fclose(base); 
    my_Akinator.create_base_file(input_name);
}


int main(){

    setlocale(LC_ALL, "Russian");
    char input_name[MAXLEN] = {'\0'};
    FILE* input_file = nullptr;

    printf("Вас приветствует Акинатор версии 0.001\n");
    printf("Если вы уже имеете базу с персонажами, то укажите название бинарного файла(если нет, то введите N):\n");
    scanf("%s", input_name);
    
    if ((input_name[0] == 'N') && (length((unsigned char*) input_name) == 1)){

        game_without_input();
    } else{

        game_with_input(input_name);
    }

    printf("Game over\n");
    return 0; 
}
