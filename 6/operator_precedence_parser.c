/*Constructing the Operactor Precedence Parsing Table and 
Checking if the given string is accepted by the grammar or not.
Running the program
gcc operator_precedence_parser.c
./a.out <input_file> <input_string> (for linux)
./a.exe <input_file> <input_string> (for windows)
*/

//Importing the libraries.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXPRODUCTIONS 30
#define MAXTERMINALS 20
#define STACKSIZE 100

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

// Decalring global variables/arrays.
int cnt = 0, cnt_t = 0;
int l = 1, t = 2;
char T[MAXTERMINALS];
// Parsing table.
char table[MAXTERMINALS][MAXTERMINALS];
char parseStr[100];

//Defining a structure array for productions.
struct Production
{
    char lhs[10], rhs[20][10];
    int n;

} pro[MAXPRODUCTIONS];

struct Leading{
    char leading_of_a[10];
    int n;
} lead[MAXPRODUCTIONS];

struct Trailing{
    char trailing_of_a[10];
    int n;
} trail[MAXPRODUCTIONS];

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

int checkaInLeadAndTrail(char a, int k, int flag){
    int i;
    if(flag == 1){
        for(i=0; i<lead[k].n; i++){
            if(lead[k].leading_of_a[i] == a){
                return 1;
            }
        }
        return 0;
    }
    else if(flag == 2){
        for(i=0; i<trail[k].n; i++){
            if(trail[k].trailing_of_a[i] == a){
                return 1;
            }
        }
        return 0;
    }
}

// Adding a to leading for the productions of the form A->Bx
void addLeadAtBa(char B, char a){
    int i, j;
    for (i = 0; i < cnt; i++)
    {
        for (j = 0; j < pro[i].n; j++)
        {
            if(strlen(pro[i].rhs[j])>0 && pro[i].rhs[j][0] == B){
                if(!checkaInLeadAndTrail(a, i, l)){
                    // printf("Leading %c %c\n", pro[i].lhs[0], a);
                    lead[i].leading_of_a[lead[i].n] = a;
                    lead[i].n++;
                    addLeadAtBa(pro[i].lhs[0], a);
                }
            }
        }
    }
    return;
}

// Adding a to trailing for the productions of the form A->xB
void addTrailataB(char B, char a){
    int i, j;
    for (i = 0; i < cnt; i++)
    {
        for (j = 0; j < pro[i].n; j++)
        {
            int len = strlen(pro[i].rhs[j]);
            if(strlen(pro[i].rhs[j])>0 && pro[i].rhs[j][len-1] == B){
                if(!checkaInLeadAndTrail(a, i, t)){
                    // printf("Trailing %c %c\n", pro[i].lhs[0], a);
                    trail[i].trailing_of_a[trail[i].n] = a;
                    trail[i].n++;
                    addTrailataB(pro[i].lhs[0], a);
                }
            }
        }
    }
    return;
}

// Function to find Leading of all Non-terminals of the grammar.
void findLeading(){
    int i,j;
    for (i = 0; i < cnt; i++)
    {
        for (j = 0; j < pro[i].n; j++)
        {
            // First step of finding Leading.
            if(strlen(pro[i].rhs[j])>0 && isTerminal(pro[i].rhs[j][0])){
                // printf("leading %c\n", pro[i].rhs[j][0]);
                lead[i].leading_of_a[lead[i].n] = pro[i].rhs[j][0];
                lead[i].n++;
                // Adding a to leading for the productions of the form A->Bx
                addLeadAtBa(pro[i].lhs[0], pro[i].rhs[j][0]);
            }
            else if(strlen(pro[i].rhs[j])>1 && isNONTerminal(pro[i].rhs[j][0]) && isTerminal(pro[i].rhs[j][1])){
                // printf("leading %c\n", pro[i].rhs[j][1]);
                lead[i].leading_of_a[lead[i].n] = pro[i].rhs[j][1];
                lead[i].n++;
                // Adding a to leading for the productions of the form A->Bx
                addLeadAtBa(pro[i].lhs[0], pro[i].rhs[j][1]);
            }
        }
    }
    printf("\n");
    for (i = 0; i < cnt; i++)
    {
        printf("Leading of %s -> ", pro[i].lhs);
        for (j = 0; j < lead[i].n; j++)
        {
            printf(" %c ", lead[i].leading_of_a[j]);
        }
        printf("\n");
    }
    return;
}

