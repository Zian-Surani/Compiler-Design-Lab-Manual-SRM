/*
Example I/O

Enter number of productions: 3
Enter productions (format A->BC):
S->CC
C->cC
C->d

OUTPUT (abridged)

SLR(1) ACTION/GOTO TABLE

State   c   d   $   |   C   S
--------------------------------
0       s3  s4      |   2   1
1           acc     |
2       s3  s4      |   5
3       s3  s4      |   6
4       r3  r3  r3
...

No conflicts → Grammar is SLR(1)
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX 20
#define MAX_ITEMS 50
#define MAX_PROD 20
#define MAX_LEN 20

/* ---------- production ---------- */

char prodLHS[MAX_PROD];
char prodRHS[MAX_PROD][MAX_LEN];
int prodCount;

/* ---------- FOLLOW ---------- */

char FOLLOW[MAX][MAX];
int followCount[MAX];

/* ---------- LR(0) item ---------- */

typedef struct {
    int p;      /* production index */
    int dot;    /* dot position */
} Item;

typedef struct {
    Item items[MAX_ITEMS];
    int count;
} State;

State states[MAX];
int stateCount = 0;

/* ---------- utility ---------- */

int sameItem(Item a, Item b) {
    return a.p == b.p && a.dot == b.dot;
}

int containsItem(State *s, Item x) {
    for (int i = 0; i < s->count; i++)
        if (sameItem(s->items[i], x)) return 1;
    return 0;
}

/* ---------- closure ---------- */

void closure(State *s) {

    for (int i = 0; i < s->count; i++) {

        Item it = s->items[i];
        char sym = prodRHS[it.p][it.dot];

        if (isupper(sym)) {

            for (int p = 0; p < prodCount; p++) {
                if (prodLHS[p] == sym) {

                    Item newItem = {p, 0};

                    if (!containsItem(s, newItem))
                        s->items[s->count++] = newItem;
                }
            }
        }
    }
}

/* ---------- GOTO ---------- */

void gotoState(State *src, State *dst, char X) {

    dst->count = 0;

    for (int i = 0; i < src->count; i++) {

        Item it = src->items[i];

        if (prodRHS[it.p][it.dot] == X) {
            dst->items[dst->count++] = (Item){it.p, it.dot + 1};
        }
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

/* ---------- canonical collection ---------- */

void buildStates() {

    states[0].count = 1;
    states[0].items[0] = (Item){0, 0};
    closure(&states[0]);

    stateCount = 1;

    for (int i = 0; i < stateCount; i++) {

        for (char X = 'A'; X <= 'Z'; X++) {

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

        for (char X = 'a'; X <= 'z'; X++) {

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

/* ---------- FOLLOW computation (simple) ---------- */

void computeFOLLOW() {

    FOLLOW[prodLHS[0]-'A'][followCount[prodLHS[0]-'A']++] = '$';

    for (int p = 0; p < prodCount; p++) {

        for (int i = 0; prodRHS[p][i]; i++) {

            if (isupper(prodRHS[p][i]) && prodRHS[p][i+1]) {

                char next = prodRHS[p][i+1];

                if (!isupper(next))
                    FOLLOW[prodRHS[p][i]-'A']
                        [followCount[prodRHS[p][i]-'A']++] = next;
            }
        }
    }
}

/* ---------- ACTION/GOTO table ---------- */

char ACTION[MAX][MAX][MAX_LEN];
int GOTO[MAX][MAX];

/* ---------- table construction ---------- */

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

                    int A = prodLHS[it.p] - 'A';

                    for (int f = 0; f < followCount[A]; f++) {

                        if (strlen(ACTION[i][FOLLOW[A][f]]))
                            conflict = 1;

                        sprintf(ACTION[i][FOLLOW[A][f]], "r%d", it.p);
                    }
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

/* ---------- print table ---------- */

void printTable() {

    printf("\nSLR(1) ACTION/GOTO TABLE\n\n");

    for (int i = 0; i < stateCount; i++) {

        printf("State %d:\n", i);

        for (char t = 'a'; t <= 'z'; t++)
            if (strlen(ACTION[i][t]))
                printf("  ACTION[%c] = %s\n", t, ACTION[i][t]);

        if (strlen(ACTION[i]['$']))
            printf("  ACTION[$] = %s\n", ACTION[i]['$']);

        for (char nt = 'A'; nt <= 'Z'; nt++)
            if (GOTO[i][nt])
                printf("  GOTO[%c] = %d\n", nt, GOTO[i][nt]);

        printf("\n");
    }

    if (conflict)
        printf("Grammar is NOT SLR(1) (conflict detected).\n");
    else
        printf("Grammar is SLR(1).\n");
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

    computeFOLLOW();
    buildStates();
    buildTable();
    printTable();

    return 0;
}
