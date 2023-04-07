#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>

void load(char* filename) {
    FILE *fp;
    char *buffer;
    long file_size;

    fp = fopen(filename, "rb"); // open the file in binary mode
    if (fp == NULL) {
        printf("Error opening file.\n");
        return;
    }

    // find the size of the file
    fseek(fp, 0, SEEK_END); // seek to the end of the file
    file_size = ftell(fp); // get the current file position (which is the size of the file)
    rewind(fp); // rewind the file pointer to the beginning of the file

    // allocate memory for the buffer to hold the file contents
    buffer = (char*) malloc(file_size + 1); // add space for null terminator
    if (buffer == NULL) {
        printf("Error allocating memory.\n");
        return;
    }

    // read the entire file into the buffer
    fread(buffer, file_size, 1, fp);

    // add a null terminator to the end of the buffer
    buffer[file_size] = '\0';

    // print the contents of the buffer
    printf("%s", buffer);

    // free the memory allocated for the buffer
    free(buffer);

    // close the file
    fclose(fp);
}

int main() {
    load("output.txt");
    
    return 0;
}