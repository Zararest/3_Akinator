#include <stdio.h>
#include <stdlib.h>
#include "headers/tree.h"


int main(){

    /*FILE* tmp = fopen("test.bin", "wb");
    int len = 50;
    char* line = (char*) calloc(len, sizeof(char));

    scanf("%s", line);
    fwrite(&len, sizeof(int), 1, tmp);
    fwrite(line, sizeof(char), len, tmp);
    fclose(tmp);

    tmp = fopen("test.bin", "rb");*/

    //Tree test_tree;
    
    
    Akinator test_akinator;

    
    test_akinator.guess_and_add();
    test_akinator.guess_and_add();
    //test_tree.dump_tree();
}