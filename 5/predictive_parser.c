//Importing the libraries.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXPRODUCTIONS 30
#define STACKSIZE 100


//Defining a structure array for productions.
struct productions{
    char lhs[MAXPRODUCTIONS][10];
    char rhs[MAXPRODUCTIONS][10];
    char first[MAXPRODUCTIONS][10];
    int first_cnt[MAXPRODUCTIONS];
    int count;
}pro;

// Defining the global variables.
char First[MAXPRODUCTIONS][10], Follow[MAXPRODUCTIONS][10];
int first_cnt[MAXPRODUCTIONS], follow_cnt[MAXPRODUCTIONS];
char NT[20], T[20];
int cnt_nt=0, cnt_t=0;
int table[MAXPRODUCTIONS][MAXPRODUCTIONS];
char parsestr[100];

// Defining stack and it's functions.
struct stack
{
    int count;
    int top;
    char items[STACKSIZE];
};

struct stack s;

int isempty()
{
    if (s.top == -1)
        return 1;
    else
        return 0;
}

int isfull()
{
    if (s.count == STACKSIZE)
        return 1;
    else
        return 0;
}

int peek()
{
    return s.items[s.top];
}

char pop()
{
    char data;

    if (!isempty())
    {
        data = s.items[s.top];
        s.top--;
        s.count--;
        return data;
    }
    else
    {
        printf("Could not retrieve data, Stack is empty.\n");
    }
}

void push(char data)
{
    if (!isfull())
    {
        s.top++;
        s.items[s.top] = data;
        s.count++;
    }
    else
    {
        printf("Could not insert data, Stack is full.\n");
    }
}

// Stack initialisation function.
void initStack()
{
    s.top = -1;
    s.count = 0;
    push('$');
    push(NT[0]);
    return;
}

// Stack printing function.
void printStack(){
    int i;
    printf("\nStack : ");
    for(i=0; i<s.count; i++){
        printf("%c ", s.items[i]);
    }
    printf("\n");
}

int isTerminal(char c){
    if(c<65 || c>90){
        return 1;
    }
    return 0;
}
int isNONTerminal(char c){
    if(c>=65 && c<=90){
        return 1;
    }
    return 0;
}

int getTid(char c){
    int i;
    for(i=0; i<cnt_t; i++){
        if(T[i]==c){
            return i;
        }
    }
}

int getNTid(char c){
    int i;
    for(i=0; i<cnt_nt; i++){
        if(NT[i]==c){
            return i;
        }
    }
}

// Function to check if the given Terminal is in the terminals array or not.
int checkInT(char c){
    int i;
    for(i=0;i<cnt_t;i++){
        if(c==T[i]){
            return 1;
        }
    }
    return 0;
}

// Function to check if the given Non Terminal is in the non-terminals array or not.
int checkInNT(char c){
    int i;
    for(i=0;i<cnt_nt;i++){
        if(c==NT[i]){
            return 1;
        }
    }
    return 0;
}

// Function that seperates all the T and NTs and store in different arrays.
void storeNTandT(){
    int i,j;
    char c;
    for(i=0; i<pro.count; i++){
        if(!checkInNT(pro.lhs[i][0])){
            NT[cnt_nt] = pro.lhs[i][0];
            cnt_nt++;
        }
        for(j=0; pro.rhs[i][j]!='\0'; j++){
            c = pro.rhs[i][j];
            if(c<'A' || c>'Z'){
                if(!checkInT(c) && c!='#'){
                    T[cnt_t]=c;
                    cnt_t++;
                }
            }
        }
    }
    T[cnt_t]='$';
    cnt_t++;
    printf("\nNon-terminals of the grammar: \n");
    for(i=0;i<cnt_nt;i++){
        printf("%c ",NT[i]);
    }
    printf("\n\nTerminals of the grammar: \n");
    for(i=0;i<cnt_t;i++){
        printf("%c ",T[i]);
    }
    printf("\n\n");
}

