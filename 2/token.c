#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "comments.h"

// Checking for a valid delimiter
int isValidDelimiter(char ch) {
   	if (ch == '\n' || ch == ' ' || ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == ',' || ch == ';' || ch == '>' || ch == '<' || ch == '=' || ch == '(' || ch == ')' || ch == '[' || ch == ']' || ch == '{' || ch == '}')
   		return 1;
	return 0;
}

// Checking for a valid operator
int isValidOperator(char ch) {
   	if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '>' || ch == '<' || ch == '=')
   		return 1;
   	return 0;
}

// Checking for a valid integer
int isValidInteger(char* str) {
   	int i, len = strlen(str);
   	if (len == 0)
   		return 0;
   	for (i = 0; i < len; i++) {
		if (str[i] > '9' || str[i] < '0')
			return 0;
   	}
   	return 1;
}

// Checking for a valid identifier (REQUIRED)
int isvalidIdentifier(char* str) {
	int i, len = strlen(str);
	if ((str[0] > '0' && str[0] < '9') || isValidDelimiter(str[0]))
   		return 0;
	for (i = 0; i < len; i++) {
		if (str[i] <= 'z' && str[i] >= 'a') {
			continue;
		} else if (str[i] <= 'Z' && str[i] >= 'A') {
			continue;
		} else if (str[i] <= '9' && str[i] >= '0') {
			continue;
		} else if (str[i] == '_') {
			continue;
		} else {
			return 0;
		}
	}
	return 1;
}

// Checking for a valid keyword
int isValidKeyword(char* str) {
   	if (!strcmp(str, "if") || !strcmp(str, "else") || !strcmp(str, "while") || !strcmp(str, "do") || !strcmp(str, "break") || !strcmp(str, "continue") || !strcmp(str, "int")
   	|| !strcmp(str, "double") || !strcmp(str, "float") || !strcmp(str, "return") || !strcmp(str, "char") || !strcmp(str, "case") || !strcmp(str, "char")
   	|| !strcmp(str, "sizeof") || !strcmp(str, "long") || !strcmp(str, "short") || !strcmp(str, "typedef") || !strcmp(str, "switch") || !strcmp(str, "unsigned")
   	|| !strcmp(str, "void") || !strcmp(str, "static") || !strcmp(str, "struct") || !strcmp(str, "goto"))
   		return 1;
   	return 0;
}

// Checking for a real number
int isRealNumber(char* str) {
   	int i, len = strlen(str);
   	int hasDecimal = 0;
   	if (len == 0)
   		return 0;
   	for (i = 0; i < len; i++) {
	  	if (str[i] > '9' || str[i] < '0')
	  		return 0;
		if (str[i] == '.')
	  		hasDecimal = 1;
   	}
   	return hasDecimal;
}

// Checking for variables inside print statements
int isPrint(char* str) {
	int len = strlen(str);
	if (str[0] == '"' && str[len-1] == '"')
		return 1;
	return 0;
}

// Retrieve a substring
char* subString(char* str, int left, int right) {
   	int i;
   	char* subStr = (char*)malloc(sizeof(char)*(right-left+2));
	for (i = left; i <= right; i++)
	  	subStr[i-left] = str[i];
   	subStr[right-left+1] = '\0';
   	return subStr;
}

// Detecting tokens
void detectTokens(char* str, int line) {
   	int left = 0, right = 0, declaration = 0;
   	int length = strlen(str);
   	while (right < length && left <= right) {
	  	if (isValidDelimiter(str[right]) == 0)
	  		right++;
	  	if (isValidDelimiter(str[right]) && left == right) {
		 	right++;
		 	left = right;
	  	} else if (isValidDelimiter(str[right]) && left != right || (right == length && left != right)) {
		 	char* subStr = subString(str, left, right-1);
			if (isValidKeyword(subStr)) {
				declaration = 1;
			}
			if (isvalidIdentifier(subStr) && !isValidDelimiter(str[right-1])) {
				if(!isValidKeyword(subStr) && !isRealNumber(subStr) && !isValidInteger(subStr) && !isPrint(subStr)) {
					if (declaration) {
						printf(" valid   | '%6s' | decl | %d:%d\n", subStr, line, left);
						declaration = 0;
					} else {
						printf(" valid   | '%6s' | defn | %d:%d\n", subStr, line, left);
					}
				}
			} else if (!isvalidIdentifier(subStr) && !isValidDelimiter(str[right-1])) {
				if (!isValidKeyword(subStr) && !isRealNumber(subStr) && !isValidInteger(subStr) && !isPrint(subStr)) {
					printf("invalid  | '%6s' | ---- | %d:%d\n", subStr, line, left);
				}
			}	
		 	left = right;
	  	}
   	}
   	return;
}
 
int main(int argc, char const *argv[]) {

	// Opening the files
	FILE *input, *output, *fp;
	input = fopen(argv[1], "r");
	if(input == NULL) {
		printf("Unable to read file...\n");
		return -1;
	}

	// Writing the code without comments
	output = fopen("output.txt", "w");
	if(output == NULL) {
		printf("Unable to write file...\n");
		return -1;
	}

	// Ignoring variables in comments
	printf("Removing comments...\n");
	removeComments(input, output);
	printf("Comments removed...\n");
	fp = fopen("output.txt", "r");
	if(fp == NULL) {
		printf("Unable to read file...\n");
		return -1;
	}

	// Reading every word and classifying
	char str[100];
	int line = 0;
	printf("------------------------------------------\n");
	printf("category | variable | type | line:position\n");
	printf("------------------------------------------\n");

	while(!feof(fp)) {
		fgets(str, 100, fp);
		line++;
		if (str[0] == '#') {
			continue;
		} else if (str[0] == '/' && str[1] == '/') {
			continue;
		} else {
			detectTokens(str, line);
		}
	}

	printf("------------------------------------------\n");
	printf("End of file...\n");
	fclose(fp);

	// Deteling the without comments file
	remove("output.txt");

	return 0;

}