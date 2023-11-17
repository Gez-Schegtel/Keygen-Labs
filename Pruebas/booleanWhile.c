#include<stdio.h> /* The printf thingy. */
#include<stdbool.h> /* Required to use the data type "bool". */
#include<stdlib.h> /* This is for the system() function. */
#include<unistd.h> /* This is for the sleep() function. */

bool condiciones(void){
    return(true);
}

int main(void){
    int count = 0;
    
    printf("Everything is ok. \n");
    sleep(3);
    system("clear");

    while(condiciones() && count < 3){
        printf("Or is it? \n");
        count++;
    }
    
    
    return(0);
}