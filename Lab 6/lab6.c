#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX 10
#define LEN 50
#define EPS 'e'

char nonTerminals[MAX];
char productions[MAX][MAX][LEN];
int prodCount[MAX];
int n;

char FIRST[MAX][MAX];
char FOLLOW[MAX][MAX];
int firstCount[MAX];
int followCount[MAX];

char table[MAX][MAX][LEN];
char terminals[MAX];
int tCount = 0;

/* ---------- utility ---------- */

int contains(char *set, int count, char symbol) {
    for (int i = 0; i < count; i++)
        if (set[i] == symbol) return 1;
    return 0;
}

void addToSet(char *set, int *count, char symbol) {
    if (!contains(set, *count, symbol))
        set[(*count)++] = symbol;
}

/* ---------- FIRST ---------- */

void findFirst(int i) {
    for (int j = 0; j < prodCount[i]; j++) {

        char *prod = productions[i][j];

        if (!isupper(prod[0])) {
            addToSet(FIRST[i], &firstCount[i], prod[0]);
        }
        else {
            int k;

            for (k = 0; prod[k]; k++) {

                int index = prod[k] - 'A';
                findFirst(index);

                for (int m = 0; m < firstCount[index]; m++)
                    if (FIRST[index][m] != EPS)
                        addToSet(FIRST[i], &firstCount[i], FIRST[index][m]);

                if (!contains(FIRST[index], firstCount[index], EPS))
                    break;
            }

            if (!prod[k])
                addToSet(FIRST[i], &firstCount[i], EPS);
        }
    }
}

/* ---------- FOLLOW ---------- */

void findFollow(int i) {

    if (i == 0)
        addToSet(FOLLOW[i], &followCount[i], '$');

    for (int A = 0; A < n; A++) {
        for (int j = 0; j < prodCount[A]; j++) {

            char *prod = productions[A][j];

            for (int k = 0; prod[k]; k++) {

                if (prod[k] == nonTerminals[i]) {

                    if (prod[k + 1]) {

                        if (!isupper(prod[k + 1])) {
                            addToSet(FOLLOW[i], &followCount[i], prod[k + 1]);
                        }
                        else {
                            int index = prod[k + 1] - 'A';

                            for (int m = 0; m < firstCount[index]; m++)
                                if (FIRST[index][m] != EPS)
                                    addToSet(FOLLOW[i], &followCount[i], FIRST[index][m]);

                            if (contains(FIRST[index], firstCount[index], EPS))
                                findFollow(A);
                        }
                    }
                    else if (A != i) {
                        findFollow(A);
                    }
                }
            }
        }
    }
}

/* ---------- LL(1) CONDITION CHECK ---------- */

int isLL1() {

    for (int i = 0; i < n; i++) {

        for (int p = 0; p < prodCount[i]; p++) {
            for (int q = p + 1; q < prodCount[i]; q++) {

                char firstP[MAX] = {0}, firstQ[MAX] = {0};
                int cP = 0, cQ = 0;

                /* FIRST of production p */
                if (!isupper(productions[i][p][0]))
                    firstP[cP++] = productions[i][p][0];
                else {
                    int idx = productions[i][p][0] - 'A';
                    for (int k = 0; k < firstCount[idx]; k++)
                        firstP[cP++] = FIRST[idx][k];
                }

                /* FIRST of production q */
                if (!isupper(productions[i][q][0]))
                    firstQ[cQ++] = productions[i][q][0];
                else {
                    int idx = productions[i][q][0] - 'A';
                    for (int k = 0; k < firstCount[idx]; k++)
                        firstQ[cQ++] = FIRST[idx][k];
                }

                /* FIRST intersection check */
                for (int a = 0; a < cP; a++)
                    for (int b = 0; b < cQ; b++)
                        if (firstP[a] == firstQ[b] && firstP[a] != EPS)
                            return 0;

                /* ε condition */
                if (contains(firstP, cP, EPS)) {
                    for (int f = 0; f < followCount[i]; f++)
                        if (contains(firstQ, cQ, FOLLOW[i][f]))
                            return 0;
                }
            }
        }
    }

    return 1;
}

/* ---------- collect terminals ---------- */

void collectTerminals() {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < prodCount[i]; j++)
            for (int k = 0; productions[i][j][k]; k++) {

                char c = productions[i][j][k];

                if (!isupper(c) && c != EPS &&
                    !contains(terminals, tCount, c))
                    terminals[tCount++] = c;
            }

    terminals[tCount++] = '$';
}

/* ---------- build table ---------- */

void buildTable() {

    for (int i = 0; i < n; i++)
        for (int j = 0; j < tCount; j++)
            strcpy(table[i][j], "");

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < prodCount[i]; j++) {

            char *prod = productions[i][j];

            if (!isupper(prod[0])) {
                for (int t = 0; t < tCount; t++)
                    if (terminals[t] == prod[0])
                        strcpy(table[i][t], prod);
            }
            else {
                int index = prod[0] - 'A';

                for (int m = 0; m < firstCount[index]; m++) {

                    char sym = FIRST[index][m];

                    if (sym != EPS)
                        for (int t = 0; t < tCount; t++)
                            if (terminals[t] == sym)
                                strcpy(table[i][t], prod);
                }

                if (contains(FIRST[index], firstCount[index], EPS))
                    for (int m = 0; m < followCount[i]; m++)
                        for (int t = 0; t < tCount; t++)
                            if (terminals[t] == FOLLOW[i][m])
                                strcpy(table[i][t], "e");
            }
        }
    }
}

/* ---------- print ---------- */

void printTable() {

    printf("\nLL(1) Parsing Table:\n\n");

    printf("      ");
    for (int j = 0; j < tCount; j++)
        printf("%6c", terminals[j]);
    printf("\n");

    for (int i = 0; i < n; i++) {

        printf("%4c  ", nonTerminals[i]);

        for (int j = 0; j < tCount; j++) {

            if (strlen(table[i][j]) == 0)
                printf("%6s", "-");
            else
                printf("%6s", table[i][j]);
        }
        printf("\n");
    }
}

/* ---------- main ---------- */

int main() {

    printf("Enter number of non-terminals: ");
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {

        printf("\nEnter Non-terminal %d: ", i + 1);
        scanf(" %c", &nonTerminals[i]);

        printf("Enter number of productions for %c: ", nonTerminals[i]);
        scanf("%d", &prodCount[i]);

        for (int j = 0; j < prodCount[i]; j++) {
            printf("Production %d: ", j + 1);
            scanf("%s", productions[i][j]);
        }
    }

    for (int i = 0; i < n; i++) findFirst(i);
    for (int i = 0; i < n; i++) findFollow(i);

    if (!isLL1()) {
        printf("\nGrammar is NOT LL(1). Parsing table cannot be constructed.\n");
        return 0;
    }

    collectTerminals();
    buildTable();
    printTable();

    return 0;
}
