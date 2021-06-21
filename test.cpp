#include <stdio.h>
#include <stdlib.h>
//#include "headers/tree.h"
//#include "Array.inl"


int length_1(unsigned char* line){

    //assert(line != NULL);
    int i = 0;
    int len = 0;
    while (line[i] != '\0'){

        //i++;
        len++;
        printf("|%i|", (line[i] & 0x80) == 128);
        i += 1 + ((line[i] & 0x80) == 128); 
        
    }

    return len;
}


int main(){

    /*Akinator test_akinator;
    char flag[MAXLEN] = {'!'};

    while (flag[0] != 'n'){

        test_akinator.guess_and_add();

        printf("Wanna continue?\n");
        scanf("%s", flag);
    }

    test_akinator.dump_tree();
    test_akinator.create_base_file("1_base.bin");*/

   /* FILE* tmp_file = fopen("1_base.bin", "rb");
    Akinator second_akinator(tmp_file);

    second_akinator.dump_tree();
    second_akinator.create_base_file("2_base.bin");*/

    /*unsigned char* tmp = (unsigned char*) calloc(30, sizeof(char));
    setlocale(LC_ALL, "Russian");

    scanf("%s", tmp);
    printf("%s", strtok((char*)tmp, "|"));*/

    //Array<int> tmp_arr(10);

    //tmp_arr.push(12);
    //printf("tmp pop = %i\n", tmp_arr.pop());

    //Array<int> second_arr(10);

    //second_arr = tmp_arr;

    //printf("second pop = %i\n", second_arr.pop());

    printf("'A' = %i 'Z' = %i  'a' = %i 'z' = %i\n", 'A', 'Z', 'a', 'z');
    printf("0x80 = %i\n", 0x80);
    
    unsigned char* str = (unsigned char*) calloc(20, sizeof(char));
    scanf("%s", str);
    printf("leng of жопа %i\n", length_1(str));

    printf("test\b\b");
}