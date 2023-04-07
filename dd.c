#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char*
reduce(char* original_input)
{
    int input_size = strlen(original_input) - 1;

    char* reduced_input;
    char* error_message = (char*)malloc(sizeof(char) * input_size);
    char* error_keyword = (char*)malloc(sizeof(char) * 10);         //strlen(keyword)

    while(input_size > 0)
    {
        printf("%d\n", input_size);
        for(int i=0; i<=strlen(original_input) - input_size; i++)
        {
            char* head;
            char* tail;
            if(0 > i-1)
                head = "";
            else{
                head = (char*)malloc(sizeof(char) * i);
                strncpy(head, original_input, i);
            }
            if(i+input_size > strlen(original_input)-1)
                tail = "";
            else{    // i = 0
                tail = (char*)malloc(sizeof(char) * (strlen(original_input)-i-input_size));
                strncpy(tail, original_input+i+input_size, strlen(original_input)-i-input_size);
            }

            //for test
            printf("(%d) head: %s, tail: %s\n", i, head, tail);

            // fork - pipe - write(head+tail);
            reduced_input = (char*)malloc(sizeof(char) * (strlen(original_input)-input_size-1));
            strcat(reduced_input, head);
            strcat(reduced_input, tail);
            free(head);
            free(tail);

            // for test
            printf("(%d) reduced input:%s\n", i, reduced_input);

            // read(error_message)
            // if(strstr(error_message, error_keyword) != NULL)
            //     break;
            // else
            //     reduce(reduced_input);
        }
        for(int i=0; i<=strlen(original_input)-input_size; i++)
        {
            char* mid;
            if(i > i+input_size-1)
            {
                mid = "";
            }
            else
            {
                mid = (char*)malloc(sizeof(char) * input_size);
                strncpy(mid, original_input+i, input_size);
            }
            printf("(%d) mid:%s\n", i, mid);

            // fork - pipe - write(mid)
            // read(error_message)
            free(mid);
            // if(strstr(error_message, error_keyword) != NULL)
            //     break;
            // else
            //     reduce(mid);
        }
        input_size = input_size - 1;
    }
    free(reduced_input);
    free(error_keyword);
    free(error_message);
    return original_input;
}

int
main()
{
    char* crashing_input = "abcde";
    reduce(crashing_input);
}
