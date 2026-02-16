#include <stdio.h>
#include <string.h>

#define MAX 10
#define LEN 50

/* ================= GRAMMAR STORAGE ================= */

char nonTerminals[MAX];
char productions[MAX][MAX][LEN];
int prodCount[MAX];
int n;

/* pool for new non-terminals */
char freshNT = 'Z';

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

        char alpha[MAX][LEN];
        char beta[MAX][LEN];
        int a = 0, b = 0;

        for (int j = 0; j < prodCount[i]; j++) {

            if (productions[i][j][0] == nonTerminals[i])
                strcpy(alpha[a++], productions[i][j] + 1);
            else
                strcpy(beta[b++], productions[i][j]);
        }

        if (a == 0) continue;

        char newNT = freshNT--;
        nonTerminals[n] = newNT;

        /* A → βA' */
        prodCount[i] = 0;
        for (int j = 0; j < b; j++)
            sprintf(productions[i][prodCount[i]++], "%s%c", beta[j], newNT);

        /* A' → αA' | e */
        prodCount[n] = 0;
        for (int j = 0; j < a; j++)
            sprintf(productions[n][prodCount[n]++], "%s%c", alpha[j], newNT);

        strcpy(productions[n][prodCount[n]++], "e");

        n++;
    }
}

/* ================= LONGEST COMMON PREFIX ================= */

int lcpLength(int idx) {
    if (prodCount[idx] < 2) return 0;

    int len = strlen(productions[idx][0]);

    for (int j = 1; j < prodCount[idx]; j++) {
        int k = 0;
        while (k < len &&
               productions[idx][0][k] == productions[idx][j][k])
            k++;
        len = k;
    }

    return len;
}

/* ================= LEFT FACTORING ================= */

void leftFactoring() {
    for (int i = 0; i < n; i++) {

        int lcp = lcpLength(i);
        if (lcp == 0) continue;

        char newNT = freshNT--;
        nonTerminals[n] = newNT;

        char prefix[LEN];
        strncpy(prefix, productions[i][0], lcp);
        prefix[lcp] = '\0';

        /* new productions for A' */
        prodCount[n] = 0;

        for (int j = 0; j < prodCount[i]; j++) {

            if (strlen(productions[i][j]) == lcp)
                strcpy(productions[n][prodCount[n]++], "e");
            else
                strcpy(productions[n][prodCount[n]++],
                       productions[i][j] + lcp);
        }

        /* replace A → prefix A' */
        prodCount[i] = 1;
        sprintf(productions[i][0], "%s%c", prefix, newNT);

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
