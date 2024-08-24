/*Constructing the SLR Parsing Table and 
Checking if the given string is accepted by the grammar or not.
*/

//Importing the libraries.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXPRODUCTIONS 30
#define MAXSTATES 30
#define STACKSIZE 100

struct productions{
    char lhs[MAXPRODUCTIONS];
    char rhs[MAXPRODUCTIONS][10];
    int count;
}pro;

struct states{
    struct productions I[MAXSTATES];
    int s_count;
}s;

char state_afterdot[MAXPRODUCTIONS];
char NT[20], T[20], all_T_and_NT[40];
int cnt_nt=0, cnt_t=0, cnt_nt_t=0;
int table[MAXSTATES][40][2];
char First[MAXPRODUCTIONS][10], Follow[MAXPRODUCTIONS][10];
int first_cnt[MAXPRODUCTIONS], follow_cnt[MAXPRODUCTIONS];
char parsestr[100];

// Defining stack and it's functions.
struct stack
{
    int count;
    int top;
    int items[STACKSIZE];
};

struct stack st;

int isempty()
{
    if (st.top == -1)
        return 1;
    else
        return 0;
}

int isfull()
{
    if (st.count == STACKSIZE)
        return 1;
    else
        return 0;
}

int peek()
{
    return st.items[st.top];
}

int pop()
{
    int data;

    if (!isempty())
    {
        data = st.items[st.top];
        st.top--;
        st.count--;
        return data;
    }
    else
    {
        printf("Could not retrieve data, Stack is empty.\n");
    }
}

int push(int data)
{
    if (!isfull())
    {
        st.top++;
        st.items[st.top] = data;
        st.count++;
    }
    else
    {
        printf("Could not insert data, Stack is full.\n");
    }
}

// Stack initialisation function.
void initStack()
{
    st.top = -1;
    st.count = 0;
    push(0);
    return;
}

// Stack printing function.
void printStack(){
    int i;
    printf("\nStack : ");
    for(i=0; i<st.count; i++){
        if(st.items[i]>=0 && st.items[i]<s.s_count){
            printf("%d ", st.items[i]);
        }
        else{
            printf("%c ", st.items[i]);
        }
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
        if(!checkInNT(pro.lhs[i])){
            NT[cnt_nt] = pro.lhs[i];
            cnt_nt++;
        }
        for(j=0; pro.rhs[i][j]!='\0'; j++){
            c = pro.rhs[i][j];
            if(isTerminal(c)){
                if(!checkInT(c) && c!='#'){
                    T[cnt_t]=c;
                    cnt_t++;
                }
            }
        }
    }
    T[cnt_t]='$';
    cnt_t++;
    printf("\nTerminals of the grammar: \n");
    for(i=0;i<cnt_t;i++){
        all_T_and_NT[i] = T[i];
        printf("%c ",T[i]);
    }
    printf("\n");
    printf("\nNon-terminals of the grammar: \n");
    for(i=0;i<cnt_nt;i++){
        all_T_and_NT[i+cnt_t] = NT[i];
        printf("%c ",NT[i]);
    }
    cnt_nt_t = cnt_t + cnt_nt;
    printf("\n\n");
}

// Returns the index value of given T from T array.
int getTid(char c){
    int i;
    for(i=0; i<cnt_t; i++){
        if(T[i]==c){
            return i;
        }
    }
}
 
// Returns the index value of given NT from NT array. 
int getNTid(char c){
    int i;
    for(i=0; i<cnt_nt; i++){
        if(NT[i]==c){
            return i;
        }
    }
}

// Returns ID from array of T and NT
int getID(char c){
    int i;
    for(i=0; i<cnt_nt_t; i++){
        if(c == all_T_and_NT[i]){
            return i;
        }
    }
}

// Checks if the char is in FIRST of given NT
int inFirst(int id, char c){
    int i;
    for(i=0; i<first_cnt[id]; i++){
        if(First[id][i] == c){
            return 1;
        }
    }
    return 0;
}

