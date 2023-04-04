#include <stdio.h>
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
main(int argc, char *argv[]){
    
    signal(SIGALRM, timeout);
    signal(SIGINT, keycontrol);
    alarm(3);
    while(1){
        puts("running...");
        sleep(1);
    }

    return 0;
}
