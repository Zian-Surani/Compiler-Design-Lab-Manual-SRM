/*
Example I/O

Enter number of TAC instructions: 3

Enter TAC in format: result op1 operator op2

t1 = a + b
t2 = t1 * c
t3 = t2 - d

OUTPUT

Generated Target Code:

MOV R0, a
ADD R0, b
MOV t1, R0

MOV R0, t1
MUL R0, c
MOV t2, R0

MOV R0, t2
SUB R0, d
MOV t3, R0
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX 50
#define LEN 20

typedef struct {
    char res[LEN];
    char op1[LEN];
    char op2[LEN];
    char op;
} TAC;

TAC code[MAX];
int n;

/* ---------- opcode mapping ---------- */

const char* getOpcode(char op) {
    switch (op) {
        case '+': return "ADD";
        case '-': return "SUB";
        case '*': return "MUL";
        case '/': return "DIV";
        case '=': return "MOV";
        default:  return "UNK";
    }
}

/* ---------- validation ---------- */

int isValidOperator(char op) {
    return op=='+' || op=='-' || op=='*' || op=='/' || op=='=';
}

/* ---------- target code generation ---------- */

void generateTargetCode() {

    printf("\nGenerated Target Code:\n\n");

    for (int i = 0; i < n; i++) {

        if (!isValidOperator(code[i].op)) {
            printf("Invalid operator in TAC at line %d.\n", i + 1);
            return;
        }

        /* assignment case: t = x */
        if (code[i].op == '=') {
            printf("MOV %s, %s\n\n", code[i].res, code[i].op1);
            continue;
        }

        /* load first operand */
        printf("MOV R0, %s\n", code[i].op1);

        /* apply operation */
        printf("%s R0, %s\n", getOpcode(code[i].op), code[i].op2);

        /* store result */
        printf("MOV %s, R0\n\n", code[i].res);
    }
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

        if (scanf("%s = %s %c %s",
                  code[i].res,
                  code[i].op1,
                  &code[i].op,
                  code[i].op2) != 4) {

            printf("Invalid TAC format at line %d.\n", i + 1);
            return 1;
        }
    }

    generateTargetCode();

    return 0;
}
