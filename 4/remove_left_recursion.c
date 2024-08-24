#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Decalring global variables/arrays.
int cnt = 0;

//Defining a structure array for productions.
struct Production
{
    char lhs[10], rhs[20][10];
    int n;

} pro[30];

void concatString(char s[10], char d[10], int p)
{
    // printf("%s %s", s, d);
    char temp[10];
    int l = 0, j;
    while (s[l] != '\0')
    {
        temp[l] = s[l];
        l++;
    }

    for (j = 1; d[j] != '\0'; ++j, ++l)
    {
        temp[l] = d[j];
    }
    temp[l] = '\0';

    strcpy(pro[p].rhs[pro[p].n], temp);
    pro[p].n++;
    return;
}

void transform(int p, int q, int pos)
{
    char temp[10];
    int i, j;
    strcpy(temp, pro[p].rhs[pos]);
    // printf("prod %s\n", temp);
    for (i = pos; i < pro[p].n - 1; i++)
    {
        strcpy(pro[p].rhs[i], pro[p].rhs[i + 1]);
    }
    pro[p].n--;
    for (i = 0; i < pro[q].n; i++)
    {
        concatString(pro[q].rhs[i], temp, p);
    }

    return;
}

void concatStringForDirect(int p, int pos){
    // printf("%s %s\n", pro[p].rhs[pos], pro[p].lhs);
    char temp[10];
    int l = 0, j;
    while (pro[p].rhs[pos][l+1] != '\0')
    {
        temp[l] = pro[p].rhs[pos][l+1];
        l++;
    }
    temp[l++] = pro[p].lhs[0];
    temp[l++] = '\'';
    temp[l] = '\0';
    // printf("%s -- \n",temp);
    strcpy(pro[cnt].rhs[pro[cnt].n], temp);
    pro[cnt].n++;
    // printf("rhs = %s\n", pro[cnt].rhs[pro[cnt].n]);
    return;
}

void removeDirectLeftRecursion(int p)
{
    int i, flag = 0, c=0;
    for(i=0;i<pro[p].n;i++){
        if(pro[p].lhs[0] == pro[p].rhs[i][0]){
            flag = 1;
            // printf("%d\n", p);
            concatStringForDirect(p, i);
            // printf("funtion called");
        }
    }
    if(flag == 1){
        char t[10];
        strcpy(t, pro[p].lhs);
        strcat(t, "'");
        strcpy(pro[cnt].lhs, t);
        cnt++;
        int newc=0;
        for(i=0; i < pro[p].n; i++){
            if(pro[p].lhs[0] != pro[p].rhs[i][0]){
                strcpy(t, pro[p].rhs[i]);
                strcat(t, pro[cnt-1].lhs);
                strcpy(pro[p].rhs[newc], t);
                newc++;
            }
        }
        pro[p].n = newc;
        strcpy(pro[cnt-1].rhs[0], "#");
    }
    
    // printf("cnt = %d",pro[cnt-1].n);
    return;
}

void removeLeftRecursion(int cnt)
{
    int i, j, k;
    for (i = 0; i < cnt; i++)
    {
        for (j = 0; j < i; j++)
        {
            for (k = 0; k < pro[i].n; k++)
            {
                if (pro[j].lhs[0] == pro[i].rhs[k][0])
                {
                    // printf("%s -> %s\n", pro[i].lhs, pro[i].rhs[k]);
                    // printf("%d %d %d\n", i, j, k);
                    transform(i, j, k);
                }
            }
        }
        removeDirectLeftRecursion(i);
    }
    return;
}

//Main function.
int main(int argc, char const *argv[])
{

    //Declaring a file pointer.
    FILE *f;
    int i, j, k, a, b;
    //Initialising the number in productions to 0.
    for (i = 0; i < 30; i++)
        pro[i].n = 0;

    //Opening the file.
    f = fopen(argv[1], "r");
    if(f == NULL){
        printf("Unable to read file...\n");
		return -1;
    }
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
    cnt--;

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

    removeLeftRecursion(cnt);

    printf("\nGrammar after removing left recursion :-\n\n");
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
}