int inFirst(int id, char c){
    int i;
    for(i=0; i<first_cnt[id]; i++){
        if(First[id][i] == c){
            return 1;
        }
    }
    return 0;
}

int inFirstofEachProduction(int id, char c){
    int i;
    for(i=0; i<pro.first_cnt[id]; i++){
        if(pro.first[id][i] == c){
            return 1;
        }
    }
    return 0;
}

int inFollow(int id, char c){
    int i;
    for(i=0; i<follow_cnt[id]; i++){
        if(Follow[id][i] == c){
            return 1;
        }
    }
    return 0;
}

// Function to find FIRST recursively based on given requirements.
void getFirst(char f){
    int i, j, k, idf, flag = 0;
    idf = getNTid(f);
    if(first_cnt[idf] != 0){
        return;
    }
    // printf("first of f = %c\n", f);
    for(i=0; i<pro.count; i++){
        if(pro.lhs[i][0] == f){
            if(isTerminal(pro.rhs[i][0]) || pro.rhs[i][0]=='#'){
                // printf("T and %f %s->%s\n", f, pro.lhs[i], pro.rhs[i]);
                First[idf][first_cnt[idf]] = pro.rhs[i][0];
                first_cnt[idf]++;
                pro.first[i][pro.first_cnt[i]] = pro.rhs[i][0];
                pro.first_cnt[i]++;
            }
            else{
                j=0;
                while(j<strlen(pro.rhs[i])){
                    if(isTerminal(pro.rhs[i][j]) || pro.rhs[i][j]=='#'){
                        if(!inFirst(idf, pro.rhs[i][j])){
                            First[idf][first_cnt[idf]] = pro.rhs[i][j];
                            first_cnt[idf]++;
                        }
                        if(!inFirstofEachProduction(i, pro.rhs[i][j])){
                            pro.first[i][pro.first_cnt[i]] = pro.rhs[i][j];
                            pro.first_cnt[i]++;
                        }
                        break;
                    }
                    int id = getNTid(pro.rhs[i][j]);
                    if(first_cnt[id] == 0){
                        getFirst(pro.rhs[i][j]);
                    }
                    flag = 0;
                    for(k=0; k<first_cnt[id]; k++){
                        if(First[id][k] == '#'){
                            flag = 1;
                            break;
                        }
                    }
                    if(flag == 1 && j!=strlen(pro.rhs[i])-1){
                        // printf("Have #\n");
                        for(k=0; k<first_cnt[id]; k++){
                            if(!inFirst(idf, First[id][k]) && First[id][k] != '#'){
                                First[idf][first_cnt[idf]] = First[id][k];
                                first_cnt[idf]++;
                            }
                            if(!inFirstofEachProduction(i, First[id][k]) &&  First[id][k] != '#'){
                                pro.first[i][pro.first_cnt[i]] = First[id][k];
                                pro.first_cnt[i]++;
                            }
                        }
                    }
                    else{
                        // printf("no #\n");
                        for(k=0; k<first_cnt[id]; k++){
                            if(!inFirst(idf, First[id][k])){
                                First[idf][first_cnt[idf]] = First[id][k];
                                first_cnt[idf]++;
                            }
                            if(!inFirstofEachProduction(i, First[id][k])){
                                pro.first[i][pro.first_cnt[i]] = First[id][k];
                                pro.first_cnt[i]++;
                            }
                        }
                    }
                    if(flag == 0){
                        break;
                    }
                    j++;
                }
            }
        }
    }
    return;
}

