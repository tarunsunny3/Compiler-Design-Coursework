// Program to remove comments.
#include <stdio.h>

int main(int argc, char const *argv[]) {

    // Opening the files
    FILE *input, *output;
    input = fopen(argv[1], "r");
    output = fopen(argv[2], "w");
    if(input == NULL) {
        printf("Unable to read file...\n");
        return -1;
    } 
    if(output == NULL) {
        printf("Unable to write to file...\n");
        return -1;
    }

    // Reading character by character and writing to the file
    char c = fgetc(input);
    int flag = 0;
    while(c != EOF) {
        if(c == '"' || c == '\'') {
            flag = ((flag + 1) % 2);
        }
        if(c != '/' && flag == 0) {
            fputc(c, output);
        } else {
            char d = fgetc(input);
            if(d == '/' && flag == 0) {
                while(1){
                    // a cmt
                    if(fgetc(input) == '\n' && flag == 0) {
                        fputc('\n', output);
                        break;
                    }
                } 
            } else if(d == '*' && flag == 0) {
                while(1) {
                    if(fgetc(input) == '*' && flag == 0) {
                        if(fgetc(input) == '/' && flag == 0) {
                            break;
                        } else {
                            continue;
                        }
                    }
                }
            } else {
                fputc(c, output);
                fputc(d, output);
            }
        }
        c = fgetc(input);
    }

    fclose(input);
    fclose(output);
    return 0;

}
