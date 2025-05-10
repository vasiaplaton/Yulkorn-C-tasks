#include <stdio.h>
#include "lexer.h"
#include "treeBuilder.h"
#include "processTree.h"

void printTree(Tree tree, int offset) {
    if (tree == NULL) return;

    printTree(tree->right, offset + 1);

    for (int i = 0; i < offset; i++) printf("  ");
    printf("%s\n", tree->content);

    printTree(tree->left, offset + 1);
}

void printTokens(Queue q) {
    printf("TOKENIZED:");
    while (!emptyQueue(q)) {
        Token t = frontQueue(q);
        if (!t) break;
        printf(" %s", t->content);
        popQueue(q);
    }
    printf("\n");
}

int main() {
    char expr[64];
    fgets(expr, sizeof(expr), stdin);
    printf("INPUT: %s\n", expr);

    Queue infix = tokenize(expr);
    //printf("TOKENIZED: ");
    //printTokens(infix); 

    printf("\n");
    printf("BUILDING TREE: \n");
    Tree tree = buildTree(infix);

    printTree(tree, 0);
    printf("\n");
    printf("PROCESSING TREE: \n");

    tree = processTree(tree);

    printf("PROCESSED: \n");

    printTree(tree, 0);

    printExpression(tree);
    printf("\n");

    deleteTree(tree);
    
    return 0;
}