// Function to find FOLLOW recursively based on given requirements.
void getFollow(char f){
    int i, j, k, l, m, idf, flag = 0, ep_flag=0, end_flag=0;
    idf = getNTid(f);
    if(follow_cnt[idf] != 0){
        return;
    }
    if(idf == 0 && !inFollow(idf, '$')){
        Follow[idf][follow_cnt[idf]] = '$';
        follow_cnt[idf]++;
    }
    for(i=0; i<pro.count; i++){
        for(j=0; j<strlen(pro.rhs[i]); j++){
            if(pro.rhs[i][j] == f){
                flag = 0;
                if(j != strlen(pro.rhs[i])-1){
                    // printf("j is not equal to len - 1\n");
                    for(k=j+1; k<strlen(pro.rhs[i]); k++){
                        if(isTerminal((pro.rhs[i][k]))){
                            Follow[idf][follow_cnt[idf]] = pro.rhs[i][k];
                            follow_cnt[idf]++;
                            break;
                        }
                        else{
                            int id = getNTid(pro.rhs[i][k]);
                            ep_flag = 0;
                            for(l=0;l<first_cnt[id];l++){
                                if(First[id][l] == '#'){
                                    ep_flag = 1;
                                    break;
                                }
                            }
                            if(ep_flag == 0){
                                for(l=0;l<first_cnt[id];l++){
                                    if(!inFollow(idf, First[id][l])){
                                        Follow[idf][follow_cnt[idf]] = First[id][l];
                                        follow_cnt[idf]++;
                                    }
                                }
                                break;
                            }
                            else{
                                for(l=0;l<first_cnt[id];l++){
                                    if(!inFollow(idf, First[id][l]) && First[id][l]!='#'){
                                        Follow[idf][follow_cnt[idf]] = First[id][l];
                                        follow_cnt[idf]++;
                                    }
                                }
                                if(k == strlen(pro.rhs[i])-1){
                                    // printf("k is equal to len - 1 after few first addition\n");
                                    flag = 1;
                                }
                            }
                        } 
                    }
                }
                if(j == strlen(pro.rhs[i])-1 || flag == 1){
                    int id = getNTid(pro.lhs[i][0]);
                    if(follow_cnt[id] == 0){
                        getFollow(pro.lhs[i][0]);
                    }
                    for(k=0; k<follow_cnt[id]; k++){
                        if(!inFollow(idf, Follow[id][k])){
                            Follow[idf][follow_cnt[idf]] = Follow[id][k];
                            follow_cnt[idf]++;
                        }
                    }
                }
            }
        }
    }
    return;
}

