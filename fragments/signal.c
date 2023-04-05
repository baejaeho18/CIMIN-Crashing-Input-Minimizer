#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>

void
read_file()
{
    FILE *fp;
    int size;

    fp = fopen("output.txt", "r");
    
    fscanf(fp, "%d", &size);
    printf("%d\n", size); // prints out the size of the shortest crashing input found so far
    
    fclose(fp);
    return;
}

void
save_file() 
{
    FILE *fp;
    char res[] = "result";
    int size = strlen(res);

    fp = fopen("output.txt", "w");
    fprintf(fp, "%d\n%s", size, res);
    
    fclose(fp);
    return;
}

void 
handler_sigint(int sig) 
{
    if (sig == SIGINT) { // ctrl + c pressed
        read_file(); // prints the result found so far
        save_file(); // produces output
        exit(0); // terminate the execution
    }
}

void 
handler_sigalrm(int sig) 
{ 
    // kill process 
    pid_t cur;
    cur = getpid();
    printf("Current process = %d\n", cur);
    save_file();
    kill(cur, SIGTERM);
}

int 
main() 
{
    signal(SIGINT, handler_sigint);
    
    signal(SIGALRM, handler_sigalrm);

    struct itimerval t;

    t.it_value.tv_sec = 3;
    t.it_interval = t.it_value;

    setitimer(ITIMER_REAL, &t, 0x0);

    while(1);
}