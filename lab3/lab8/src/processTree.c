#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "processTree.h"
#include "tree.h"
#include "token.h"
#include "stackTree.h"

// Классификация множителей: числа — в nums, всё остальное — в others
static void classify(Tree root, StackTree nums, StackTree others) {
    if (!root) return;
    if (root->type == NUM) {
        pushStackTree(nums, root);
    } else if (root->type == VAR) {
        pushStackTree(others, root);
    } else {
        classify(root->left,  nums, others);
        classify(root->right, nums, others);
    }
}

// Строим дерево ассоциативного произведения из оставшихся factors в others и числового множителя factor
static Tree buildProduct(StackTree others, int factor) {
    Tree prod = NULL;
    // собираем дерево из прочих множителей
    while (!emptyStackTree(others)) {
        Tree t = topStackTree(others);
        popStackTree(others);
        if (!prod) {
            prod = t;
        } else {
            Tree node = createNode(OP, "*");
            node->left  = prod;
            node->right = t;
            prod = node;
        }
    }
    // если числовой множитель не равен 1, добавляем его
    if (factor != 1) {
        char buf[16];
        sprintf(buf, "%d", factor);
        Tree fnode = createNode(NUM, buf);
        if (!prod) return fnode;
        Tree node = createNode(OP, "*");
        node->left  = fnode;
        node->right = prod;
        prod = node;
    }
    // если ничего не осталось — возвращаем 1
    if (!prod) {
        return createNode(NUM, "1");
    }
    return prod;
}

// Упрощение одного узла: свёртка +/-, дистрибуция и упрощение *
static Tree simplifyNode(Tree root) {
    if (!root || root->type != OP) 
        return root;

    char op = root->content[0];
    Tree L = root->left, R = root->right;

    // 1) Свертка констант в + и -
    if ((op == '+' || op == '-') && L && R 
        && L->type == NUM && R->type == NUM) {
        int a = atoi(L->content);
        int b = atoi(R->content);
        int c = (op == '+') ? (a + b) : (a - b);
        char buf[16];
        sprintf(buf, "%d", c);
        deleteTree(L);
        deleteTree(R);
        free(root);
        return createNode(NUM, buf);
    }

    // 2) Дистрибуция VAR*(A-B) и (A-B)*VAR
    if (op == '*') {
        // x * (A - B)
        if (L && L->type == VAR && R && R->type == OP && R->content[0] == '-') {
            Tree A = R->left;
            Tree B = R->right;
            // x*A
            Tree m1 = createNode(OP, "*");
            m1->left  = createNode(VAR, L->content);
            m1->right = processTree(A);
            // x*B
            Tree m2 = createNode(OP, "*");
            m2->left  = createNode(VAR, L->content);
            m2->right = processTree(B);
            // (x*A)-(x*B)
            Tree d = createNode(OP, "-");
            d->left  = processTree(m1);
            d->right = processTree(m2);
            free(R);
            free(root);
            return d;
        }
        // (A - B) * x
        if (R && R->type == VAR && L && L->type == OP && L->content[0] == '-') {
            Tree A = L->left;
            Tree B = L->right;
            Tree m1 = createNode(OP, "*");
            m1->left  = processTree(A);
            m1->right = createNode(VAR, R->content);
            Tree m2 = createNode(OP, "*");
            m2->left  = processTree(B);
            m2->right = createNode(VAR, R->content);
            Tree d = createNode(OP, "-");
            d->left  = processTree(m1);
            d->right = processTree(m2);
            free(L);
            free(root);
            return d;
        }
    }

    // 3) Упрощение умножения: свёртка констант, удаление 0 и 1
    if (op == '*') {
        StackTree nums   = createStackTree();
        StackTree others = createStackTree();
        classify(root, nums, others);

        int factor = 1;
        bool hasZero = false;
        // перемножаем константы
        while (!emptyStackTree(nums)) {
            Tree t = topStackTree(nums);
            popStackTree(nums);
            int v = atoi(t->content);
            if (v == 0) hasZero = true;
            else factor *= v;
            deleteTree(t);
        }
        destroyStackTree(nums);

        // если был ноль — всё подмножается в 0
        if (hasZero) {
            deleteTree(L);
            deleteTree(R);
            free(root);
            destroyStackTree(others);
            return createNode(NUM, "0");
        }

        // собираем оставшиеся множители + числовой factor
        Tree prod = buildProduct(others, factor);
        destroyStackTree(others);
        free(root);
        return prod;
    }

    // ничего не поменялось
    return root;
}

Tree processTree(Tree root) {
    if (!root) return NULL;
    // сначала рекурсивно упрощаем детей
    root->left  = processTree(root->left);
    root->right = processTree(root->right);
    // затем пытаемся упростить текущий узел
    return simplifyNode(root);
}