// Function that calls FIRST and FOLLOW for each and every NT and prints FIRST and FOLLOW.
void first_and_follow(){
    int i,j;
    for(i=0; i<cnt_nt; i++){
        first_cnt[i] = 0;
        follow_cnt[i] = 0;
    }
    for(i=0; i<cnt_nt; i++){
        getFirst(NT[i]);
    }
    for(i=0; i<cnt_nt; i++){
        getFollow(NT[i]);
    }
    printf("\n");
    for(i=0;i<cnt_nt;i++){
        printf("First of %c = ", NT[i]);
        for(j=0; j<first_cnt[i]; j++){
            printf("%c ", First[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    // for(i=0;i<pro.count;i++){
    //     printf("First of %c = ", pro.lhs[i][0]);
    //     for(j=0; j<pro.first_cnt[i]; j++){
    //         printf("%c ", pro.first[i][j]);
    //     }
    //     printf("\n");
    // }
    // printf("\n");
    for(i=0;i<cnt_nt;i++){
        printf("Follow of %c = ", NT[i]);
        for(j=0; j<follow_cnt[i]; j++){
            printf("%c ", Follow[i][j]);
        }
        printf("\n");
    }
}

// Function to construct the parsing table.
int TableConstruction(){
    int i, j, k;
    // Inintializing the table with -100.
    for(i=0;i<cnt_nt;i++){
		for(j=0; j<cnt_t; j++){
			table[i][j] = -100;
        }
	}
    for(i=0; i<pro.count; i++){
        int ntid = getNTid(pro.lhs[i][0]);
        for(j=0; j<pro.first_cnt[i]; j++){
            if(pro.first[i][j] != '#'){
                int tid = getTid(pro.first[i][j]);
                if(table[ntid][tid] == -100 || table[ntid][tid] == i){
                    table[ntid][tid] = i;
                }
                else{
                    printf("The grammar is NOT LL(1)\n");
                    return 0;
                }
            }
            else{
                for(k=0; k<follow_cnt[ntid]; k++){
                    int tid = getTid(Follow[ntid][k]);
                    if(table[ntid][tid] == -100 || table[ntid][tid] == i){
                        table[ntid][tid] = i;
                    }
                    else{
                        printf("The grammar is NOT LL(1)\n");
                        return 0;
                    }
                }
            }
        }
    }
    // Printing the parsing table.
    printf("\nPredictive Parsing Table:\n\n");
    printf(" \t\t");
    for(i=0; i<cnt_t; i++){
        printf("%c\t\t", T[i]);
    }
    printf("\n");
    for(i=0;i<cnt_nt;i++){
        printf("%c\t\t", NT[i]);
		for(j=0; j<cnt_t; j++){
            if(table[i][j] == -100){
                printf(".\t\t");
            }
            else{
			    printf("%s->%s\t\t", pro.lhs[table[i][j]], pro.rhs[table[i][j]]);
            }
        }
        printf("\n");
	}
    printf("\n");
    return 1;
}

// Function that parses the given string input.
int ParseString(){
    initStack();
    char s[100], x[3] = "$\0";
    strcpy(s, parsestr);
    strcat(s, x);
    printf("Parsing string :- %s\n", s);
    printf("Stack top :- %c\n", peek());

    int index=0;
    char a = s[index];
    if(!isTerminal(a)){
        return 0;
    }

    while(1){
        if(peek() == a && a == '$'){
            return 1;
        }
        if(peek() == a){
            pop();
            index++;
            a = s[index];
            if(!isTerminal(a)){
                return 0;
            }
        }
        else{
            if(isNONTerminal(peek())){
                int i = getNTid(peek());
                int j = getTid(a);
                if(table[i][j] == -100){
                    return 0;
                }else{
                    pop();
                    int prod_index = table[i][j];
                    for(int k=strlen(pro.rhs[prod_index])-1;k>=0;k--){
                        if(pro.rhs[prod_index][k] != '#'){
                            push(pro.rhs[prod_index][k]);
                        }
                    }
                }
            }
            else if(peek() == '$' && a!='$'){
                return 0;
            }
        }
        // Printing the stack to know the progress of the parser.
        printStack();
	}
    return 1;
}

//Main function.
int main(int argc, char const *argv[]){
    //Declaring a file pointer.
    FILE *f;
    int i;
    pro.count = 0;
    for(i=0; i<MAXPRODUCTIONS; i++){
        pro.first_cnt[i] = 0;
    }
    //Opening the file.
    f = fopen(argv[1], "r");
    printf("file name : %s\n", argv[1]);
    if(f == NULL){
        printf("Unable to open the file.\n");
        return 0;
    }
    //Reading the contents of the file.
    while (!feof(f))
    {
        //Storing the LHS of the production.
        fscanf(f, "%s", pro.lhs[pro.count]);
        //Storing the RHS of the production.
        fscanf(f, "%s", pro.rhs[pro.count]);
        pro.count++;
    }

    //Printing the grammar.
    printf("\nThe initial Grammar is :-\n\n");
    i=0;
    while (i < pro.count)
    {
        printf("%s -> ", pro.lhs[i]);
        printf("%s \n", pro.rhs[i]);
        i++;
    }

    // Separating NT and T & storing them in different arrays for easy access.
    storeNTandT();
    // Finding FIRST and FOLLOW.
    first_and_follow();
    int isLL1 = TableConstruction();
    // If given grammar is LL(1) the we'll take input string to parse.
    if(isLL1 == 1){
        printf("Type input string without # and $: ");
        scanf("%s",&parsestr);
        printf("\nThe input string :- %s\n", parsestr);
        // Calling the parse string function.
        int flag = ParseString();
        if(flag == 1){
            printf("\nThe string %s is accepted by Parser\n", parsestr);
        }
        else{
            printf("\nThe string %s is NOT accepted by Parser\n", parsestr);
        }
    }

    return 0;
}