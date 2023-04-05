// C program to demonstrate use of fork() and pipe()
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
 
int
main()
{
    // Used to store two ends of pipe
    int p2c[2];
    int c2p[2];
 
    char fixed_str[] = "forgeeks.org";
    char input_str[100];
    pid_t pid;
 
    if (pipe(p2c) == -1) 
    {
        fprintf(stderr, "Pipe Failed");
        return 1;
    }
    if (pipe(c2p) == -1) 
    {
        fprintf(stderr, "Pipe Failed");
        return 1;
    }
 
    scanf("%s", input_str);
    pid = fork();
 
    if (pid < 0) {
        fprintf(stderr, "fork Failed");
        return 1;
    }
    else if (pid > 0) {   // Parent process : delta debugging
        char concat_str[100];
 
        close(p2c[0]); // Close reading end of first pipe
 
        // Write input string and close writing end of first pipe.
        write(p2c[1], input_str, strlen(input_str) + 1);
        close(p2c[1]);
 
        // Wait for child to send a string
        wait(NULL);
 
        close(c2p[1]); // Close writing end of second pipe
 
        // Read string from child, print it and close
        // reading end.
        read(c2p[0], concat_str, 100);
        printf("Concatenated string %s\n", concat_str);
        close(c2p[0]);
    }
    else {  // child process : run target program and get error message
        close(p2c[1]); // Close writing end of first pipe
 
        // Read a string using first pipe
        char concat_str[100];
        read(p2c[0], concat_str, 100);
 
        // Concatenate a fixed string with it
        int k = strlen(concat_str);
        int i;
        for (i = 0; i < strlen(fixed_str); i++)
            concat_str[k++] = fixed_str[i];
 
        concat_str[k] = '\0'; // string ends with '\0'
 
        // Close both reading ends
        close(p2c[0]);
        close(c2p[0]);
 
        // Write concatenated string and close writing end
        write(c2p[1], concat_str, strlen(concat_str) + 1);
        close(c2p[1]);
 
        exit(0);
    }



}