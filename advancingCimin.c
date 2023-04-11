#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

char t[4096] ;
size_t t_len ;

pid_t child_pid ;


void
timeout(int sig)
{
    if(sig == SIGALRM)
    {
        puts("Time out!");
        kill(child_pid, SIGTERM);
    }
    
    exit(0);
}

void
keycontrol(int sig)
{
    if(sig == SIGINT)
    {
        puts("Ctrl+c pressed!");
        kill(child_pid, SIGTERM);
    }
    exit(0);
}

int
main()
{
    // signal
    signal(SIGALRM, timeout) ;   // output_path
    signal(SIGINT, keycontrol) ; // output_path

    // load testcases
    FILE * fp = fopen("crash.json", "r") ;
    t_len = 0;
    while (!feof(fp)) {
        t_len = fread(t + t_len, 1, 4096, fp) ;
    }
    fclose(fp) ;

    // fork
    int exit_code ;
    // pipe
    int p2c[2];
    int c2p[2];
    
            

    char err_msg[4096];
    // delta debugging
    char tm[4096] ;
    size_t tm_len ;

    memcpy(tm, t, t_len) ;
    tm_len = t_len ;
    size_t s = tm_len - 1 ;

    int input_count ;

    while (s > 0) {
        for (int i = 0 ; i <= tm_len - s ; i++) {
            char head[4096] ;
            char tail[4096] ;
            size_t head_len ;
            size_t tail_len ;

            memcpy(head, tm, i) ;
            head_len = i;
            memcpy(tail, tm + i + s, tm_len - 1 - i - s + 1) ;
            tail_len = tm_len - 1 - i - s + 1 ;

            char headtail[4096] ;
            size_t headtail_len ;

            memcpy(headtail, head, head_len) ;
            memcpy(headtail + head_len, tail, tail_len) ;
            headtail_len = head_len + tail_len ;

            printf("%s\n", headtail);
            printf("%d : %zd\n", input_count++, headtail_len);
            
            // 나중에 while 밖으로 빼야하는 부분
            if (pipe(p2c) == -1 || pipe(c2p) == -1) 
            {
                fprintf(stderr, "Pipe Failed");
                exit(1);
            }  

            // fork, exec, pipe
            if (child_pid = fork()) {
            // parent
                close(p2c[0]);
                close(c2p[1]);
//                dprintf(p2c[1], "%s", headtail) ;
                write(p2c[1], headtail, headtail_len) ;
                close(p2c[1]) ;
                alarm(3) ;
                wait(&exit_code) ;
                // exit_code가 signaled termination : exit(0);
                alarm(0) ;
                ssize_t s ;
                while ((s = read(c2p[0], err_msg, 4096)) > 0) {
                    err_msg[s + 1] = 0x0 ;
                    printf(">%s\n", err_msg) ;
                }
                // check if error_message contains error_keyword
                char err_keyword[20];
                memcpy(err_keyword, "heap-buffer-overflow", 17);
                printf("%s", err_msg);
                printf("\n%s", err_keyword);
                if(strstr(err_msg, err_keyword) == NULL)
                    printf("no markup\n");
                //    break;
                else
                    printf("good reduced\n");
                //    return reduce(reduced_input);
            }
            else {
            // child
                close(p2c[1]);
                close(c2p[0]);
                dup2(p2c[0], STDIN_FILENO /*read standard input*/) ;
                dup2(c2p[1], 2 /*write standard error*/) ;
                close(p2c[0]);
                execl("./jsmn/jsondump", "./jsmn/jsondump", (char *) 0x0) ;
            }
            // output file save
            // char filename[16] ;
            // sprintf(filename, "input%d", input_count++) ;
            // FILE * fw = fopen(filename, "w") ;
            // size_t written ;
            // written = 0 ;
            // while (written < headtail_len) {
            //     written += fwrite(headtail + written, 1, headtail_len - written, fw) ;
            // }
            // fclose(fw) ;
        }
        // mid

        s-- ;
    }

}
