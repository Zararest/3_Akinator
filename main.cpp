 #include <stdio.h>
#include <stdlib.h>
#include "headers/tree.h"

void game_without_input(){

    Akinator my_Akinator;

    char answer[MAXLEN] = {'!'};

    while ((answer[0] != 'n') && (answer[0] != 'N')){

        my_Akinator.guess_and_add();
        printf("\nWanna continue? [Y/N]\n");
        scanf("%s", answer);
    }

    printf("Введите файл, куда будет загружено дерево(если вы не хотите этого, то введите N)\n");
    scanf("%s", answer);

    if ((answer[0] != 'N') || (length((unsigned char*) answer) != 1)){

        my_Akinator.create_base_file(answer);
    }
}

void game_with_input(FILE* base){ //надо сделать аргументом название файла

    Akinator my_Akinator(base);

    char answer[MAXLEN] = {'!'};
    my_Akinator.dump();
    while ((answer[0] != 'n') && (answer[0] != 'N')){

        my_Akinator.guess_and_add();
        printf("\nWanna continue? [Y/N]\n");
        scanf("%s", answer);
    }
}


int main(){

    setlocale(LC_ALL, "Russian");
    char input_name[MAXLEN] = {'\0'};
    FILE* input_file = NULL;

    printf("Вас приветствует Акинатор версии 0.001\n");
    printf("Если вы уже имеете базу с персонажами, то укажите название бинарного файла(если нет, то введите N):\n");
    scanf("%s", input_name);
    
    if ((input_name[0] == 'N') && (length((unsigned char*) input_name) == 1)){

        game_without_input();
        
    } else{

        input_file = fopen(input_name, "rb");
 
        if (input_file == NULL){

            printf("Incorrect file name\n");
        } else{

            game_with_input(input_file);
        }
    }

    printf("Game over\n");
    return 0; 
}