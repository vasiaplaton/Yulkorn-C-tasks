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

int main() {
    char expr[64];
    fgets(expr, sizeof(expr), stdin);

    Queue infix = tokenize(expr);

    Tree tree = buildTree(infix);

    printTree(tree, 0);

    tree = processTree(tree);

    printf("PROCESSED: \n");

    printTree(tree, 0);

    printExpression(tree);
    printf("\n");

    deleteTree(tree);
    
    return 0;
}
