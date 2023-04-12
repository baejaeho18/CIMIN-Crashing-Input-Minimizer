#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

pid_t child_pid ;
int input_count ;
char **argu ;
char output_path[30] ;
char error_keyword[30] ;
char target_program[30] ;
char latest_reduced_input[4096] ;
size_t latest_reduced_input_len ;

void
saveResult()
{
    FILE * fw = fopen(output_path, "w") ;
    size_t written ;
    written = 0 ;
    while (written < latest_reduced_input_len) {
        written += fwrite(latest_reduced_input + written, 1, latest_reduced_input_len - written, fw) ;
    }
    printf("reduced size: %zd\n", latest_reduced_input_len) ;
    fclose(fw) ;
}

void
timeout(int sig)
{
    if(sig == SIGALRM)
    {
        puts("Time out!");
        kill(child_pid, SIGTERM);
        saveResult();
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
        saveResult();
    }
    exit(0);
}

void
reduce(char* t, size_t t_len)
{
    char tm[4096] ;
    size_t tm_len ;
    char err_msg[4096];

    memcpy(tm, t, t_len) ;
    tm_len = t_len ;
    size_t s = tm_len - 1 ;

    // fork
    int exit_code ;
    // pipe
    int p2c[2] ;
    int c2p[2] ;

    while (s > 0) {
        for (int i = 0 ; i <= tm_len - s ; i++)
        {
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

            printf("%d:%zd ", input_count++, headtail_len) ;
            
            // 나중에 while 밖으로 빼야하는 부분
            if (pipe(p2c) == -1 || pipe(c2p) == -1) 
            {
                fprintf(stderr, "Pipe Failed") ;
                exit(1) ;
            }
        
            // fork, exec, pipe
            if (child_pid = fork())
            {    // parent
                close(p2c[0]) ;
                close(c2p[1]) ;
                write(p2c[1], headtail, headtail_len) ;
                close(p2c[1]) ;
                alarm(3) ;
                wait(&exit_code) ;
                alarm(0) ;
                ssize_t s ;
                while ((s = read(c2p[0], err_msg, 4096)) > 0) {
                    err_msg[s + 1] = 0x0 ;
                }
                // check if error_message contains error_keyword
                printf("%s\n", err_msg) ;
                if(strstr(err_msg, error_keyword) != NULL) {
                    printf("reduced: %s\n", headtail) ;
                    if(headtail_len <= latest_reduced_input_len){
                        memcpy(latest_reduced_input, headtail, headtail_len) ;
                        latest_reduced_input_len = headtail_len ;
                    }
                    reduce(headtail, headtail_len) ;
                }
            }
            else
            {   // child
                close(p2c[1]) ;
                close(c2p[0]) ;
                dup2(p2c[0], 0) ;
                dup2(c2p[1], 2) ;
                close(p2c[0]) ;
                execv(target_program, argu) ;
            }
        }
        // mid

        s-- ;
    }
}

int
main(int argc, char* argv[])
{
    // signal
    signal(SIGALRM, timeout) ;   // output_path
    signal(SIGINT, keycontrol) ; // output_path

    // Used to get option(input, key error, ouput)
    int op ; // option
    char input_path[30] ;
    argu = (char **)malloc(sizeof(char *) * (argc - 5)) ;
    int argu_idx = 0 ;
    // get option i, m, o
    while ( (op = getopt(argc, argv, ":i:m:o:")) != -1 )
    {
        switch (op)
        {
            case 'i' :
                memcpy(input_path, optarg, strlen(optarg)) ;
                break;
            case 'm' :
                memcpy(error_keyword, optarg, strlen(optarg)) ;
                break;
            case 'o' :
                memcpy(output_path, optarg, strlen(optarg)) ;
                break;
            case ':' :
                printf("err : %c option requires string\n", optopt) ;
                return 0 ;
            case '?' :
                printf("err : %c is undetermined option\n", optopt) ;
                return 0 ;
        }
    }
    strcpy(target_program, argv[optind]) ;
    for (int i = optind; i < argc; i++) {
        argu[argu_idx++] = argv[i] ;
    }
    argu[argc] = NULL;
    // printf("i : %s\n", input_path) ;
    // printf("m : %s\n", error_keyword) ;
    // printf("o : %s\n", output_path) ;
    // printf("p : %s\n", target_program) ;
    // printf("argu: ") ;
    // for (int i = 0; i < argu_idx; i++) {
    //     printf("%s ", argu[i]) ;
    // }

    
    // load testcases
    FILE * fp = fopen(input_path, "r") ;
    char t[4096] ;
    size_t t_len = 0;
    while (!feof(fp)) {
        t_len = fread(t + t_len, 1, 4096, fp) ;
    }
    fclose(fp) ;

    // delta debuggin
    memcpy(latest_reduced_input, t, t_len) ;
    latest_reduced_input_len = t_len ;
    reduce(t, t_len) ;

    saveResult();
    return 0 ;
}
