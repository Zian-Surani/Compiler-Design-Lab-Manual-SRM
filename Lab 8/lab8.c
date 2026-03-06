#include <stdio.h>
#include <string.h>

#define MAX 50
#define MAX_PROD 20

char stack[MAX];
int top = -1;

char lhs[MAX_PROD];
char rhs[MAX_PROD][MAX];

int n;

/* stack operations */

void push(char c)
{
    stack[++top] = c;
    stack[top+1] = '\0';
}

void pop(int count)
{
    while(count--)
        top--;
    stack[top+1] = '\0';
}

void printState(char *input, int ip, char *action)
{
    printf("%-15s %-15s %-15s\n", stack, input + ip, action);
}

/* reduction function */

int reduce()
{
    for(int i = 0; i < n; i++)
    {
        int len = strlen(rhs[i]);

        if(top + 1 >= len)
        {
            if(strncmp(stack + top - len + 1, rhs[i], len) == 0)
            {
                pop(len);
                push(lhs[i]);
                return 1;
            }
        }
    }

    return 0;
}

/* shift reduce parser */

int parse(char *input)
{
    int ip = 0;

    printf("\n%-15s %-15s %-15s\n","STACK","INPUT","ACTION");
    printf("--------------------------------------------------\n");

    while(1)
    {

        if(reduce())
        {
            printState(input, ip, "REDUCE");
            continue;
        }

        if(input[ip] == '$' && top == 0 && stack[0] == lhs[0])
            return 1;

        if(input[ip] == '\0')
            return 0;

        push(input[ip++]);
        printState(input, ip, "SHIFT");
    }
}

/* grammar input */

void inputGrammar()
{
    printf("\nEnter number of productions: ");
    scanf("%d",&n);

    for(int i=0;i<n;i++)
    {
        printf("Production %d (Example E->E+E): ",i+1);
        char prod[MAX];

        scanf("%s",prod);

        lhs[i] = prod[0];
        strcpy(rhs[i], prod+3);
    }
}

/* reset stack */

void reset()
{
    top = -1;
    stack[0] = '\0';
}

/* main menu */

int main()
{
    int choice;
    char input[MAX];

    while(1)
    {
        printf("\n===== SHIFT REDUCE PARSER =====\n");
        printf("1. Enter Grammar\n");
        printf("2. Parse String\n");
        printf("3. Exit\n");
        printf("Enter choice: ");
        scanf("%d",&choice);

        switch(choice)
        {

            case 1:
                inputGrammar();
                break;

            case 2:

                printf("Enter input string ending with $ : ");
                scanf("%s",input);

                reset();

                if(parse(input))
                    printf("\nSTRING ACCEPTED\n");
                else
                    printf("\nSTRING REJECTED\n");

                break;

            case 3:
                return 0;

            default:
                printf("Invalid choice\n");
        }
    }
}
