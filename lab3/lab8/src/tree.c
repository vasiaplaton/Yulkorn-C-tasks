#include "tree.h"
#include "tree.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Tree createNode(TokenType type, char *content) {
    Tree node = malloc(sizeof(_Node));
    node->type = type;
    strcpy(node->content, content);
    node->left = node->right = NULL;
    return node;
}

void deleteTree(Tree node) {
    if (!node) return;
    deleteTree(node->left);
    deleteTree(node->right);
    free(node);
}

void printExpression(Tree node) {
    if (!node) return;
    // для операторов: (левый   оп   правый)
    if (node->type == OP) {
        printf("(");
        printExpression(node->left);
        printf("%s", node->content);
        printExpression(node->right);
        printf(")");
    } else {
        printf("%s", node->content);
    }
}
