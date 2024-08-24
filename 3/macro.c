/* A program for macro expansion with provision for argument substitution 
(#define in full generality)*/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define MAXMACROS 20
#define MAXMACROCHARSIZE 100
#define MAXSTRSIZE 200
#define MAX 1000

char macro_variable[MAXMACROS][MAXMACROCHARSIZE];
char macro_value[MAXMACROS][MAXMACROCHARSIZE];
int macro_cnt=0;
FILE *output;

void storeMACROval(char str[MAXSTRSIZE]){
    char * token = strtok(str, " ");
    int cnt=0;
    // loop through the string to extract all other tokens
    if(strcmp(token, "#define") == 0){
        while( token != NULL ) {
            if(cnt==1){
                strcpy(macro_variable[macro_cnt], token);
            }
            else if(cnt==2){
                strcpy(macro_value[macro_cnt], token);
                macro_value[macro_cnt][strlen(macro_value[macro_cnt])-1] = '\0';
            }
            token = strtok(NULL, " ");
            cnt++;
        }
        macro_cnt++;
    }
    return;
}

int find(char* s, char* oldW, char* newW){
    int i, cnt = 0;
    int newWlen = strlen(newW);
    int oldWlen = strlen(oldW);
  
    // Counting the number of times old word
    // occur in the string
    int flag = 0;
    for (i = 0; s[i] != '\0'; i++) {
        if(s[i] == '"' || s[i] == '\''){
            char c = s[i];
            i++;
            while(s[i] != c && i<strlen(s)){
                i++;
            }
        }
        else if (strstr(&s[i], oldW) == &s[i]) {
            cnt++;
            // Jumping to index after the old word.
            i += oldWlen - 1;
        }
    }
    return cnt;
}

char* findAndReplace(char* s, char* oldW, char* newW, int cnt)
{
    char* result = NULL;
    int i;
    int newWlen = strlen(newW);
    int oldWlen = strlen(oldW);
  
    // Making new string of enough length
    result = (char*)malloc(i + cnt * (newWlen - oldWlen) + 1);
  
    i = 0;
    while (*s) {
        // compare the substring with the result
        if (strstr(s, oldW) == s) {
            strcpy(&result[i], newW);
            i += newWlen;
            s += oldWlen;
        }
        else
            result[i++] = *s++;
    }
  
    result[i] = '\0';
    
    return result;
}

void replaceMACRO(char str[MAXSTRSIZE]){
    char *result = NULL;
    int i;
    if(str[0] != '\n'){
        for(i=0; i<macro_cnt; i++){
            if(result == NULL){
                int cnt = find(str, macro_variable[i], macro_value[i]);
                if(cnt!=0){
                    result = findAndReplace(str, macro_variable[i], macro_value[i], cnt);
                }
            }
            else{
                int cnt = find(result, macro_variable[i], macro_value[i]);
                if(find(result, macro_variable[i], macro_value[i])!=0){
                    result = findAndReplace(result, macro_variable[i], macro_value[i], cnt);
                }
            }
        }
    }
    if(result == NULL){
        fprintf(output ,"%s", str);
    }
    else{
        fprintf(output ,"%s", result);
    }
}

int main(int argc, char const *argv[]) {
    // Opening the files
	FILE *input;
    char str[MAXSTRSIZE];
    int i,j;
    char s[100] = "MAX MAXSTRSIZE ";
	input = fopen(argv[1], "r");
	if(input == NULL) {
		printf("Unable to read file...\n");
		return -1;
	}
    // Writing the code to output file.
	output = fopen("output.c", "w");
	if(output == NULL) {
		printf("Unable to write file...\n");
		return -1;
	}
    while(!feof(input)){
        fgets(str, MAXSTRSIZE, input);
		if (str[0] == '#' && str[1]=='d') {
			storeMACROval(str);
		}
        else{
            replaceMACRO(str);
        }
    }

    printf("MACRO NAME AND VALUE:\n");
    for(i=0; i<macro_cnt; i++){
        printf("%s - %s\n", macro_variable[i], macro_value[i]);
    }
}