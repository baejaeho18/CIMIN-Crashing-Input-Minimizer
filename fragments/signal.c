#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

void 
handler_sigint(int sig) 
{
    if (sig == SIGINT) { // ctrl + c pressed
        int temp = 99;
        printf("\n%d\n", temp); // prints out the size of the shortest crashing input found so far
        // TODO : produces output file > 함수 만들어서 호출 ; size, result
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
    kill(cur, SIGTERM);
}

int 
main() 
{
    // signal(SIGINT, handler_sigint);
    
    signal(SIGALRM, handler_sigalrm);

    struct itimerval t;

    t.it_value.tv_sec = 3;
    t.it_interval = t.it_value;

    setitimer(ITIMER_REAL, &t, 0x0);

    while(1);
}