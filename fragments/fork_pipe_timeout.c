#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

void 
timeout(int sig)
{
    if(sig==SIGALRM)
        puts("Time out!");
    exit(0);
}
void
keycontrol(int sig)
{
    if(sig==SIGINT)
        puts("CTRL+C pressed");
    exit(0);
} 

int
main(int argc, char* argv[])
{
    // Used to store two ends of pipe
    int p2c[2];
    int c2p[2];

    char crashing_input[100] = "crashing input like json, xml, balance";
    pid_t pid;
    
    signal(SIGALRM, timeout);
    signal(SIGINT, keycontrol);
    
    if (pipe(p2c) == -1 || pipe(c2p) == -1) 
    {
        fprintf(stderr, "Pipe Failed");
        return 1;
    }
 
    if ((pid = fork()) < 0)
    {
        fprintf(stderr, "fork Failed");
        return 1;
    }
    else if (pid > 0)
    {   // Parent process : delta debugging
        printf("parent: I'm parent\n");
        close(p2c[0]); // Close reading end of first pipe
        
        char reduced_input[100];
        char error_message[100];

        // delta debugging algorithm
        for(int i=0;i<strlen(crashing_input);i++){
            reduced_input[i] = crashing_input[i];
        }
 
        // Write input string and close writing end of first pipe
        write(p2c[1], reduced_input, strlen(reduced_input) + 1);
        alarm(3);

        close(p2c[1]);  // go to signal or terminal funciton
        wait(NULL);     // Wait for child to send a string
        close(c2p[1]);  // go to signal or terminal funciton

        read(c2p[0], error_message, 100);
        alarm(0);
        printf("parent: %s\n", error_message);
        close(c2p[0]); // go to signal or terminal funciton

        // return recursive delta debugging
    }
    else 
    {  // child process : run target program and get error message
        close(p2c[1]); // Close writing end of first pipe
        dup2(c2p[1], 2);
        printf("child: I'm child\n");
 
        // Read a string using first pipe
        char reduced_input[100];
        read(p2c[0], reduced_input, 100);
        printf("child: %s\n", reduced_input);

        // sleep(3);

        if(strcmp(reduced_input, crashing_input) == 0)
            fprintf(stderr, "child: std error\n");
        char error_message[100] = "str error";
 
        // Close both reading ends
        close(p2c[0]);
        close(c2p[0]);
 
        // Write concatenated string and close writing end
//        write(c2p[1], error_message, strlen(error_message) + 1);
        close(c2p[1]);
 
        exit(0);
    }

}
