#include "processTree.h"
#include "token.h"
#include "stackTree.h"
#include <stdio.h>

void classify(Tree root, StackTree nums, StackTree other) {
    if (root->type == NUM) {
        pushStackTree(nums, root);
        return;
    }

    if (root->type == VAR) {
        pushStackTree(other, root);
        return;
    }

    // ...

    classify(root->left, nums, other);
    classify(root->right, nums, other);
}

Tree processTree(Tree root) {
    if (root->type == NUM || root->type == VAR) {
        return root;
    }

    if (root->content[0] != '*') {
        root->left = processTree(root->left);
        root->right = processTree(root->right);
        return root;
    }

    StackTree nums = createStackTree();
    StackTree other = createStackTree();

    classify(root, nums, other);

    int factor = 1;
    while (!emptyStackTree(nums)) {
        factor *= atoi(topStackTree(nums)->content);
        deleteTree(topStackTree(nums));
        popStackTree(nums);
    }
    
    Tree otherProduct = NULL;

    while (!emptyStackTree(other)) {
        Tree top = topStackTree(other);
        // тут оставлена ошибка, найдите ее
        popStackTree(other);

        if (otherProduct == NULL) {
            otherProduct = top;
        } else {
            Tree node = createNode(OP, "*");
            node->left = otherProduct;
            node->right = top;
            otherProduct = node;
        }
    }

    destroyStackTree(nums);
    destroyStackTree(other);
    
    char str[16];
    sprintf(str, "%d", factor);
    
    Tree factorNode = createNode(NUM, str);

    if (otherProduct == NULL) {
        return factorNode;
    }

    if (factor == 1) {
        return otherProduct;
    }

    Tree node = createNode(OP, "*");
    node->left = factorNode;
    node->right = otherProduct;
    
    return node;
}