/*
Example I/O

Enter number of TAC instructions: 4

Enter TAC in format: result op1 operator op2

t1 = 2 * 3
t2 = t1 + 0
t3 = t1 + 0
t4 = t3 * 1

OUTPUT

After Constant Folding:
t1 = 6
t2 = t1 + 0
t3 = t1 + 0
t4 = t3 * 1

After Common Subexpression Elimination:
t1 = 6
t2 = t1 + 0
t3 = t2
t4 = t3 * 1

After Dead Code Removal:
t2 = t1 + 0
t3 = t2
t4 = t3 * 1

After Strength Reduction:
t2 = t1
t3 = t2
t4 = t3
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX 50
#define LEN 20

typedef struct {
    char res[LEN];
    char op1[LEN];
    char op2[LEN];
    char op;
    int valid;
} TAC;

TAC code[MAX];
int n;

/* ---------- utility ---------- */

int isNumber(char *s) {
    for (int i = 0; s[i]; i++)
        if (!isdigit(s[i])) return 0;
    return 1;
}

/* ---------- constant folding ---------- */

void constantFolding() {

    for (int i = 0; i < n; i++) {

        if (!code[i].valid) continue;

        if (isNumber(code[i].op1) && isNumber(code[i].op2)) {

            int a = atoi(code[i].op1);
            int b = atoi(code[i].op2);
            int result = 0;

            switch (code[i].op) {
                case '+': result = a + b; break;
                case '-': result = a - b; break;
                case '*': result = a * b; break;
                case '/': if (b != 0) result = a / b; break;
            }

            sprintf(code[i].op1, "%d", result);
            strcpy(code[i].op2, "");
            code[i].op = '=';
        }
    }

    printf("\nAfter Constant Folding:\n");
    for (int i = 0; i < n; i++)
        if (code[i].valid)
            printf("%s = %s %c %s\n",
                   code[i].res, code[i].op1,
                   code[i].op, code[i].op2);
}

/* ---------- common subexpression ---------- */

void commonSubexpr() {

    for (int i = 0; i < n; i++) {

        if (!code[i].valid) continue;

        for (int j = i + 1; j < n; j++) {

            if (!code[j].valid) continue;

            if (strcmp(code[i].op1, code[j].op1) == 0 &&
                strcmp(code[i].op2, code[j].op2) == 0 &&
                code[i].op == code[j].op) {

                strcpy(code[j].op1, code[i].res);
                strcpy(code[j].op2, "");
                code[j].op = '=';
            }
        }
    }

    printf("\nAfter Common Subexpression Elimination:\n");
    for (int i = 0; i < n; i++)
        if (code[i].valid)
            printf("%s = %s %c %s\n",
                   code[i].res, code[i].op1,
                   code[i].op, code[i].op2);
}

/* ---------- dead code removal ---------- */

void deadCode() {

    for (int i = 0; i < n - 1; i++) {

        int used = 0;

        for (int j = i + 1; j < n; j++) {

            if (strcmp(code[i].res, code[j].op1) == 0 ||
                strcmp(code[i].res, code[j].op2) == 0) {
                used = 1;
                break;
            }
        }

        if (!used)
            code[i].valid = 0;
    }

    printf("\nAfter Dead Code Removal:\n");
    for (int i = 0; i < n; i++)
        if (code[i].valid)
            printf("%s = %s %c %s\n",
                   code[i].res, code[i].op1,
                   code[i].op, code[i].op2);
}

/* ---------- strength reduction ---------- */

void strengthReduction() {

    for (int i = 0; i < n; i++) {

        if (!code[i].valid) continue;

        if (code[i].op == '*' &&
            (strcmp(code[i].op2, "1") == 0 ||
             strcmp(code[i].op1, "1") == 0)) {

            if (strcmp(code[i].op2, "1") == 0)
                strcpy(code[i].op1, code[i].op1);
            else
                strcpy(code[i].op1, code[i].op2);

            strcpy(code[i].op2, "");
            code[i].op = '=';
        }

        if (code[i].op == '+' &&
            strcmp(code[i].op2, "0") == 0) {

            strcpy(code[i].op2, "");
            code[i].op = '=';
        }
    }

    printf("\nAfter Strength Reduction:\n");
    for (int i = 0; i < n; i++)
        if (code[i].valid)
            printf("%s = %s %c %s\n",
                   code[i].res, code[i].op1,
                   code[i].op, code[i].op2);
}

/* ---------- main ---------- */

int main() {

    printf("Enter number of TAC instructions: ");
    if (scanf("%d", &n) != 1 || n <= 0 || n > MAX) {
        printf("Invalid number of instructions.\n");
        return 1;
    }

    printf("\nEnter TAC in format: result op1 operator op2\n\n");

    for (int i = 0; i < n; i++) {

        code[i].valid = 1;

        if (scanf("%s = %s %c %s",
                  code[i].res,
                  code[i].op1,
                  &code[i].op,
                  code[i].op2) != 4) {

            printf("Invalid TAC format.\n");
            return 1;
        }
    }

    constantFolding();
    commonSubexpr();
    deadCode();
    strengthReduction();

    return 0;
}
