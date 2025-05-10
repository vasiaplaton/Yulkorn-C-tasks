#include "token.h"

Token createToken(char *content, TokenType type) {
    Token res = malloc(sizeof(_Token));
    
    strcpy(res->content, content);
    res->type = type;

    return res;
}

void deleteToken(Token token) {
    free(token);
}

