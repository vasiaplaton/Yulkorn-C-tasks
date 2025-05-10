#pragma once
#include "token.h"

typedef struct _Node {
    TokenType type;
    char content[16];

    struct _Node *left;
    struct _Node *right;
} _Node, *Tree;

Tree createNode(TokenType type, char *content);
void deleteTree(Tree node);
void printExpression(Tree node);

