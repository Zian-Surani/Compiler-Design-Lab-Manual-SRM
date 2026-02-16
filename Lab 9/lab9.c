#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX 50

/* Terminals supported: i, +, *, (, ), $ */

char stack[MAX];
int top = -1;

/* precedence table
      i   +   *   (   )   $
i     >   >   >   -   >   >
+     <   >   <   <   >   >
*     <   >   >   <   >   >
(     <   <   <   <   =   -
)     >   >   >   -   >   >
$     <   <   <   <   -   =
*/

char precedence(char a, char b) {

    if (a == 'i' && strchr("+*)$", b)) return '>';
    if (a == '+' && b == '+') return '>';
    if (a == '+' && b == '*') return '<';
    if (a == '+' && strchr("i(", b)) return '<';
    if (a == '+' && b == ')') return '>';
    if (a == '+' && b == '$') return '>';

    if (a == '*' && strchr("i(", b)) return '<';
    if (a == '*' && strchr("+*)$", b)) return '>';

    if (a == '(' && b == ')') return '=';
    if (a == '(') return '<';

    if (a == ')' && strchr("+*)$", b)) return '>';

    if (a == '$' && b == '$') return '=';
    if (a == '$') return '<';

    return '-'; /* invalid */
}

/* ---------- utility ---------- */

void push(char c) {
    if (top >= MAX - 1) return;
    stack[++top] = c;
    stack[top + 1] = '\0';
}

void pop() {
    if (top >= 0) top--;
    stack[top + 1] = '\0';
}

void printState(char *input, int ip, char *action) {
    printf("%-15s %-15s %-10s\n", stack, input + ip, action);
}

/* ---------- reduction ---------- */

int reduce() {

    /* i → E (represented as i → N for nonterminal placeholder) */
    if (top >= 0 && stack[top] == 'i') {
        stack[top] = 'N';
        return 1;
    }

    /* (N) → N */
    if (top >= 2 &&
        stack[top] == ')' &&
        stack[top - 1] == 'N' &&
        stack[top - 2] == '(') {

        top -= 2;
        stack[top] = 'N';
        return 1;
    }

    /* N+N → N */
    if (top >= 2 &&
        stack[top] == 'N' &&
        stack[top - 1] == '+' &&
        stack[top - 2] == 'N') {

        top -= 2;
        stack[top] = 'N';
        return 1;
    }

    /* N*N → N */
    if (top >= 2 &&
        stack[top] == 'N' &&
        stack[top - 1] == '*' &&
        stack[top - 2] == 'N') {

        top -= 2;
        stack[top] = 'N';
        return 1;
    }

    return 0;
}

/* ---------- parsing ---------- */

int parse(char *input) {

    int ip = 0;

    push('$');

    printf("\n%-15s %-15s %-10s\n", "STACK", "INPUT", "ACTION");
    printf("---------------------------------------------\n");

    while (1) {

        char a = stack[top];
        char b = input[ip];

        if (a == 'N') {
            int k = top - 1;
            while (k >= 0 && stack[k] == 'N') k--;
            a = stack[k];
        }

        char rel = precedence(a, b);

        if (rel == '<' || rel == '=') {
            push(b);
            ip++;
            printState(input, ip, "SHIFT");
        }
        else if (rel == '>') {
            if (!reduce())
                return 0;
            printState(input, ip, "REDUCE");
        }
        else {
            return -1;
        }

        if (stack[0] == '$' && stack[1] == 'N' &&
            stack[2] == '\0' && input[ip] == '$')
            return 1;
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

    for (int i = 0; i < len - 1; i++) {
        if (!strchr("i+*()", input[i])) {
            printf("Invalid symbol in input.\n");
            return 1;
        }
    }

    int result = parse(input);

    if (result == 1)
        printf("\nString ACCEPTED by operator-precedence parser.\n");
    else if (result == 0)
        printf("\nString REJECTED: precedence reduction failed.\n");
    else
        printf("\nString REJECTED: invalid precedence relation.\n");

    return 0;
}
