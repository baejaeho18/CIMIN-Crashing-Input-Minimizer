#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char*
reduce(char* original_input)
{
    int input_size = strlen(original_input) - 1;

    char* reduced_input = (char*)malloc(sizeof(char) * input_size);
    char* error_message = (char*)malloc(sizeof(char) * input_size);
    char* error_keyword = (char*)malloc(sizeof(char) * 10);         //strlen(keyword)

    while(input_size > 0)
    {
        unsigned int stop;
        for(int i=0;i<strlen(original_input) - input_size; i++)
        {
            char* head;
            char* tail;
            if(0 > i-1)
                head = "";
            else{
                head = (char*)malloc(sizeof(char) * (i+1));
                strncpy(head, original_input, i);
            }
            if(i+input_size > input_size)
                tail = "";
            else{    // i = 0
                tail = (char*)malloc(sizeof(char) * (2-i));
                strncpy(tail, original_input+i+input_size, 1-i);
            }

            //for test
            printf("head: %s, tail: %s\n", head, tail);

            // fork - pipe - write(head+tail);
            
            strcat(reduced_input, head);
            strcat(reduced_input, tail);

            // for test
            printf("reduced input:%s\n", reduced_input);

            // read(error_message)
            if(strstr(error_message, error_keyword) != NULL)
                break;
            else
                reduce(reduced_input);
        }
        for(int i=0; i<strlen(original_input)-input_size; i++)
        {
            char* mid;
            if(i > i + input_size - 1)
                mid = "";
            else if(i+input_size-1 > input_size)
            {
                mid = (char*)malloc(sizeof(char) * (input_size + 2 - i));
                strncpy(mid, original_input+i, input_size + 1 - i);
            }
            else
            {
                mid = (char*)malloc(sizeof(char) * (input_size + 1));
                strncpy(mid, original_input+i, input_size);
            }
            printf("mid:%s\n", mid);

            // fork - pipe - write(mid)
            // read(error_message)

            if(strstr(error_message, error_keyword) != NULL)
                break;
            else
                reduce(mid);
        }
        input_size = input_size - 1;
    }

    return original_input;
}

int
main()
{
    char* crashing_input = "abcdef";
    reduce(crashing_input);
}
