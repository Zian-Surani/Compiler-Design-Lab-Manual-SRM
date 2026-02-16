/*
Example I/O

Enter number of productions: 3
Enter productions (format A->BC):
S->CC
C->cC
C->d

OUTPUT (abridged)

Canonical LR(1) ACTION/GOTO TABLE

State 0:
  ACTION[c] = s3
  ACTION[d] = s4
  GOTO[C] = 2
  GOTO[S] = 1
...

No conflicts → Grammar is LR(1)
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX 20
#define MAX_ITEMS 100
#define MAX_PROD 20
#define MAX_LEN 20

/* ---------- production ---------- */

char prodLHS[MAX_PROD];
char prodRHS[MAX_PROD][MAX_LEN];
int prodCount;

/* ---------- LR(1) item ---------- */

typedef struct {
    int p;
    int dot;
    char la;   /* lookahead */
} Item;

typedef struct {
    Item items[MAX_ITEMS];
    int count;
} State;

State states[MAX];
int stateCount = 0;

/* ---------- utility ---------- */

int sameItem(Item a, Item b) {
    return a.p == b.p && a.dot == b.dot && a.la == b.la;
}

int containsItem(State *s, Item x) {
    for (int i = 0; i < s->count; i++)
        if (sameItem(s->items[i], x)) return 1;
    return 0;
}

/* ---------- FIRST of single symbol ---------- */

char firstOf(char X) {
    if (!isupper(X)) return X;
    return prodRHS[0][0];
}

/* ---------- closure LR(1) ---------- */

void closure(State *s) {

    for (int i = 0; i < s->count; i++) {

        Item it = s->items[i];
        char B = prodRHS[it.p][it.dot];

        if (!isupper(B)) continue;

        char beta = prodRHS[it.p][it.dot + 1];
        char look = beta ? firstOf(beta) : it.la;

        for (int p = 0; p < prodCount; p++) {
            if (prodLHS[p] == B) {

                Item newItem = {p, 0, look};

                if (!containsItem(s, newItem))
                    s->items[s->count++] = newItem;
            }
        }
    }
}

/* ---------- GOTO ---------- */

void gotoState(State *src, State *dst, char X) {

    dst->count = 0;

    for (int i = 0; i < src->count; i++) {

        Item it = src->items[i];

        if (prodRHS[it.p][it.dot] == X)
            dst->items[dst->count++] = (Item){it.p, it.dot + 1, it.la};
    }

    closure(dst);
}

/* ---------- state equality ---------- */

int sameState(State *a, State *b) {

    if (a->count != b->count) return 0;

    for (int i = 0; i < a->count; i++) {

        int found = 0;

        for (int j = 0; j < b->count; j++)
            if (sameItem(a->items[i], b->items[j]))
                found = 1;

        if (!found) return 0;
    }

    return 1;
}

/* ---------- canonical LR(1) collection ---------- */

void buildStates() {

    states[0].count = 1;
    states[0].items[0] = (Item){0, 0, '$'};
    closure(&states[0]);

    stateCount = 1;

    for (int i = 0; i < stateCount; i++) {

        for (char X = 0; X < 127; X++) {

            if (!isalpha(X)) continue;

            State temp;
            gotoState(&states[i], &temp, X);

            if (temp.count == 0) continue;

            int found = -1;

            for (int j = 0; j < stateCount; j++)
                if (sameState(&states[j], &temp))
                    found = j;

            if (found == -1)
                states[stateCount++] = temp;
        }
    }
}

/* ---------- ACTION/GOTO ---------- */

char ACTION[MAX][128][MAX_LEN];
int GOTO[MAX][128];
int conflict = 0;

void buildTable() {

    for (int i = 0; i < stateCount; i++) {

        for (int j = 0; j < states[i].count; j++) {

            Item it = states[i].items[j];
            char sym = prodRHS[it.p][it.dot];

            if (islower(sym)) {

                State temp;
                gotoState(&states[i], &temp, sym);

                for (int k = 0; k < stateCount; k++)
                    if (sameState(&states[k], &temp)) {

                        if (strlen(ACTION[i][sym]))
                            conflict = 1;

                        sprintf(ACTION[i][sym], "s%d", k);
                    }
            }
            else if (sym == '\0') {

                if (it.p == 0) {
                    strcpy(ACTION[i]['$'], "acc");
                }
                else {

                    if (strlen(ACTION[i][it.la]))
                        conflict = 1;

                    sprintf(ACTION[i][it.la], "r%d", it.p);
                }
            }
        }

        for (char X = 'A'; X <= 'Z'; X++) {

            State temp;
            gotoState(&states[i], &temp, X);

            for (int k = 0; k < stateCount; k++)
                if (sameState(&states[k], &temp))
                    GOTO[i][X] = k;
        }
    }
}

/* ---------- print ---------- */

void printTable() {

    printf("\nCanonical LR(1) ACTION/GOTO TABLE\n\n");

    for (int i = 0; i < stateCount; i++) {

        printf("State %d:\n", i);

        for (char t = 0; t < 127; t++)
            if (strlen(ACTION[i][t]))
                printf("  ACTION[%c] = %s\n", t, ACTION[i][t]);

        for (char nt = 'A'; nt <= 'Z'; nt++)
            if (GOTO[i][nt])
                printf("  GOTO[%c] = %d\n", nt, GOTO[i][nt]);

        printf("\n");
    }

    if (conflict)
        printf("Grammar is NOT LR(1) (conflict detected).\n");
    else
        printf("Grammar is LR(1).\n");
}

/* ---------- main ---------- */

int main() {

    printf("Enter number of productions: ");
    if (scanf("%d", &prodCount) != 1 || prodCount <= 0)
        return 1;

    printf("Enter productions (A->BC form):\n");

    for (int i = 0; i < prodCount; i++) {

        char buf[50];
        scanf("%s", buf);

        prodLHS[i] = buf[0];
        strcpy(prodRHS[i], buf + 3);
    }

    buildStates();
    buildTable();
    printTable();

    return 0;
}
