#include <stdio.h>

int main() {
   FILE *fp;
   char res[] = "result";

   fp = fopen("output.txt", "w");
   fprintf(fp, "%s", res);
   fclose(fp);

    /*
    size_t len = strlen(res);
    fp = fopen("output.bin", "wb");
    fwrite(res, sizeof(char), len, fp);
    fclose(fp);

    */

   return 0;
}