// Checks if the char is in FOLLOW of given NT
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
    for(i=0; i<pro.count; i++){
        if(pro.lhs[i] == f){
            if(isTerminal(pro.rhs[i][0])){
                First[idf][first_cnt[idf]] = pro.rhs[i][0];
                first_cnt[idf]++;
            }
            else{
                j=0;
                while(j<strlen(pro.rhs[i])){
                    if(isTerminal(pro.rhs[i][j])){
                        if(!inFirst(idf, pro.rhs[i][j])){
                            First[idf][first_cnt[idf]] = pro.rhs[i][j];
                            first_cnt[idf]++;
                        }
                        break;
                    }
                    int id = getNTid(pro.rhs[i][j]);
                    if(first_cnt[id] == 0 && pro.rhs[i][j]!=f){
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
                        for(k=0; k<first_cnt[id]; k++){
                            if(!inFirst(idf, First[id][k]) && First[id][k] != '#'){
                                First[idf][first_cnt[idf]] = First[id][k];
                                first_cnt[idf]++;
                            }
                        }
                    }
                    else{
                        for(k=0; k<first_cnt[id]; k++){
                            if(!inFirst(idf, First[id][k])){
                                First[idf][first_cnt[idf]] = First[id][k];
                                first_cnt[idf]++;
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
                                    flag = 1;
                                }
                            }
                        } 
                    }
                }
                if(j == strlen(pro.rhs[i])-1 || flag == 1){
                    int id = getNTid(pro.lhs[i]);
                    if(follow_cnt[id] == 0){
                        getFollow(pro.lhs[i]);
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
    printf("\nFirst of all Non Terminals:\n");
    for(i=0;i<cnt_nt;i++){
        printf("First of %c = ", NT[i]);
        for(j=0; j<first_cnt[i]; j++){
            printf("%c ", First[i][j]);
        }
        printf("\n");
    }
    printf("\nFollow of all Non Terminals:\n");
    for(i=0;i<cnt_nt;i++){
        printf("Follow of %c = ", NT[i]);
        for(j=0; j<follow_cnt[i]; j++){
            printf("%c ", Follow[i][j]);
        }
        printf("\n");
    }
}

int inafterdotstatelist(char c){
    int i;
    for(i=0; i<MAXPRODUCTIONS; i++){
        if(state_afterdot[i] == c){
            return 1;
        }
    }
    return 0;
}

// Function to print the table.
void printTable(){
    int i,j;
    printf("\nSLR Parsing Table:\n\n");
    printf("\t");
    for(i=0; i<cnt_nt_t; i++){
        printf("%c\t", all_T_and_NT[i]);
    }
    printf("\n");
    for(i=0; i<s.s_count; i++){
        printf("%d\t", i);
        for(j=0; j<cnt_nt_t; j++){
            if(table[i][j][0] == -100){
                printf("E\t");
            }
            else{
                // Shift when 1.
                if(table[i][j][0] == 1){
                    printf("s%d\t", table[i][j][1]);
                }
                // Reduce when 2
                else if(table[i][j][0] == 2){
                    printf("r%d\t", table[i][j][1]);
                }
                // NT operation when 3
                else if(table[i][j][0] == 3){
                    printf("%d\t", table[i][j][1]);
                }
                // Accept when 4
                else if(table[i][j][0] == 4){
                    printf("acc\t");
                }
            }
            
        }
        printf("\n");
    }
}

int isAlreadyaState(){
    int i,j,k, pt=0;
    int is = s.s_count, ip = s.I[is].count;
    for(i=0; i<is-1; i++){
        pt=0;
        for(j=0; j<ip; j++){
            if(s.I[i].lhs[j] == s.I[is].lhs[j] && strcmp(s.I[i].rhs[j],s.I[is].rhs[j]) == 0){
                pt++;
                // printf("\t%c->%s %d\n", s.I[i].lhs[j], s.I[i].rhs[j], pt);
            }
        }
        if(pt == ip){
            return i;
        }
    }
    return 0;
}

void moreproduction(){
    int l = 0, i, j;
    int cnt_read=0;
    int is = s.s_count, ip = s.I[is].count;
    char read[MAXPRODUCTIONS], temp;
    for(i=0; i<s.I[is].count; i++){
        for(j=0; j<strlen(s.I[is].rhs[i])-1; j++){
            if(s.I[is].rhs[i][j] == '.' && isNONTerminal(s.I[is].rhs[i][j+1])){
                temp = s.I[is].rhs[l][j+1];
                l++;
            }
        }
        for (j = 0; j < cnt_read; j++){
            if (temp == read[j]){
                break;
            }
        }
        if (j == cnt_read){
            read[cnt_read] = temp;
            cnt_read++;
        }
        else{
            continue;
        }

        for (j = 0; j < pro.count; j++)
        {
            if (pro.lhs[j] == temp)
            {
                s.I[is].rhs[s.I[is].count][0] = '.';
                s.I[is].rhs[s.I[is].count][1] = '\0';
                strcat(s.I[is].rhs[s.I[is].count], pro.rhs[j]);
                s.I[is].lhs[s.I[is].count] = pro.lhs[j];
                s.I[is].count++;
            }
        }
    }
    s.s_count++;
}

void canonical(int l)
{
    int i,j,k,m, state_cnt = 0;
    for(i=0; i<MAXPRODUCTIONS; i++){
        state_afterdot[i] = '.';
    }
    for(i=0; i<s.I[l].count; i++){
        for(j=0; j<strlen(s.I[l].rhs[i]); j++){
            if(s.I[l].rhs[i][j] == '.'){
                if(j<strlen(s.I[l].rhs[i])-1 && !inafterdotstatelist(s.I[l].rhs[i][j+1])){
                    state_afterdot[state_cnt] = s.I[l].rhs[i][j+1];
                    state_cnt++;
                }
            }
        }
    }

    for(k=0; k<state_cnt; k++){
        int is = s.s_count, flag=0;
        for(i=0; i<s.I[l].count; i++){
            for(j=0; j<strlen(s.I[l].rhs[i])-1; j++){
                if(s.I[l].rhs[i][j] == '.' && s.I[l].rhs[i][j+1]==state_afterdot[k]){
                    int ip = s.I[is].count;
                    s.I[is].lhs[ip] = s.I[l].lhs[i];
                    for(m=0; m<j; m++){
                        s.I[is].rhs[ip][m] = s.I[l].rhs[i][m];
                    }
                    s.I[is].rhs[ip][m] = s.I[l].rhs[i][m+1];
                    s.I[is].rhs[ip][m+1] = '.';

                    for(m=j+2; m<strlen(s.I[l].rhs[i]); m++){
                        s.I[is].rhs[ip][m] = s.I[l].rhs[i][m];
                    }
                    s.I[is].rhs[ip][m] = '\0';
                    s.I[is].count++;
                    flag = 1;
                    break;
                }
            }
        }
        if(flag == 1){
            int state_no = isAlreadyaState();
            if(state_no > 0){
                for(j=0; j<s.I[is].count; j++){
                    s.I[is].lhs[j] = '\0';
                    s.I[is].rhs[j][0] = '\0';
                }
                s.I[is].count=0;
                
                if(table[l][getID(state_afterdot[k])][0] == -100){
                    if(isTerminal(state_afterdot[k])){
                        table[l][getID(state_afterdot[k])][0] = 1;
                    }
                    else{
                        table[l][getID(state_afterdot[k])][0] = 3;
                    }
                    table[l][getID(state_afterdot[k])][1] = state_no;
                }
            }
            else{
                moreproduction();
                if(table[l][getID(state_afterdot[k])][0] == -100){
                    if(isTerminal(state_afterdot[k])){
                        table[l][getID(state_afterdot[k])][0] = 1;
                    }
                    else{
                        table[l][getID(state_afterdot[k])][0] = 3;
                    }
                    table[l][getID(state_afterdot[k])][1] = is;
                }
            }
        }
    }
}

int addReduceinTable(){
    int i,j,k,x;
    for(i=0; i<s.s_count; i++){
        for(j=0; j<s.I[i].count; j++){
            int l = strlen(s.I[i].rhs[j]);
            if(s.I[i].rhs[j][l-1] == '.'){
                if(i == 1){
                    if(table[i][getID('$')][0] == -100){
                        table[i][getID('$')][0] = 4;
                    }
                    else{
                        return 0;
                    }
                }
                else if(strcmp(s.I[i].rhs[j], s.I[1].rhs[0]) == 0){
                    if(table[i][getID('$')][0] == -100){
                        table[i][getID('$')][0] = 4;
                    }
                    else{
                        return 0;
                    }
                }
                else{
                    char lhs = s.I[i].lhs[j];
                    int nt_id = getNTid(lhs);
                    for(k=0; k<follow_cnt[nt_id]; k++){
                        if(table[i][getID(Follow[nt_id][k])][0] == -100){
                            table[i][getID(Follow[nt_id][k])][0] = 2;
                            char sample[100];
                            strcpy(sample, s.I[i].rhs[j]);
                            sample[strlen(sample)-1] = '\0';
                            for(x=0; x<pro.count; x++){
                                if(lhs == pro.lhs[x] && strcmp(sample, pro.rhs[x]) == 0){
                                    table[i][getID(Follow[nt_id][k])][1] = x;
                                }
                            }
                        }
                        else{
                            return 0;
                        }
                    }
                }
            }
        }
    }
    return 1;
}

// Function that parses the given string input.
int ParseString(){
    initStack();
    int i;
    char s[100], x[3] = "$\0";
    strcpy(s, parsestr);
    strcat(s, x);
    printf("Parsing string :- %s\n", s);
    printf("Stack top :- %d \n", peek());

    int index=0;
    char a = s[index];
    if(!checkInT(a)){
        return 0;
    }
    printStack();
    while(index < strlen(s)){
        if(peek()==1 && a=='$'){
            // printf("true");
            return 1;
        }
        int id = getID(a);
        int op = table[peek()][id][0];
        int pro_no = table[peek()][id][1];
        // printf("id = %d op = %d pro_no = %d\n", id, op, pro_no);
        if(op == 1){
            push(a);
            push(pro_no);
            index++;
            a = s[index];
            if(!checkInT(a)){
                return 0;
            }
        }
        else if(op == 2){
            pop();
            for(i=strlen(pro.rhs[pro_no])-1; i>=0; i--){
                if(pro.rhs[pro_no][i] == (char)peek()){
                    pop();
                }
                if(i!=0){
                    pop();
                }
            }
            int s_id = peek();
            push(pro.lhs[pro_no]);
            if(table[s_id][getID(pro.lhs[pro_no])][0] == 3){
                push(table[s_id][getID(pro.lhs[pro_no])][1]);
            }
            else{
                return 0;
            }
        }
        else if(op == 4){
            return 1;
        }
        else if(op == -100){
            return 0;
        }
        printStack();
    }

}

//Main function.
int main(int argc, char const *argv[]){
    //Declaring a file pointer.
    FILE *f;
    int i,j,l;
    char t[10], temp;
    pro.count = 0;
    s.s_count = 0;
    for (i = 0; i < MAXSTATES; i++){
        s.I[i].count = 0;
    }
    for(i=0; i<MAXSTATES; i++){
        for(j=0; j<40; j++){
            table[i][j][0] = -100;
            table[i][j][1] = -100;
        }
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
        fscanf(f, "%s", t);
        pro.lhs[pro.count] = t[0];
        //Storing the RHS of the production.
        fscanf(f, "%s", pro.rhs[pro.count]);
        pro.count++;
    }

    //Printing the grammar.
    printf("\nThe initial Grammar is :-\n\n");
    i=0;
    while (i < pro.count)
    {
        printf("%c -> ", pro.lhs[i]);
        printf("%s \n", pro.rhs[i]);
        i++;
    }

    storeNTandT();

    s.I[0].lhs[0] = 'Z';
    t[0] = '.';
    t[1] = pro.lhs[0];
    t[2] = '\0';
    strcpy(s.I[0].rhs[0], t);
    s.I[0].count++;
    l = 0;
    int cnt_read=0;
    char read[MAXPRODUCTIONS];
    for (i = 0; i < pro.count; i++){
        temp = s.I[0].rhs[l][1];
        l++;
        for (j = 0; j < cnt_read; j++){
            if (temp == read[j]){
                break;
            }
        }
        if (j == cnt_read){
            read[cnt_read] = temp;
            cnt_read++;
        }
        else{
            continue;
        }
        
        for (j = 0; j < pro.count; j++)
        {
            if (pro.lhs[j] == temp)
            {
                s.I[0].rhs[s.I[0].count][0] = '.';
                strcat(s.I[0].rhs[s.I[0].count], pro.rhs[j]);
                s.I[0].lhs[s.I[0].count] = pro.lhs[j];
                s.I[0].count++;
            }
        }
    }
    s.s_count++;

    for(i = 0; i<s.s_count; i++){
        canonical(i);
    }
    first_and_follow();

    printf("\nAll states:\n");
    for(i=0; i<s.s_count; i++){
        printf("I%d :-\n", i);
        for(j=0; j<s.I[i].count; j++){
            printf("%c -> %s\n", s.I[i].lhs[j], s.I[i].rhs[j]);
        }
        printf("\n");
    }

    int isSLR = addReduceinTable();
    if(isSLR){
        printTable();
        printf("The given grammar is in SLR.\n\n");
        printf("Type input string without # and $: ");
        scanf("%s",&parsestr);
        printf("\nThe input string :- %s\n", parsestr);
        // Calling the parse string function.
        int flag = ParseString();
        if(flag){
            printf("\nThe string %s is accepted by Parser\n", parsestr);
        }
        else{
            printf("\nThe string %s is NOT accepted by Parser\n", parsestr);
        }
    }
    else{
        printf("The grammar is not in SLR\n");
    }

    return 0;
}