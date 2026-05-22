#include <stdio.h>
#include <string.h>

#define MAX_LEN 1024

enum TokenType {
    TOKEN_NUM,
    TOKEN_PLUS,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_END,
    TOKEN_INVALID
};

char input[MAX_LEN];
int pos = 0;
int token;
char token_text[128];

void next_token();
void parse_S(int depth);
void parse_S_prime(int depth);
void parse_E(int depth);
void print_indent(int depth);
void error();

void print_indent(int depth)
{
    int i;
    for (i = 0; i < depth; i++) {
        printf("  ");
    }
}

void error()
{
    printf("\nParse Error!\n");
    printf("Error near position: %d\n", pos);
}

void next_token()
{
    int i = 0;

    while (input[pos] == ' ' || input[pos] == '\t' || input[pos] == '\n') {
        pos++;
    }

    if (input[pos] == '\0') {
        token = TOKEN_END;
        strcpy(token_text, "EOF");
        return;
    }

    if (input[pos] >= '0' && input[pos] <= '9') {
        while (input[pos] >= '0' && input[pos] <= '9') {
            token_text[i++] = input[pos++];
        }
        token_text[i] = '\0';
        token = TOKEN_NUM;
        return;
    }

    if (input[pos] == '+') {
        token_text[0] = '+';
        token_text[1] = '\0';
        pos++;
        token = TOKEN_PLUS;
        return;
    }

    if (input[pos] == '(') {
        token_text[0] = '(';
        token_text[1] = '\0';
        pos++;
        token = TOKEN_LPAREN;
        return;
    }

    if (input[pos] == ')') {
        token_text[0] = ')';
        token_text[1] = '\0';
        pos++;
        token = TOKEN_RPAREN;
        return;
    }

    token_text[0] = input[pos];
    token_text[1] = '\0';
    pos++;
    token = TOKEN_INVALID;
}

void parse_S(int depth)
{
    print_indent(depth);
    printf("S -> E S'\n");

    if (token == TOKEN_NUM || token == TOKEN_LPAREN) {
        parse_E(depth + 1);
        parse_S_prime(depth + 1);
    } else {
        error();
    }
}

void parse_S_prime(int depth)
{
    if (token == TOKEN_PLUS) {
        print_indent(depth);
        printf("S' -> + S\n");

        next_token();

        print_indent(depth + 1);
        printf("+\n");

        parse_S(depth + 1);
    } else if (token == TOKEN_RPAREN || token == TOKEN_END) {
        print_indent(depth);
        printf("S' -> epsilon\n");
    } else {
        error();
    }
}

void parse_E(int depth)
{
    if (token == TOKEN_NUM) {
        print_indent(depth);
        printf("E -> num\n");

        print_indent(depth + 1);
        printf("%s\n", token_text);

        next_token();
    } else if (token == TOKEN_LPAREN) {
        print_indent(depth);
        printf("E -> ( S )\n");

        next_token();

        print_indent(depth + 1);
        printf("(\n");

        parse_S(depth + 1);

        if (token != TOKEN_RPAREN) {
            error();
            return;
        }

        print_indent(depth + 1);
        printf(")\n");

        next_token();
    } else {
        error();
    }
}

int main()
{
    printf("Input expression: ");
    fgets(input, MAX_LEN, stdin);

    pos = 0;
    next_token();

    printf("\nParse Tree:\n");
    printf("--------------------\n");

    parse_S(0);

    if (token == TOKEN_END) {
        printf("--------------------\n");
        printf("Parsing Success!\n");
    } else {
        printf("--------------------\n");
        printf("Parsing Failed! Extra token: %s\n", token_text);
    }

    return 0;
}
