//c code implementation for converting a given NFA to DFA using subset conversion algorithm

/*Compute ε-closure(start state) → DFA start state

For each DFA state and input symbol:

Apply move

Take ε-closure

Form new DFA state

Mark DFA state final if it contains any NFA final state*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STATES 20
#define MAX_SYMBOLS 10
#define EPSILON 'e'

/* ================= NFA STRUCTURE ================= */

typedef struct {
    int numStates;
    int numSymbols;
    char symbols[MAX_SYMBOLS];
    int transition[MAX_STATES][MAX_SYMBOLS][MAX_STATES]; 
    int transCount[MAX_STATES][MAX_SYMBOLS];
    int epsilon[MAX_STATES][MAX_STATES];
    int epsilonCount[MAX_STATES];
    int startState;
    int finalStates[MAX_STATES];
} NFA;

/* ================= DFA STRUCTURE ================= */

typedef struct {
    int states[1 << MAX_STATES][MAX_STATES];
    int stateCount;
    int isFinal[1 << MAX_STATES];
    int transition[1 << MAX_STATES][MAX_SYMBOLS];
} DFA;

/* ================= ε-CLOSURE ================= */

void epsilonClosure(NFA *nfa, int *inputSet, int *closure) {
    int stack[MAX_STATES], top = -1;

    for (int i = 0; i < nfa->numStates; i++) {
        closure[i] = inputSet[i];
        if (inputSet[i])
            stack[++top] = i;
    }

    while (top >= 0) {
        int s = stack[top--];

        for (int i = 0; i < nfa->epsilonCount[s]; i++) {
            int next = nfa->epsilon[s][i];
            if (!closure[next]) {
                closure[next] = 1;
                stack[++top] = next;
            }
        }
    }
}

/* ================= MOVE FUNCTION ================= */

void move(NFA *nfa, int *stateSet, int symbolIndex, int *result) {
    memset(result, 0, sizeof(int) * MAX_STATES);

    for (int i = 0; i < nfa->numStates; i++) {
        if (stateSet[i]) {
            for (int j = 0; j < nfa->transCount[i][symbolIndex]; j++) {
                int next = nfa->transition[i][symbolIndex][j];
                result[next] = 1;
            }
        }
    }
}

/* ================= CHECK SET EXISTENCE ================= */

int sameSet(int *a, int *b, int n) {
    for (int i = 0; i < n; i++)
        if (a[i] != b[i]) return 0;
    return 1;
}

/* ================= NFA → DFA ================= */

void convertToDFA(NFA *nfa, DFA *dfa) {
    int startSet[MAX_STATES] = {0};
    startSet[nfa->startState] = 1;

    epsilonClosure(nfa, startSet, dfa->states[0]);
    dfa->stateCount = 1;

    for (int i = 0; i < dfa->stateCount; i++) {

        /* Mark final states */
        for (int f = 0; f < nfa->numStates; f++)
            if (dfa->states[i][f] && nfa->finalStates[f])
                dfa->isFinal[i] = 1;

        for (int s = 0; s < nfa->numSymbols; s++) {

            int moveSet[MAX_STATES], closure[MAX_STATES];

            move(nfa, dfa->states[i], s, moveSet);
            epsilonClosure(nfa, moveSet, closure);

            /* Check if closure already exists */
            int found = -1;
            for (int j = 0; j < dfa->stateCount; j++) {
                if (sameSet(dfa->states[j], closure, nfa->numStates)) {
                    found = j;
                    break;
                }
            }

            /* New DFA state */
            if (found == -1) {
                found = dfa->stateCount;
                memcpy(dfa->states[dfa->stateCount++], closure,
                       sizeof(int) * MAX_STATES);
            }

            dfa->transition[i][s] = found;
        }
    }
}

/* ================= PRINT DFA ================= */

void printDFA(DFA *dfa, NFA *nfa) {
    printf("\nDFA Transition Table:\n");

    for (int i = 0; i < dfa->stateCount; i++) {

        printf("State %d { ", i);
        for (int j = 0; j < nfa->numStates; j++)
            if (dfa->states[i][j]) printf("q%d ", j);
        printf("}");

        if (dfa->isFinal[i]) printf("  [Final]");

        printf("\n");

        for (int s = 0; s < nfa->numSymbols; s++) {
            printf("  --%c--> State %d\n",
                   nfa->symbols[s],
                   dfa->transition[i][s]);
        }
        printf("\n");
    }
}

/* ================= SAMPLE NFA INPUT ================= */

void sampleNFA(NFA *nfa) {
    nfa->numStates = 3;
    nfa->numSymbols = 2;

    nfa->symbols[0] = 'a';
    nfa->symbols[1] = 'b';

    nfa->startState = 0;

    memset(nfa->transition, 0, sizeof(nfa->transition));
    memset(nfa->transCount, 0, sizeof(nfa->transCount));
    memset(nfa->epsilonCount, 0, sizeof(nfa->epsilonCount));
    memset(nfa->finalStates, 0, sizeof(nfa->finalStates));

    /* Transitions */
    nfa->transition[0][0][nfa->transCount[0][0]++] = 0;
    nfa->transition[0][0][nfa->transCount[0][0]++] = 1;
    nfa->transition[1][1][nfa->transCount[1][1]++] = 2;

    /* ε-transition */
    nfa->epsilon[0][nfa->epsilonCount[0]++] = 2;

    /* Final state */
    nfa->finalStates[2] = 1;
}

/* ================= MAIN ================= */

int main() {
    NFA nfa;
    DFA dfa = {0};

    sampleNFA(&nfa);

    convertToDFA(&nfa, &dfa);

    printDFA(&dfa, &nfa);

    return 0;
}
