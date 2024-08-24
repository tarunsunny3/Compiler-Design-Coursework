#include <stdio.h>
#include "comments.h"

void removeComments(FILE *input, FILE *output) {
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
}