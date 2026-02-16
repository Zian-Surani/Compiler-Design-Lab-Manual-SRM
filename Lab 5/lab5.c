#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX 10
#define LEN 50
#define EPS 'e'   /* epsilon representation */

char nonTerminals[MAX];
char productions[MAX][MAX][LEN];
int prodCount[MAX];
int n;

char FIRST[MAX][MAX];
char FOLLOW[MAX][MAX];
int firstCount[MAX];
int followCount[MAX];

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

        /* terminal or epsilon */
        if (!isupper(prod[0])) {
            addToSet(FIRST[i], &firstCount[i], prod[0]);
        }
        else {
            int k;

            for (k = 0; prod[k] != '\0'; k++) {

                int index = prod[k] - 'A';
                findFirst(index);

                for (int m = 0; m < firstCount[index]; m++) {
                    if (FIRST[index][m] != EPS)
                        addToSet(FIRST[i], &firstCount[i], FIRST[index][m]);
                }

                if (!contains(FIRST[index], firstCount[index], EPS))
                    break;
            }

            if (prod[k] == '\0')
                addToSet(FIRST[i], &firstCount[i], EPS);
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
                                if (FIRST[index][m] != EPS)
                                    addToSet(FOLLOW[i], &followCount[i], FIRST[index][m]);
                            }

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

/* ================= PRINT SETS ================= */

void printSets() {

    printf("\nFIRST Sets:\n");
    for (int i = 0; i < n; i++) {
        printf("FIRST(%c) = { ", nonTerminals[i]);
        for (int j = 0; j < firstCount[i]; j++)
            printf("%c ", FIRST[i][j]);
        printf("}\n");
    }

    printf("\nFOLLOW Sets:\n");
    for (int i = 0; i < n; i++) {
        printf("FOLLOW(%c) = { ", nonTerminals[i]);
        for (int j = 0; j < followCount[i]; j++)
            printf("%c ", FOLLOW[i][j]);
        printf("}\n");
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

    for (int i = 0; i < n; i++)
        findFirst(i);

    for (int i = 0; i < n; i++)
        findFollow(i);

    printSets();

    return 0;
}