// Function to find Trailing of all Non-terminals of the grammar.
void findTrailing(){
    int i,j;
    for (i = 0; i < cnt; i++)
    {
        for (j = 0; j < pro[i].n; j++)
        {
            int len = strlen(pro[i].rhs[j]);
            if(len>0 && isTerminal(pro[i].rhs[j][len-1])){
                // printf("trailing %c\n", pro[i].rhs[j][len-1]);
                trail[i].trailing_of_a[trail[i].n] = pro[i].rhs[j][len-1];
                trail[i].n++;
                addTrailataB(pro[i].lhs[0], pro[i].rhs[j][len-1]);
            }
            else if(len>1 && isNONTerminal(pro[i].rhs[j][len-1]) && isTerminal(pro[i].rhs[j][len-2])){
                // printf("trailing %c\n", pro[i].rhs[j][len-2]);
                trail[i].trailing_of_a[trail[i].n] = pro[i].rhs[j][len-2];
                trail[i].n++;
                addTrailataB(pro[i].lhs[0], pro[i].rhs[j][len-2]);
            }
        }
    }
    printf("\n");
    for (i = 0; i < cnt; i++)
    {
        printf("Trailing of %s -> ", pro[i].lhs);
        for (j = 0; j < trail[i].n; j++)
        {
            printf(" %c ", trail[i].trailing_of_a[j]);
        }
        printf("\n");
    }
    return;
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

// Function to get all Terminals and store it in an array.
void storeT(){
    int i,j,k;
    char c;
    for(i=0; i<cnt; i++){
        for(j=0; j<pro[i].n; j++){
            for(k=0; pro[i].rhs[j][k]!='\0'; k++){
                c = pro[i].rhs[j][k];
                if(isTerminal(c)){
                    if(!checkInT(c) && c!='#'){
                        T[cnt_t]=c;
                        cnt_t++;
                    }
                }
            }
        }
    }
    T[cnt_t]='$';
    cnt_t++;
    printf("\n");
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
    for(i=0; i<cnt; i++){
        if(pro[i].lhs[0]==c){
            return i;
        }
    }
}

void printTable(){
    int i,j;
    printf("Operator Precedence Parsing Table: \n");
    printf("   ");
    for(i=0; i<cnt_t; i++){
        printf("%c  ", T[i]);
    }
    printf("\n");
    for(i=0; i<cnt_t; i++){
        printf("%c  ", T[i]);
        for(j = 0; j<cnt_t; j++){
            printf("%c  ", table[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Constructing Operator precedence parsing table.
void constructParseTable(){
    // Step 1: Finding Leading and trailing of NT in cfg.
    findLeading();
    findTrailing();
    // Step 2:
    storeT();
    int i, j, k, x;
    for (i = 0; i < cnt; i++)
    {
        for (j = 0; j < pro[i].n; j++)
        {
            int len = strlen(pro[i].rhs[j]);
            for(k=0; k<len-1; k++){
                if(isTerminal(pro[i].rhs[j][k]) && isTerminal(pro[i].rhs[j][k+1])){
                    // printf("2 T sidebyside %c %c\n", pro[i].rhs[j][k],pro[i].rhs[j][k+1]);
                    table[getTid(pro[i].rhs[j][k])][getTid(pro[i].rhs[j][k+1])] = '=';
                }
                if(k<len-2 && isTerminal(pro[i].rhs[j][k]) && isTerminal(pro[i].rhs[j][k+2]) && isNONTerminal(pro[i].rhs[j][k+1])){
                    // printf("2 T with NT at center %c %c %c\n", pro[i].rhs[j][k],pro[i].rhs[j][k+1],pro[i].rhs[j][k+2], get);
                    table[getTid(pro[i].rhs[j][k])][getTid(pro[i].rhs[j][k+2])] = '=';
                }
                if(isTerminal(pro[i].rhs[j][k]) && isNONTerminal(pro[i].rhs[j][k+1])){
                    int id = getNTid(pro[i].rhs[j][k+1]);
                    for(x=0; x<lead[id].n; x++){
                        table[getTid(pro[i].rhs[j][k])][getTid(lead[id].leading_of_a[x])] = '<';
                    }
                }
                if(isNONTerminal(pro[i].rhs[j][k]) && isTerminal(pro[i].rhs[j][k+1])){
                    int id = getNTid(pro[i].rhs[j][k]);
                    for(x=0; x<trail[id].n; x++){
                        table[getTid(trail[id].trailing_of_a[x])][getTid(pro[i].rhs[j][k+1])] = '>';
                    }
                }
            }
        }
    }
    // Step 3:
    for(i = 0; i<lead[0].n; i++){
        table[cnt_t-1][getTid(lead[0].leading_of_a[i])] = '<';
    }
    for(i = 0; i<trail[0].n; i++){
        table[getTid(trail[0].trailing_of_a[i])][cnt_t-1] = '>';
    }
    for(i=0; i<cnt_t; i++){
        for(j = 0; j<cnt_t; j++){
            if(table[i][j] != '=' && table[i][j] != '<' && table[i][j] != '>'){
                table[i][j] = 'E';
            }
        }
    }
    printTable();
}

int parseString(){
    initStack();
    char s[100], x[3] = "$\0";
    strcpy(s, parseStr);
    strcat(s, x);
    printf("Parsing string = %s\n", s);
    printf("Stack top = %c\n", peek());
    int i = 0;
    while(1){
        if(peek()=='$' && s[i]=='$'){
            return 1;
        }
        else{
            char a = peek();
            char b = s[i];
            int ida = getTid(a);
            int idb = getTid(b);
            if(table[ida][idb] == '<' || table[ida][idb] == '='){
                push(s[i]);
                i++;
                printStack();
            }
            else if(table[ida][idb] == '>'){
                while(table[ida][idb] != '<' && table[ida][idb] != '=' && peek()!='$'){
                    pop();
                    a = peek();
                    ida = getTid(a);
                    printStack();
                }
            }
            else{
                return 0;
            }
        }
    }
}

//Main function.
int main(int argc, char const *argv[])
{
    //Declaring a file pointer.
    FILE *f;
    int i, j, k, a, b;
    //Initialising the number in productions to 0.
    for (i = 0; i < MAXPRODUCTIONS; i++){
        pro[i].n = 0;
    }
    for (i = 0; i < MAXPRODUCTIONS; i++){
        lead[i].n = 0;
    }
    for (i = 0; i < MAXPRODUCTIONS; i++){
        trail[i].n = 0;
    }

    //Opening the file.
    f = fopen(argv[1], "r");
    //Reading the contents of the file.
    while (!feof(f))
    {
        //Storing the LHS of the production.
        fscanf(f, "%s", pro[cnt].lhs);
        if (cnt > 0)
        {
            if (strcmp(pro[cnt].lhs, pro[cnt - 1].lhs) == 0)
            {
                pro[cnt].lhs[0] = '\0';
                fscanf(f, "%s", pro[cnt - 1].rhs[pro[cnt - 1].n]);
                pro[cnt - 1].n++;
                continue;
            }
        }
        //Storing the RHS of the production.
        fscanf(f, "%s", pro[cnt].rhs[pro[cnt].n]);
        pro[cnt].n++;
        cnt++;
    }
    fclose(f);
    cnt--;

    strcpy(parseStr, argv[2]);

    //Printing the grammar.
    printf("\nThe initial Grammar is :-\n\n");
    for (i = 0; i < cnt; i++)
    {
        printf("%s -> ", pro[i].lhs);
        for (j = 0; j < pro[i].n; j++)
        {
            printf(" %s ", pro[i].rhs[j]);
            if (j != pro[i].n - 1)
                printf("|");
        }
        printf("\n");
    }

    constructParseTable();
    int check = parseString();
    printf("\n");
    if(check){
        printf("%s is accepted by given grammar.\n", parseStr);
    }
    else{
        printf("%s is NOT accepted by given grammar.\n", parseStr);
    }
}