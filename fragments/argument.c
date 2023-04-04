#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main (int argc, char* argv[]){
    int op; // option
    char input_path[30];
    char keyword[30];
    char output_path[30];
    while( (op = getopt(argc, argv, "i:m:o:")) != -1){
        switch(op){
            case 'i':
                memcpy(input_path, optarg, strlen(optarg));
                break;
            case 'm':
                memcpy(keyword, optarg, strlen(optarg));
                break;
            case 'o':
                memcpy(output_path, optarg, strlen(optarg));
                break;
            case '?':
                if(optopt == 'i' || optopt == 'm' || optopt == 'o')
                    printf("err : %c option requires string\n", optopt);
                else
                    printf("err : undetermined option\n");
                break;
        }
    }
    if(strcmp(argv[argc-1], "a.out") == 0)
        printf("a.out\n");
    printf("i : %s\n", input_path);
    printf("m : %s\n", keyword);
    printf("o : %s\n", output_path);
}