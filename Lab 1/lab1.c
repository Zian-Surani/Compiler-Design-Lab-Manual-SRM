// c code implementation for developing a lexical analyzer 
//[conseptually a DFA equivalent method used with if-else and looping statements for conditional logic] 
/*  ⭐ Line-wise debugging output
    ⭐ Real symbol table construction
    ⭐ Handles strings & multi-line comments
    ⭐ Detects double operators (==, <=, &&)
    ⭐ Clean enum-based token system
    ⭐ Structured like a real compiler front-end*/

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define MAX_TOKEN 100
#define MAX_SYMBOLS 200

/* ================= TOKEN TYPES ================= */

typedef enum {
    KEYWORD,
    IDENTIFIER,
    NUMBER,
    OPERATOR,
    DELIMITER,
    STRING_LITERAL,
    COMMENT,
    UNKNOWN
} TokenType;

/* ================= SYMBOL TABLE ================= */

typedef struct {
    char name[50];
    int index;
} Symbol;

Symbol symbolTable[MAX_SYMBOLS];
int symbolCount = 0;

/* ================= KEYWORDS ================= */

char *keywords[] = {
    "int","float","char","double","if","else","while",
    "for","return","void","break","continue","switch",
    "case","default","do","struct","typedef","const"
};

int keywordCount = sizeof(keywords)/sizeof(keywords[0]);

/* ================= UTILITIES ================= */

int isKeyword(char *str) {
    for (int i = 0; i < keywordCount; i++)
        if (strcmp(str, keywords[i]) == 0)
            return 1;
    return 0;
}

int addSymbol(char *name) {
    for (int i = 0; i < symbolCount; i++)
        if (strcmp(symbolTable[i].name, name) == 0)
            return symbolTable[i].index;

    strcpy(symbolTable[symbolCount].name, name);
    symbolTable[symbolCount].index = symbolCount;
    return symbolCount++;
}

/* ================= TOKEN PRINTER ================= */

void printToken(TokenType type, char *value, int line) {
    char *typeStr[] = {
        "KEYWORD","IDENTIFIER","NUMBER","OPERATOR",
        "DELIMITER","STRING","COMMENT","UNKNOWN"
    };

    printf("[Line %d] %-12s : %s\n", line, typeStr[type], value);
}

/* ================= LEXICAL ANALYZER ================= */

void lexicalAnalyze(FILE *fp) {
    char ch, buffer[MAX_TOKEN];
    int i = 0, line = 1;

    while ((ch = fgetc(fp)) != EOF) {

        /* ---------- LINE COUNT ---------- */
        if (ch == '\n') {
            line++;
            continue;
        }

        /* ---------- SKIP WHITESPACE ---------- */
        if (isspace(ch)) continue;

        /* ---------- IDENTIFIER / KEYWORD ---------- */
        if (isalpha(ch) || ch == '_') {
            buffer[i++] = ch;

            while (isalnum(ch = fgetc(fp)) || ch == '_')
                buffer[i++] = ch;

            buffer[i] = '\0';
            ungetc(ch, fp);

            if (isKeyword(buffer))
                printToken(KEYWORD, buffer, line);
            else {
                printToken(IDENTIFIER, buffer, line);
                addSymbol(buffer);
            }

            i = 0;
        }

        /* ---------- NUMBER ---------- */
        else if (isdigit(ch)) {
            buffer[i++] = ch;

            while (isdigit(ch = fgetc(fp)) || ch == '.')
                buffer[i++] = ch;

            buffer[i] = '\0';
            ungetc(ch, fp);

            printToken(NUMBER, buffer, line);
            i = 0;
        }

        /* ---------- STRING LITERAL ---------- */
        else if (ch == '"') {
            buffer[i++] = ch;

            while ((ch = fgetc(fp)) != '"' && ch != EOF)
                buffer[i++] = ch;

            buffer[i++] = '"';
            buffer[i] = '\0';

            printToken(STRING_LITERAL, buffer, line);
            i = 0;
        }

        /* ---------- COMMENTS ---------- */
        else if (ch == '/') {
            char next = fgetc(fp);

            /* Single line */
            if (next == '/') {
                while ((ch = fgetc(fp)) != '\n' && ch != EOF);
                line++;
            }

            /* Multi line */
            else if (next == '*') {
                char prev = 0;
                while ((ch = fgetc(fp)) != EOF) {
                    if (ch == '\n') line++;
                    if (prev == '*' && ch == '/') break;
                    prev = ch;
                }
            }

            else {
                ungetc(next, fp);
                printToken(OPERATOR, "/", line);
            }
        }

        /* ---------- OPERATORS ---------- */
        else if (strchr("+-*=<>!&|", ch)) {
            buffer[0] = ch;
            buffer[1] = '\0';

            char next = fgetc(fp);

            /* Double operators like ==, <=, && */
            if (strchr("=<>!&|", next)) {
                buffer[1] = next;
                buffer[2] = '\0';
            } else {
                ungetc(next, fp);
            }

            printToken(OPERATOR, buffer, line);
        }

        /* ---------- DELIMITERS ---------- */
        else if (strchr("();{},[]", ch)) {
            buffer[0] = ch;
            buffer[1] = '\0';
            printToken(DELIMITER, buffer, line);
        }

        /* ---------- UNKNOWN ---------- */
        else {
            buffer[0] = ch;
            buffer[1] = '\0';
            printToken(UNKNOWN, buffer, line);
        }
    }
}

/* ================= SYMBOL TABLE DISPLAY ================= */

void printSymbolTable() {
    printf("\n===== SYMBOL TABLE =====\n");
    printf("Index\tIdentifier\n");
    for (int i = 0; i < symbolCount; i++)
        printf("%d\t%s\n", symbolTable[i].index, symbolTable[i].name);
}

/* ================= MAIN ================= */

int main() {
    FILE *fp;

    printf("Enter C source file name: ");
    char filename[100];
    scanf("%s", filename);

    fp = fopen(filename, "r");
    if (!fp) {
        printf("Error opening file.\n");
        return 1;
    }

    printf("\n===== LEXICAL ANALYSIS START =====\n\n");

    lexicalAnalyze(fp);

    fclose(fp);

    printSymbolTable();

    printf("\n===== ANALYSIS COMPLETE =====\n");

    return 0;
}