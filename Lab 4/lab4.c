//Elimination of Ambiguity, Left Recursion and Left Factoring
/*      

        ✔ Elimination of Left Recursion

    Implements:

    1. Immediate left recursion
    2. Creates A′ non-terminal
    3. Adds ε production

        ✔ Left Factoring

    Detects: 
    1. Common prefix
    2. Introduces new factored non-terminal

        ✔ Ambiguity handling*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX 10
#define LEN 50

/* ================= GRAMMAR STORAGE ================= */

char nonTerminals[MAX];
char productions[MAX][MAX][LEN];   // productions[A][i] = ith rule of A
int prodCount[MAX];
int n;

/* ================= PRINT GRAMMAR ================= */

void printGrammar(char title[]) {
    printf("\n=== %s ===\n", title);

    for (int i = 0; i < n; i++) {
        printf("%c -> ", nonTerminals[i]);

        for (int j = 0; j < prodCount[i]; j++) {
            printf("%s", productions[i][j]);
            if (j != prodCount[i] - 1) printf(" | ");
        }
        printf("\n");
    }
}

/* ================= LEFT RECURSION REMOVAL ================= */

void removeLeftRecursion() {
    for (int i = 0; i < n; i++) {

        char alpha[MAX][LEN];  // recursive parts
        char beta[MAX][LEN];   // non-recursive parts
        int a = 0, b = 0;

        for (int j = 0; j < prodCount[i]; j++) {

            if (productions[i][j][0] == nonTerminals[i]) {
                // A → Aα  → store α
                strcpy(alpha[a++], productions[i][j] + 1);
            } else {
                // A → β
                strcpy(beta[b++], productions[i][j]);
            }
        }

        if (a == 0) continue; // no left recursion

        // create new non-terminal A'
        char newNT = nonTerminals[i] + '\'';
        nonTerminals[n] = newNT;

        // update A → βA'
        prodCount[i] = 0;
        for (int j = 0; j < b; j++) {
            sprintf(productions[i][prodCount[i]++], "%s%c", beta[j], newNT);
        }

        // create A' → αA' | ε
        prodCount[n] = 0;
        for (int j = 0; j < a; j++) {
            sprintf(productions[n][prodCount[n]++], "%s%c", alpha[j], newNT);
        }
        strcpy(productions[n][prodCount[n]++], "ε");

        n++;
    }
}

/* ================= LEFT FACTORING ================= */

void leftFactoring() {
    for (int i = 0; i < n; i++) {

        if (prodCount[i] < 2) continue;

        char prefix = productions[i][0][0];
        int same = 1;

        for (int j = 1; j < prodCount[i]; j++) {
            if (productions[i][j][0] != prefix) {
                same = 0;
                break;
            }
        }

        if (!same) continue;

        // create new non-terminal A'
        char newNT = nonTerminals[i] + '\'';
        nonTerminals[n] = newNT;

        // A → prefix A'
        char temp[LEN];
        sprintf(temp, "%c%c", prefix, newNT);

        prodCount[n] = 0;

        // A' → remaining suffixes
        for (int j = 0; j < prodCount[i]; j++) {
            if (strlen(productions[i][j]) == 1)
                strcpy(productions[n][prodCount[n]++], "ε");
            else
                strcpy(productions[n][prodCount[n]++], productions[i][j] + 1);
        }

        // replace A productions
        prodCount[i] = 1;
        strcpy(productions[i][0], temp);

        n++;
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

    printGrammar("Original Grammar");

    removeLeftRecursion();
    printGrammar("After Removing Left Recursion");

    leftFactoring();
    printGrammar("After Left Factoring");

    return 0;
}
