/*
Example I/O

Enter arithmetic expression (use i,+,*,/,() ):
i+i*i

OUTPUT

Postfix Expression: ii*i+

Three Address Code:
t1 = i * i
t2 = i + t1
Result stored in: t2
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX 100

char stack[MAX];
int top = -1;

char postfix[MAX];
int pIndex = 0;

char tacLHS[MAX][10];
char tacOp1[MAX][10];
char tacOp2[MAX][10];
char tacOp[MAX];
int tacCount = 0;

int tempCount = 1;

/* ---------- stack ---------- */

void push(char c) {
    stack[++top] = c;
}

char pop() {
    return stack[top--];
}

char peek() {
    return stack[top];
}

/* ---------- precedence ---------- */

int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

/* ---------- infix → postfix ---------- */

int infixToPostfix(char *infix) {

    for (int i = 0; infix[i]; i++) {

        char c = infix[i];

        if (isalnum(c)) {
            postfix[pIndex++] = c;
        }
        else if (c == '(') {
            push(c);
        }
        else if (c == ')') {

            while (top >= 0 && peek() != '(')
                postfix[pIndex++] = pop();

            if (top < 0) return 0;
            pop();
        }
        else if (strchr("+-*/", c)) {

            while (top >= 0 && precedence(peek()) >= precedence(c))
                postfix[pIndex++] = pop();

            push(c);
        }
        else {
            return -1;
        }
    }

    while (top >= 0)
        postfix[pIndex++] = pop();

    postfix[pIndex] = '\0';
    return 1;
}

/* ---------- TAC generation ---------- */

void generateTAC() {

    char evalStack[MAX][10];
    int etop = -1;

    for (int i = 0; postfix[i]; i++) {

        char c = postfix[i];

        if (isalnum(c)) {
            evalStack[++etop][0] = c;
            evalStack[etop][1] = '\0';
        }
        else {

            char op2[10], op1[10];

            strcpy(op2, evalStack[etop--]);
            strcpy(op1, evalStack[etop--]);

            char temp[10];
            sprintf(temp, "t%d", tempCount++);

            strcpy(tacLHS[tacCount], temp);
            strcpy(tacOp1[tacCount], op1);
            strcpy(tacOp2[tacCount], op2);
            tacOp[tacCount] = c;
            tacCount++;

            strcpy(evalStack[++etop], temp);
        }
    }

    printf("\nThree Address Code:\n");

    for (int i = 0; i < tacCount; i++)
        printf("%s = %s %c %s\n",
               tacLHS[i], tacOp1[i], tacOp[i], tacOp2[i]);

    printf("Result stored in: %s\n", evalStack[etop]);
}

/* ---------- main ---------- */

int main() {

    char infix[MAX];

    printf("Enter arithmetic expression (use i,+,*,/,() ): ");

    if (scanf("%s", infix) != 1) {
        printf("Invalid input read.\n");
        return 1;
    }

    for (int i = 0; infix[i]; i++) {
        if (!isalnum(infix[i]) && !strchr("+-*/()", infix[i])) {
            printf("Invalid character in expression.\n");
            return 1;
        }
    }

    int status = infixToPostfix(infix);

    if (status == 0) {
        printf("Mismatched parentheses.\n");
        return 1;
    }
    if (status == -1) {
        printf("Invalid symbol encountered.\n");
        return 1;
    }

    printf("\nPostfix Expression: %s\n", postfix);

    generateTAC();

    return 0;
}
