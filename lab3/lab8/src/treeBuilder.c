// File: src/treeBuilder.c
#include "treeBuilder.h"
#include "operations.h"
#include <stdlib.h>

// Перевод инфиксной очереди токенов в постфиксную (алгоритм Шантинга-Ярдена)
Queue toPostfix(Queue infix) {
    Queue output = createQueue();
    Stack ops = createStack();
    while (!emptyQueue(infix)) {
        Token t = frontQueue(infix);
        popQueue(infix);
        switch (t->type) {
            case NUM:
            case VAR:
                pushQueue(output, t);
                break;
            case OP: {
                char cur = t->content[0];
                // пока на вершине стека оператор с >= приоритетом
                while (!emptyStack(ops) && topStack(ops)->type == OP) {
                    char topc = topStack(ops)->content[0];
                    if (opPriority(topc) >= opPriority(cur)) {
                        pushQueue(output, topStack(ops));
                        popStack(ops);
                    } else break;
                }
                pushStack(ops, t);
                break;
            }
            case BR_OPEN:
                pushStack(ops, t);
                break;
            case BR_CLOSE:
                // сбрасываем до открывающей скобки
                while (!emptyStack(ops) && topStack(ops)->type != BR_OPEN) {
                    pushQueue(output, topStack(ops));
                    popStack(ops);
                }
                if (!emptyStack(ops) && topStack(ops)->type == BR_OPEN) {
                    deleteToken(topStack(ops));
                    popStack(ops);
                }
                deleteToken(t);
                break;
            default:
                // прочие токены отбрасываем
                deleteToken(t);
        }
    }
    // оставшиеся операторы
    while (!emptyStack(ops)) {
        Token u = topStack(ops);
        popStack(ops);
        if (u->type != BR_OPEN) pushQueue(output, u);
        else deleteToken(u);
    }
    destroyStack(ops);
    destroyQueue(infix);
    return output;
}

// Построение дерева из постфиксной очереди
Tree buildTree(Queue infix) {
    Queue postfix = toPostfix(infix);
    StackTree st = createStackTree();
    while (!emptyQueue(postfix)) {
        Token t = frontQueue(postfix);
        popQueue(postfix);
        if (t->type == NUM || t->type == VAR) {
            Tree leaf = createNode(t->type, t->content);
            pushStackTree(st, leaf);
            deleteToken(t);
        }
        else if (t->type == OP) {
            // два операнда
            Tree right = topStackTree(st); popStackTree(st);
            Tree left  = topStackTree(st); popStackTree(st);
            Tree node  = createNode(OP, t->content);
            node->left  = left;
            node->right = right;
            pushStackTree(st, node);
            deleteToken(t);
        }
        else {
            deleteToken(t);
        }
    }
    destroyQueue(postfix);
    Tree result = NULL;
    if (!emptyStackTree(st)) {
        result = topStackTree(st);
        popStackTree(st);
    }
    destroyStackTree(st);
    return result;
}
