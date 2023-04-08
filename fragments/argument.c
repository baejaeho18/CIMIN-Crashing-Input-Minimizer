#include <stdio.h>
#include <unistd.h>
#include <string.h>

char error_keyword[30];
char* target_p[5];
char output_path[30];

int main (int argc, char* argv[]){
    // Used to get option(input, key error, ouput)
    int op; // option
    char input_path[30];
    // get option i, m, o
    while ( (op = getopt(argc, argv, ":i:m:o:")) != -1 )
    {
        switch (op)
        {
            case 'i':
                memcpy(input_path, optarg, strlen(optarg));
                break;
            case 'm':
                memcpy(error_keyword, optarg, strlen(optarg));
                break;
            case 'o':
                memcpy(output_path, optarg, strlen(optarg));
                break;
            case ':':
                printf("err : %c option requires string\n", optopt);
                return 0;
            case '?':
                printf("err : %c is undetermined option\n", optopt);
                return 0;
        }
    }
    if(argc < 7)    // how to change?
    {
        printf("err : three parameters must be given with options\n");
        return 0;
    }
    int cnt=0;
    while(optind < argc)
    {
        target_p[cnt] = argv[optind++];
    }
    printf("i : %s\n", input_path);
    printf("m : %s\n", error_keyword);
    printf("o : %s\n", output_path);
    for(int i=0;i<5;i++){
        if(target_p[i] != NULL)
            printf("%s ", target_p[i]);
    }
    printf("\n");

    return 0;
}