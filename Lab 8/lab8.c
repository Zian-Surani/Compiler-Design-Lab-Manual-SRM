#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX 50

/* Grammar used:
   E → E+E | E*E | (E) | id
   id is represented by 'i'
*/

char stack[MAX];
int top = -1;

/* ---------- utility ---------- */

void push(char c) {
    if (top >= MAX - 1) {
        printf("Stack overflow.\n");
        return;
    }
    stack[++top] = c;
}

void pop(int count) {
    while (count-- > 0 && top >= 0)
        top--;
}

void printStack(char *input, int ip, char *action) {

    printf("%-15s %-15s %-15s\n", stack, input + ip, action);
}

/* ---------- reduction rules ---------- */

int reduce() {

    /* id → E  (i → E) */
    if (top >= 0 && stack[top] == 'i') {
        stack[top] = 'E';
        return 1;
    }

    /* (E) → E */
    if (top >= 2 &&
        stack[top] == ')' &&
        stack[top - 1] == 'E' &&
        stack[top - 2] == '(') {

        pop(2);
        stack[top] = 'E';
        return 1;
    }

    /* E+E → E */
    if (top >= 2 &&
        stack[top] == 'E' &&
        stack[top - 1] == '+' &&
        stack[top - 2] == 'E') {

        pop(2);
        stack[top] = 'E';
        return 1;
    }

    /* E*E → E */
    if (top >= 2 &&
        stack[top] == 'E' &&
        stack[top - 1] == '*' &&
        stack[top - 2] == 'E') {

        pop(2);
        stack[top] = 'E';
        return 1;
    }

    return 0;
}

/* ---------- shift-reduce parsing ---------- */

int parse(char *input) {

    int ip = 0;

    printf("\n%-15s %-15s %-15s\n", "STACK", "INPUT", "ACTION");
    printf("-------------------------------------------------------\n");

    while (1) {

        /* try reductions repeatedly */
        if (reduce()) {
            printStack(input, ip, "REDUCE");
            continue;
        }

        /* accept condition */
        if (stack[0] == 'E' && stack[1] == '\0' && input[ip] == '$')
            return 1;

        /* shift next symbol */
        if (input[ip] == '\0')
            return 0;

        if (!strchr("i+*()$", input[ip]))
            return -1;

        char sym = input[ip++];
        push(sym);

        stack[top + 1] = '\0';
        printStack(input, ip, "SHIFT");
    }
}

/* ---------- main ---------- */

int main() {

    char input[MAX];

    printf("Enter expression using i,+,*,(,) and ending with $: ");

    if (scanf("%s", input) != 1) {
        printf("Invalid input read.\n");
        return 1;
    }

    int len = strlen(input);

    if (len == 0 || input[len - 1] != '$') {
        printf("Input must terminate with $.\n");
        return 1;
    }

    int result = parse(input);

    if (result == 1)
        printf("\nString ACCEPTED by shift-reduce parser.\n");
    else if (result == 0)
        printf("\nString REJECTED: grammar cannot derive input.\n");
    else
        printf("\nString REJECTED: invalid symbol in input.\n");

    return 0;
}
