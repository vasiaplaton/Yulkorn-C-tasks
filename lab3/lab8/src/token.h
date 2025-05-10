#pragma once
#include <stdlib.h>
#include <string.h>

typedef enum TokenType {
    NUM,
    VAR,
    BR_OPEN,
    OP,
    BR_CLOSE,
    // ...
} TokenType;

typedef struct _Token {
    char content[16];
    TokenType type;
} _Token, *Token;

Token createToken(char *content, TokenType type);
void deleteToken(Token token);
