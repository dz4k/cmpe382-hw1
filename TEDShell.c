#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// This is a bare-bone TEDShell
// You need to write your code to work as described in the document.

// Use following command to compile:
// g++ TEDShell.c -o TEDShell

// To run the program
// ./TEDShell

// To run testcases
// ./TEDShell-tester


void readline(char* buffer){
    fgets(buffer, 100, stdin);
    buffer[strcspn(buffer, "\n")] = 0;
}
int main(int argc, char *argv[])
{

    char input[100];

    printf("TEDshell> ");
    readline(input);

    while (strcmp(input, "exit") != 0)
    {

        printf("this is a bare-bone TEDShell\n");
        printf("You provided the command : %s\n", input);

        printf("TEDShell> ");
        readline(input);        
    }

    return 0;
}