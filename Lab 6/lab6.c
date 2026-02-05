//Construction of LL(1) Parsing Table
//based on FIRST and FOLLOW sets.

/* 
Implements:

1. FIRST & FOLLOW usage
2. LL(1) parsing table construction
3. ε-production handling
4. Terminal discovery
5. Table-based predictive parsing structure

*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX 10
#define LEN 50

char nonTerminals[MAX];
char productions[MAX][MAX][LEN];
int prodCount[MAX];
int n;

char FIRST[MAX][MAX];
char FOLLOW[MAX][MAX];
int firstCount[MAX];
int followCount[MAX];

char table[MAX][MAX][LEN];   // LL(1) parsing table
char terminals[MAX];
int tCount = 0;

/* ================= UTILITY ================= */

int contains(char *set, int count, char symbol) {
    for (int i = 0; i < count; i++)
        if (set[i] == symbol) return 1;
    return 0;
}

void addToSet(char *set, int *count, char symbol) {
    if (!contains(set, *count, symbol))
        set[(*count)++] = symbol;
}

/* ================= FIND FIRST ================= */

void findFirst(int i) {
    for (int j = 0; j < prodCount[i]; j++) {

        char *prod = productions[i][j];

        if (!isupper(prod[0])) {
            addToSet(FIRST[i], &firstCount[i], prod[0]);
        }
        else {
            for (int k = 0; prod[k] != '\0'; k++) {

                int index = prod[k] - 'A';
                findFirst(index);

                for (int m = 0; m < firstCount[index]; m++) {
                    if (FIRST[index][m] != 'ε')
                        addToSet(FIRST[i], &firstCount[i], FIRST[index][m]);
                }

                if (!contains(FIRST[index], firstCount[index], 'ε'))
                    break;

                if (prod[k + 1] == '\0')
                    addToSet(FIRST[i], &firstCount[i], 'ε');
            }
        }
    }
}

/* ================= FIND FOLLOW ================= */

void findFollow(int i) {

    if (i == 0)
        addToSet(FOLLOW[i], &followCount[i], '$');

    for (int A = 0; A < n; A++) {
        for (int j = 0; j < prodCount[A]; j++) {

            char *prod = productions[A][j];

            for (int k = 0; prod[k] != '\0'; k++) {

                if (prod[k] == nonTerminals[i]) {

                    if (prod[k + 1] != '\0') {

                        if (!isupper(prod[k + 1])) {
                            addToSet(FOLLOW[i], &followCount[i], prod[k + 1]);
                        }
                        else {
                            int index = prod[k + 1] - 'A';

                            for (int m = 0; m < firstCount[index]; m++) {
                                if (FIRST[index][m] != 'ε')
                                    addToSet(FOLLOW[i], &followCount[i], FIRST[index][m]);
                            }

                            if (contains(FIRST[index], firstCount[index], 'ε'))
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

/* ================= COLLECT TERMINALS ================= */

void collectTerminals() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < prodCount[i]; j++) {
            for (int k = 0; productions[i][j][k]; k++) {

                char c = productions[i][j][k];

                if (!isupper(c) && c != 'ε' && !contains(terminals, tCount, c))
                    terminals[tCount++] = c;
            }
        }
    }

    terminals[tCount++] = '$';
}

/* ================= BUILD LL(1) TABLE ================= */

void buildTable() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < tCount; j++) {
            strcpy(table[i][j], "");
        }
    }

    for (int i = 0; i < n; i++) {

        for (int j = 0; j < prodCount[i]; j++) {

            char *prod = productions[i][j];

            if (!isupper(prod[0])) {
                for (int t = 0; t < tCount; t++) {
                    if (terminals[t] == prod[0])
                        strcpy(table[i][t], prod);
                }
            }
            else {
                int index = prod[0] - 'A';

                for (int m = 0; m < firstCount[index]; m++) {

                    char sym = FIRST[index][m];

                    if (sym != 'ε') {
                        for (int t = 0; t < tCount; t++) {
                            if (terminals[t] == sym)
                                strcpy(table[i][t], prod);
                        }
                    }
                }

                if (contains(FIRST[index], firstCount[index], 'ε')) {
                    for (int m = 0; m < followCount[i]; m++) {
                        for (int t = 0; t < tCount; t++) {
                            if (terminals[t] == FOLLOW[i][m])
                                strcpy(table[i][t], "ε");
                        }
                    }
                }
            }
        }
    }
}

/* ================= PRINT TABLE ================= */

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

/* ================= MAIN ================= */

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

    /* FIRST */
    for (int i = 0; i < n; i++)
        findFirst(i);

    /* FOLLOW */
    for (int i = 0; i < n; i++)
        findFollow(i);

    /* Terminals */
    collectTerminals();

    /* LL(1) Table */
    buildTable();

    /* Output */
    printTable();

    return 0;
}