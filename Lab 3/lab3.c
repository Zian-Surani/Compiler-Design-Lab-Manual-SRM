#include <stdio.h>
#include <string.h>

#define MAX_STATES 10
#define MAX_SYMBOLS 10

/* ================= NFA ================= */

typedef struct {
    int numStates;
    int numSymbols;
    char symbols[MAX_SYMBOLS];

    int trans[MAX_STATES][MAX_SYMBOLS][MAX_STATES];
    int transCount[MAX_STATES][MAX_SYMBOLS];

    int epsilon[MAX_STATES][MAX_STATES];
    int epsilonCount[MAX_STATES];

    int start;
    int final[MAX_STATES];
} NFA;

/* ================= DFA ================= */

typedef struct {
    int states[1 << MAX_STATES][MAX_STATES];
    int stateCount;
    int final[1 << MAX_STATES];
    int trans[1 << MAX_STATES][MAX_SYMBOLS];
} DFA;

/* ================= ε-CLOSURE ================= */

void epsilonClosure(NFA *nfa, int *input, int *closure) {
    int stack[MAX_STATES], top = -1;

    for (int i = 0; i < nfa->numStates; i++) {
        closure[i] = input[i];
        if (input[i]) stack[++top] = i;
    }

    while (top >= 0) {
        int s = stack[top--];

        for (int i = 0; i < nfa->epsilonCount[s]; i++) {
            int nxt = nfa->epsilon[s][i];
            if (!closure[nxt]) {
                closure[nxt] = 1;
                stack[++top] = nxt;
            }
        }
    }
}

/* ================= MOVE ================= */

void move(NFA *nfa, int *stateSet, int sym, int *result) {
    memset(result, 0, sizeof(int) * MAX_STATES);

    for (int i = 0; i < nfa->numStates; i++) {
        if (stateSet[i]) {
            for (int j = 0; j < nfa->transCount[i][sym]; j++) {
                result[nfa->trans[i][sym][j]] = 1;
            }
        }
    }
}

/* ================= CHECK SAME SET ================= */

int same(int *a, int *b, int n) {
    for (int i = 0; i < n; i++)
        if (a[i] != b[i]) return 0;
    return 1;
}

/* ================= SUBSET CONSTRUCTION ================= */

void convert(NFA *nfa, DFA *dfa) {
    int startSet[MAX_STATES] = {0};
    startSet[nfa->start] = 1;

    epsilonClosure(nfa, startSet, dfa->states[0]);
    dfa->stateCount = 1;

    for (int i = 0; i < dfa->stateCount; i++) {

        for (int j = 0; j < nfa->numStates; j++)
            if (dfa->states[i][j] && nfa->final[j])
                dfa->final[i] = 1;

        for (int s = 0; s < nfa->numSymbols; s++) {

            int mv[MAX_STATES], cl[MAX_STATES];

            move(nfa, dfa->states[i], s, mv);
            epsilonClosure(nfa, mv, cl);

            int found = -1;
            for (int j = 0; j < dfa->stateCount; j++) {
                if (same(dfa->states[j], cl, nfa->numStates)) {
                    found = j;
                    break;
                }
            }

            if (found == -1) {
                found = dfa->stateCount;
                memcpy(dfa->states[dfa->stateCount++], cl,
                       sizeof(int) * MAX_STATES);
            }

            dfa->trans[i][s] = found;
        }
    }
}

/* ================= PRINT DFA ================= */

void printDFA(DFA *dfa, NFA *nfa) {
    printf("\n===== DFA TRANSITION TABLE =====\n\n");

    for (int i = 0; i < dfa->stateCount; i++) {

        printf("State %d { ", i);
        for (int j = 0; j < nfa->numStates; j++)
            if (dfa->states[i][j]) printf("q%d ", j);
        printf("}");

        if (dfa->final[i]) printf("  [FINAL]");
        printf("\n");

        for (int s = 0; s < nfa->numSymbols; s++) {
            printf("   --%c--> State %d\n",
                   nfa->symbols[s], dfa->trans[i][s]);
        }
        printf("\n");
    }
}

/* ================= MAIN ================= */

int main() {
    NFA nfa = {0};
    DFA dfa = {0};

    printf("Enter number of NFA states: ");
    scanf("%d", &nfa.numStates);

    printf("Enter number of symbols: ");
    scanf("%d", &nfa.numSymbols);

    printf("Enter symbols: ");
    for (int i = 0; i < nfa.numSymbols; i++)
        scanf(" %c", &nfa.symbols[i]);

    printf("Enter start state: ");
    scanf("%d", &nfa.start);

    int fcount;
    printf("Enter number of final states: ");
    scanf("%d", &fcount);

    printf("Enter final states: ");
    for (int i = 0; i < fcount; i++) {
        int x; scanf("%d", &x);
        nfa.final[x] = 1;
    }

    int t;
    printf("Enter number of transitions: ");
    scanf("%d", &t);

    printf("Enter transitions: from symbol to\n");
    for (int i = 0; i < t; i++) {
        int from, to;
        char sym;
        scanf("%d %c %d", &from, &sym, &to);

        char *ptr = strchr(nfa.symbols, sym);
        if (!ptr) {
            printf("Invalid symbol!\n");
            return 1;
        }

        int idx = ptr - nfa.symbols;
        nfa.trans[from][idx][nfa.transCount[from][idx]++] = to;
    }

    int e;
    printf("Enter number of epsilon transitions: ");
    scanf("%d", &e);

    printf("Enter epsilon transitions: from to\n");
    for (int i = 0; i < e; i++) {
        int from, to;
        scanf("%d %d", &from, &to);
        nfa.epsilon[from][nfa.epsilonCount[from]++] = to;
    }

    convert(&nfa, &dfa);
    printDFA(&dfa, &nfa);

    return 0;
}
