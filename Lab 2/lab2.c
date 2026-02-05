// c code to implement regex to nfa conversion with the transition table
/*Convert infix regex → postfix

Scan postfix:

Symbol → create basic NFA

. → concatenate

| → union

* → Kleene star

Final stack element → complete NFA*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX 100
#define EPSILON 'ε'

/* ================= STATE & TRANSITION ================= */

typedef struct state State;
typedef struct transition Transition;

struct transition {
    char symbol;
    State *to;
    Transition *next;
};

struct state {
    int id;
    Transition *transitions;
};

/* ================= NFA STRUCTURE ================= */

typedef struct {
    State *start;
    State *end;
} NFA;

int stateCount = 0;

/* ================= STACK ================= */

typedef struct {
    NFA items[MAX];
    int top;
} Stack;

void push(Stack *s, NFA nfa) { s->items[++s->top] = nfa; }
NFA pop(Stack *s) { return s->items[s->top--]; }

/* ================= CREATE STATE ================= */

State* createState() {
    State *s = (State*)malloc(sizeof(State));
    s->id = stateCount++;
    s->transitions = NULL;
    return s;
}

/* ================= ADD TRANSITION (Linked List) ================= */

void addTransition(State *from, State *to, char symbol) {
    Transition *t = (Transition*)malloc(sizeof(Transition));
    t->symbol = symbol;
    t->to = to;
    t->next = from->transitions;
    from->transitions = t;
}

/* ================= BASIC SYMBOL NFA ================= */

NFA createSymbolNFA(char symbol) {
    NFA nfa;
    nfa.start = createState();
    nfa.end   = createState();

    addTransition(nfa.start, nfa.end, symbol);
    return nfa;
}

/* ================= CONCATENATION ================= */

NFA concatenate(NFA a, NFA b) {
    addTransition(a.end, b.start, EPSILON);
    a.end = b.end;
    return a;
}

/* ================= UNION ================= */

NFA unionNFA(NFA a, NFA b) {
    NFA nfa;
    nfa.start = createState();
    nfa.end   = createState();

    addTransition(nfa.start, a.start, EPSILON);
    addTransition(nfa.start, b.start, EPSILON);
    addTransition(a.end, nfa.end, EPSILON);
    addTransition(b.end, nfa.end, EPSILON);

    return nfa;
}

/* ================= KLEENE STAR ================= */

NFA kleeneStar(NFA a) {
    NFA nfa;
    nfa.start = createState();
    nfa.end   = createState();

    addTransition(nfa.start, a.start, EPSILON);
    addTransition(nfa.start, nfa.end, EPSILON);
    addTransition(a.end, a.start, EPSILON);
    addTransition(a.end, nfa.end, EPSILON);

    return nfa;
}

/* ================= PRECEDENCE ================= */

int precedence(char op) {
    if (op == '*') return 3;
    if (op == '.') return 2;
    if (op == '|') return 1;
    return 0;
}

/* ================= ADD CONCATENATION SYMBOL ================= */

void insertConcat(char *regex, char *output) {
    int j = 0;

    for (int i = 0; regex[i]; i++) {
        output[j++] = regex[i];

        if (regex[i+1]) {
            if ((isalnum(regex[i]) || regex[i] == '*' || regex[i] == ')') &&
                (isalnum(regex[i+1]) || regex[i+1] == '(')) {
                output[j++] = '.';
            }
        }
    }
    output[j] = '\0';
}

/* ================= INFIX → POSTFIX ================= */

void infixToPostfix(char *infix, char *postfix) {
    char stack[MAX];
    int top = -1, j = 0;

    for (int i = 0; infix[i]; i++) {
        char c = infix[i];

        if (isalnum(c)) {
            postfix[j++] = c;
        }
        else if (c == '(') {
            stack[++top] = c;
        }
        else if (c == ')') {
            while (stack[top] != '(')
                postfix[j++] = stack[top--];
            top--;
        }
        else {
            while (top >= 0 && precedence(stack[top]) >= precedence(c))
                postfix[j++] = stack[top--];
            stack[++top] = c;
        }
    }

    while (top >= 0)
        postfix[j++] = stack[top--];

    postfix[j] = '\0';
}

/* ================= POSTFIX → NFA ================= */

NFA postfixToNFA(char *postfix) {
    Stack s;
    s.top = -1;

    for (int i = 0; postfix[i]; i++) {
        char c = postfix[i];

        if (isalnum(c)) {
            push(&s, createSymbolNFA(c));
        }
        else if (c == '.') {
            NFA b = pop(&s);
            NFA a = pop(&s);
            push(&s, concatenate(a, b));
        }
        else if (c == '|') {
            NFA b = pop(&s);
            NFA a = pop(&s);
            push(&s, unionNFA(a, b));
        }
        else if (c == '*') {
            NFA a = pop(&s);
            push(&s, kleeneStar(a));
        }
    }

    return pop(&s);
}

/* ================= PRINT NFA ================= */

void printNFA(State *s, int visited[]) {
    if (!s || visited[s->id]) return;

    visited[s->id] = 1;

    for (Transition *t = s->transitions; t; t = t->next) {
        printf("δ(q%d, %c) → q%d\n", s->id, t->symbol, t->to->id);
        printNFA(t->to, visited);
    }
}

/* ================= MAIN ================= */

int main() {
    char regex[MAX], modified[MAX], postfix[MAX];

    printf("Enter Regular Expression: ");
    scanf("%s", regex);

    insertConcat(regex, modified);
    infixToPostfix(modified, postfix);

    printf("\nPostfix Expression: %s\n", postfix);

    NFA nfa = postfixToNFA(postfix);

    int visited[200] = {0};

    printf("\nNFA Transition Table:\n");
    printNFA(nfa.start, visited);

    printf("\nStart State: q%d", nfa.start->id);
    printf("\nFinal State: q%d\n", nfa.end->id);

    return 0;
}