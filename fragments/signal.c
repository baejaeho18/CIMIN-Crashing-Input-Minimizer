#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>

const char *
read_file()
{
    FILE *fp;
    char *str;

    fp = fopen("output.txt", "r");
    
    fscanf(fp, "%s", str);
    
    fclose(fp);

    return str;
}

void
save_file() 
{
    FILE *fp;
    char res[] = "result to find hi";

    fp = fopen("output.txt", "w");
    fprintf(fp, "%s", res);
    
    fclose(fp);
    return;
}

void 
handler_sigint(int sig) 
{
    if (sig == SIGINT) { // ctrl + c pressed
        const char *str;
        str = read_file();
        printf("%lu\n", strlen(str)); // prints the result found so far
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