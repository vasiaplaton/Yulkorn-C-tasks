## 1. Список файлов и краткое содержание

* **Makefile** – правила сборки: компиляция модулей и линковка в `app.out`.
* **src/main.c** – точка входа, чтение выражения, вывод токенов, построение и печать дерева, вызов `processTree`.
* **src/lexer.h / src/lexer.c** – лексер: разбивает строку на токены (числа, переменные, скобки, операторы).
* **src/token.h / src/token.c** – структура токена: создание (`createToken`), удаление (`deleteToken`).
* **src/tree.h / src/tree.c** – узел синтаксического дерева: создание (`createNode`), удаление (`deleteTree`), печать выражения (`printExpression`).
* **src/treeBuilder.h / src/treeBuilder.c** –

  * `toPostfix`: перевод инфиксной очереди в постфиксную (алг. Шантинга–Ярдена).
  * `buildTree`: построение дерева из постфиксной очереди.
* **src/processTree.h / src/processTree.c** – оптимизация дерева:

  1. свёртка констант в сложении/вычитании;
  2. дистрибуция `VAR*(A–B)` и `(A–B)*VAR`;
  3. упрощение умножения (свёртка чисел, удаление множителя 1, обработка нуля).
* **src/operations.h / src/operations.c** – приоритеты и коммутативность операций (`opPriority`, `opCommut`).
* **src/queue.h / src/queue.c** – реализация кольцевой очереди для токенов.
* **src/stack.h / src/stack.c** – реализация динамического стека для токенов.
* **src/stackTree.h / src/stackTree.c** – аналогичный стек, но для узлов `Tree`.

---

## 2. Детальный обзор каждого файла

### Makefile

```makefile
all: build run

build: app.out

app.out: operations.o token.o tree.o queue.o stack.o stackTree.o lexer.o treeBuilder.o processTree.o main.o
	gcc operations.o token.o tree.o queue.o stack.o stackTree.o lexer.o treeBuilder.o processTree.o main.o -o app.out

operations.o: operations.c
	gcc -c operations.c -o operations.o

token.o: token.c
	gcc -c token.c -o token.o

tree.o: tree.c
	gcc -c tree.c -o tree.o

queue.o: queue.c
	gcc -c queue.c -o queue.o

stack.o: stack.c
	gcc -c stack.c -o stack.o

stackTree.o: stackTree.c
	gcc -c stackTree.c -o stackTree.o

lexer.o: lexer.c
	gcc -c lexer.c -o lexer.o

treeBuilder.o: treeBuilder.c
	gcc -c treeBuilder.c -o treeBuilder.o

processTree.o: processTree.c
	gcc -c processTree.c -o processTree.o

main.o: main.c
	gcc -c main.c -o main.o
	
run:
	./app.out

clean:
	rm *.o
```

**Зачем и что здесь сделано**

* Правила `*.o: *.c` компилируют каждый модуль в объектник.
* Цель `app.out` линкует все `.o` в исполняемый.
* `run` сразу выполняет `app.out`.
* `clean` очищает объектники.

---

### src/main.c

```c
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
    printTokens(infix);

    printf("\nBUILDING TREE:\n");
    Tree tree = buildTree(infix);
    printTree(tree, 0);

    printf("\nPROCESSING TREE:\n");
    tree = processTree(tree);
    printTree(tree, 0);

    printf("\nRESULT EXPRESSION:\n");
    printExpression(tree);
    printf("\n");

    deleteTree(tree);
    return 0;
}
```

**Объяснение по частям**

* `printTree` — красивая печать дерева «книжкой»: сначала правый, потом левый.
* `printTokens` — выводит все токены для отладки.
* В `main`:

  1. Читаем строку `expr`.
  2. Лексим в очередь `infix`.
  3. Строим дерево (`buildTree`).
  4. Печатаем до и после оптимизации (`processTree`).
  5. Конечное выражение выводим через `printExpression`.
  6. Освобождаем память `deleteTree`.

---

### src/lexer.h

```c
#pragma once
#include "queue.h"

Queue tokenize(char *expr);
```

---

### src/lexer.c

```c
#include "lexer.h"
#include "token.h"
#include <ctype.h>
#include <string.h>

// Лексер: числа, переменные (буквы+цифры), скобки, операторы + - * /
Queue tokenize(char *expr) {
    Queue out = createQueue();
    int i = 0;
    while (expr[i] != '\0' && expr[i] != '\n' && expr[i] != '=') {
        char c = expr[i];
        if (isspace((unsigned char)c)) {
            i++;
        }
        else if (isdigit((unsigned char)c) || (c == '.' && isdigit((unsigned char)expr[i+1]))) {
            char buf[16]; int j = 0;
            while ((isdigit((unsigned char)expr[i]) || expr[i] == '.') && j < 15)
                buf[j++] = expr[i++];
            buf[j] = '\0';
            pushQueue(out, createToken(buf, NUM));
        }
        else if (isalpha((unsigned char)c)) {
            char buf[16]; int j = 0;
            while ((isalpha((unsigned char)expr[i]) || isdigit((unsigned char)expr[i])) && j < 15)
                buf[j++] = expr[i++];
            buf[j] = '\0';
            pushQueue(out, createToken(buf, VAR));
        }
        else if (c == '(') {
            pushQueue(out, createToken("(", BR_OPEN)); i++;
        }
        else if (c == ')') {
            pushQueue(out, createToken(")", BR_CLOSE)); i++;
        }
        else if (c=='+'||c=='-'||c=='*'||c=='/') {
            char op[2] = {c,'\0'};
            pushQueue(out, createToken(op, OP)); i++;
        }
        else {
            i++;
        }
    }
    return out;
}
```

**Зачем и что здесь сделано**

* Пропускаем пробельные символы.
* Собираем числа (включая дробные).
* Собираем переменные из букв и цифр.
* Отдельно обрабатываем `(`, `)`, `+ - * /`.
* Игнорируем всё остальное.
* Возвращаем заполненную очередь токенов.

---

### src/token.h

```c
#pragma once
#include <stdlib.h>
#include <string.h>

typedef enum TokenType {
    NUM,
    VAR,
    BR_OPEN,
    BR_CLOSE,
    OP
} TokenType;

typedef struct _Token {
    char content[16];
    TokenType type;
} _Token, *Token;

Token createToken(char *content, TokenType type);
void deleteToken(Token token);
```

---

### src/token.c

```c
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
```

**Объяснение**

* `createToken` выделяет память, копирует строку и тип.
* `deleteToken` освобождает память.

---

### src/tree.h

```c
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
```

---

### src/tree.c

```c
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
```

**Объяснение**

* `createNode` – простой конструктор узла.
* `deleteTree` – пост-order рекурсивно освобождает все поддеревья.
* `printExpression` – инфиксная печать с кавычками для операторов.

---

### src/treeBuilder.h

```c
#pragma once
#include "token.h"
#include "tree.h"
#include "queue.h"
#include "stack.h"
#include "stackTree.h"

Tree buildTree(Queue infix);
```

---

### src/treeBuilder.c

```c
#include "treeBuilder.h"
#include "operations.h"
#include <stdlib.h>

// Алгоритм Шантинга–Ярдена: инфикс → постфикс
Queue toPostfix(Queue infix) {
    Queue output = createQueue();
    Stack ops = createStack();
    while (!emptyQueue(infix)) {
        Token t = frontQueue(infix); popQueue(infix);
        switch (t->type) {
            case NUM:
            case VAR:
                pushQueue(output, t);
                break;
            case OP: {
                char cur = t->content[0];
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
                deleteToken(t);
        }
    }
    while (!emptyStack(ops)) {
        Token u = topStack(ops); popStack(ops);
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
        Token t = frontQueue(postfix); popQueue(postfix);
        if (t->type == NUM || t->type == VAR) {
            Tree leaf = createNode(t->type, t->content);
            pushStackTree(st, leaf);
            deleteToken(t);
        }
        else if (t->type == OP) {
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
```

**Объяснение**

1. **`toPostfix`**:

   * Использует стек `ops` для операторов.
   * На вход поступают токены, на выход — очередь в постфиксном порядке.
   * Обрабатывает скобки и сравнивает приоритеты через `opPriority`.
2. **`buildTree`**:

   * Читает постфиксную очередь, для каждого числа/переменной создаёт лист.
   * Для оператора извлекает два вершины стека, создаёт новый узел и пушит его.

---

### src/processTree.h

```c
#pragma once
#include "tree.h"

Tree processTree(Tree root);
```

---

### src/processTree.c

```c
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "processTree.h"
#include "tree.h"
#include "token.h"
#include "stackTree.h"

// Упрощение узла с учётом операций и дистрибуции
static Tree simplifyNode(Tree root) {
    if (!root || root->type != OP) return root;
    char op = root->content[0];
    // свёртка констант для +/-
    if ((op=='+'||op=='-') && root->left && root->right
        && root->left->type==NUM && root->right->type==NUM) {
        int a = atoi(root->left->content);
        int b = atoi(root->right->content);
        int c = (op=='+' ? a+b : a-b);
        char buf[16]; sprintf(buf,"%d",c);
        deleteTree(root->left);
        deleteTree(root->right);
        free(root);
        return createNode(NUM, buf);
    }
    // дистрибуция VAR*(A-B) и (A-B)*VAR
    if (op=='*') {
        Tree L=root->left, R=root->right;
        if (L && L->type==VAR && R && R->type==OP && R->content[0]=='-') {
            Tree m1=createNode(OP,"*"), m2=createNode(OP,"*"), d=createNode(OP,"-");
            m1->left=createNode(VAR,L->content);
            m1->right=processTree(R->left);
            m2->left=createNode(VAR,L->content);
            m2->right=processTree(R->right);
            d->left=processTree(m1);
            d->right=processTree(m2);
            free(R); free(root);
            return d;
        }
        if (R && R->type==VAR && L && L->type==OP && L->content[0]=='-') {
            Tree m1=createNode(OP,"*"), m2=createNode(OP,"*"), d=createNode(OP,"-");
            m1->left=processTree(L->left);
            m1->right=createNode(VAR,R->content);
            m2->left=processTree(L->right);
            m2->right=createNode(VAR,R->content);
            d->left=processTree(m1);
            d->right=processTree(m2);
            free(L); free(root);
            return d;
        }
    }
    // упрощение умножения: свёртка констант, удаление 0 и 1
    if (root->content[0]=='*') {
        StackTree nums   = createStackTree();
        StackTree others = createStackTree();
        // классифицируем множители
        static void classify(Tree node, StackTree n, StackTree o) {
            if (!node) return;
            if (node->type==NUM) pushStackTree(n,node);
            else if (node->type==VAR) pushStackTree(o,node);
            else {
                classify(node->left,n,o);
                classify(node->right,n,o);
            }
        }
        classify(root, nums, others);
        int factor=1; bool hasZero=false;
        while (!emptyStackTree(nums)) {
            Tree t=topStackTree(nums); popStackTree(nums);
            int v=atoi(t->content);
            if (v==0) hasZero=true; else factor*=v;
            deleteTree(t);
        }
        destroyStackTree(nums);
        if (hasZero) {
            deleteTree(root->left);
            deleteTree(root->right);
            free(root);
            destroyStackTree(others);
            return createNode(NUM,"0");
        }
        // собираем оставшиеся + фактор
        static Tree buildProduct(StackTree o, int f) {
            Tree prod=NULL;
            while (!emptyStackTree(o)) {
                Tree t=topStackTree(o); popStackTree(o);
                if (!prod) prod=t;
                else {
                    Tree n=createNode(OP,"*");
                    n->left=prod; n->right=t; prod=n;
                }
            }
            if (f!=1) {
                char b[16]; sprintf(b,"%d",f);
                Tree fn = createNode(NUM,b);
                if (!prod) return fn;
                Tree n = createNode(OP,"*");
                n->left=fn; n->right=prod; prod=n;
            }
            if (!prod) return createNode(NUM,"1");
            return prod;
        }
        Tree result = buildProduct(others, factor);
        destroyStackTree(others);
        free(root);
        return result;
    }
    return root;
}

Tree processTree(Tree root) {
    if (!root) return NULL;
    root->left  = processTree(root->left);
    root->right = processTree(root->right);
    return simplifyNode(root);
}
```

**Зачем и что здесь сделано**

* **`processTree`** сначала рекурсивно упрощает поддеревья, затем вызывает `simplifyNode`.
* **`simplifyNode`** выполняет:

  1. **Свёртка констант** в `+`/`-`.
  2. **Дистрибуция** `VAR*(A–B)` и `(A–B)*VAR`.
  3. **Упрощение `*`**: собирает все константы (стек `nums`), перемножает, обрабатывает ноль и единицу, затем собирает прочие множители в одно дерево.

---

### src/operations.h

```c
#pragma once
#include <stdbool.h>

int opPriority(char operation);
bool opCommut(char operation);
```

---

### src/operations.c

```c
#include "operations.h"

// Приоритеты: +,- →1; *,/ →2; прочие →0
int opPriority(char operation) {
    switch (operation) {
        case '+': case '-': return 1;
        case '*': case '/': return 2;
        default:            return 0;
    }
}

// Коммутативность: +,* — true; -,/ — false
bool opCommut(char operation) {
    switch (operation) {
        case '+': case '*': return true;
        case '-': case '/': return false;
        default:            return false;
    }
}
```

---

### src/queue.h

```c
#pragma once
#include <stdbool.h>
#include <stdlib.h>
#include "token.h"

typedef Token QUEUE_T;
typedef struct Queue {
    int first, last, length, capacity;
    QUEUE_T *elems;
} _Queue, *Queue;

Queue createQueue();
void destroyQueue(Queue q);
bool emptyQueue(Queue q);
int lengthQueue(Queue q);
void pushQueue(Queue q, QUEUE_T t);
QUEUE_T frontQueue(Queue q);
void popQueue(Queue q);
```

---

### src/queue.c

```c
#include "queue.h"
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 16

Queue createQueue() {
    Queue q = malloc(sizeof(_Queue));
    q->capacity = INITIAL_CAPACITY;
    q->elems    = malloc(sizeof(QUEUE_T)*q->capacity);
    q->first=0; q->last=0; q->length=0;
    return q;
}
void destroyQueue(Queue q) { free(q->elems); free(q); }
bool emptyQueue(Queue q) { return q->length==0; }
int lengthQueue(Queue q){ return q->length; }
void pushQueue(Queue q, QUEUE_T t) {
    if (q->length==q->capacity) {
        int newCap=q->capacity*2;
        QUEUE_T *newArr=malloc(sizeof(QUEUE_T)*newCap);
        for(int i=0;i<q->length;i++)
            newArr[i]=q->elems[(q->first+i)%q->capacity];
        free(q->elems);
        q->elems=newArr;
        q->capacity=newCap; q->first=0; q->last=q->length;
    }
    q->elems[q->last]=t;
    q->last=(q->last+1)%q->capacity;
    q->length++;
}
QUEUE_T frontQueue(Queue q){ return emptyQueue(q)?NULL:q->elems[q->first]; }
void popQueue(Queue q){ if(!emptyQueue(q)){q->first=(q->first+1)%q->capacity; q->length--;} }
```

---

### src/stack.h

```c
#pragma once
#include <stdbool.h>
#include <stdlib.h>
#include "token.h"

typedef Token STACK_T;
typedef struct Stack {
    int last, capacity;
    STACK_T *elems;
} _Stack, *Stack;

Stack createStack();
void destroyStack(Stack s);
bool emptyStack(Stack s);
int depthStack(Stack s);
void pushStack(Stack s, STACK_T t);
STACK_T topStack(Stack s);
void popStack(Stack s);
```

---

### src/stack.c

```c
#include "stack.h"
#include <stdlib.h>

#define INITIAL_CAPACITY 16

Stack createStack() {
    Stack s = malloc(sizeof(_Stack));
    s->capacity=INITIAL_CAPACITY;
    s->elems   =malloc(sizeof(STACK_T)*s->capacity);
    s->last=0;
    return s;
}
void destroyStack(Stack s){ free(s->elems); free(s); }
bool emptyStack(Stack s){ return s->last==0; }
int depthStack(Stack s){ return s->last; }
void pushStack(Stack s, STACK_T t){
    if(s->last==s->capacity){
        int newCap=s->capacity*2;
        STACK_T *newArr=malloc(sizeof(STACK_T)*newCap);
        for(int i=0;i<s->capacity;i++) newArr[i]=s->elems[i];
        free(s->elems);
        s->elems=newArr;
        s->capacity=newCap;
    }
    s->elems[s->last++]=t;
}
STACK_T topStack(Stack s){ return emptyStack(s)?NULL:s->elems[s->last-1]; }
void popStack(Stack s){ if(!emptyStack(s)) s->last--; }
```

---

### src/stackTree.h

```c
#pragma once
#include <stdbool.h>
#include <stdlib.h>
#include "tree.h"

typedef Tree STACK_TREE_T;
typedef struct StackTree {
    int last, capacity;
    STACK_TREE_T *elems;
} _StackTree, *StackTree;

StackTree createStackTree();
void destroyStackTree(StackTree s);
bool emptyStackTree(StackTree s);
int depthStackTree(StackTree s);
void pushStackTree(StackTree s, STACK_TREE_T t);
STACK_TREE_T topStackTree(StackTree s);
void popStackTree(StackTree s);
```

---

### src/stackTree.c

```c
#include "stackTree.h"
#include <stdlib.h>

#define INITIAL_CAPACITY 16

StackTree createStackTree() {
    StackTree s=malloc(sizeof(_StackTree));
    s->capacity=INITIAL_CAPACITY;
    s->elems   =malloc(sizeof(STACK_TREE_T)*s->capacity);
    s->last=0;
    return s;
}
void destroyStackTree(StackTree s){ free(s->elems); free(s); }
bool emptyStackTree(StackTree s){ return s->last==0; }
int depthStackTree(StackTree s){ return s->last; }
void pushStackTree(StackTree s, STACK_TREE_T t){
    if(s->last==s->capacity){
        int newCap=s->capacity*2;
        STACK_TREE_T *newArr=malloc(sizeof(STACK_TREE_T)*newCap);
        for(int i=0;i<s->capacity;i++) newArr[i]=s->elems[i];
        free(s->elems);
        s->elems=newArr;
        s->capacity=newCap;
    }
    s->elems[s->last++]=t;
}
STACK_TREE_T topStackTree(StackTree s){ return emptyStackTree(s)?NULL:s->elems[s->last-1]; }
void popStackTree(StackTree s){ if(!emptyStackTree(s)) s->last--; }
```

---

## 3. Мега-подробный разбор `processTree`

### Цель `processTree`

Главная задача — **упростить** синтаксическое дерево арифметического выражения, применяя:

1. **Свёртку констант** при сложении/вычитании.
2. **Дистрибутивный закон** `x*(A–B) ⇒ x*A – x*B` (и аналогично справа).
3. **Упрощение произведения**:

   * собрать все числовые множители в одно число;
   * если среди них есть `0`, результат всего произведения — `0`;
   * убрать множитель `1`;
   * оставить ассоциативную цепочку оставшихся множителей.

### Алгоритм

1. **Рекурсивный шаг**

   ```c
   Tree processTree(Tree root) {
       if (!root) return NULL;
       root->left  = processTree(root->left);
       root->right = processTree(root->right);
       return simplifyNode(root);
   }
   ```

   Сначала спускаемся до листьев, упрощаем их, затем обрабатываем текущий узел через `simplifyNode`.

2. **`simplifyNode` для узлов-операторов**

   ```c
   static Tree simplifyNode(Tree root) {
       if (!root||root->type!=OP) return root;
       char op = root->content[0];
       …
       return root;
   }
   ```

3. **Свёртка констант (`+` и `-`)**

   ```c
   if ((op=='+'||op=='-') && L->type==NUM && R->type==NUM) {
       int a = atoi(L->content), b = atoi(R->content);
       int c = (op=='+'? a+b : a-b);
       // создаём новый узел NUM со значением c
   }
   ```

   * Убираем два листа-числа, заменяем их на один.

4. **Дистрибуция `*`**

   * **Случай 1:** `VAR * (A–B)`

     ```c
     if (L->type==VAR && R->type==OP && R->content[0]=='-') {
         // строим два узла x*A и x*B, затем D = (x*A)-(x*B)
     }
     ```
   * **Случай 2:** `(A–B) * VAR` (аналогично).

5. **Упрощение произведения**

   * Классификация всех вложенных множителей:

     ```c
     classify(root, nums, others);
     ```

     где `nums` — стек числовых узлов, `others` — стек прочих (переменные и поддеревья).
   * Извлекаем из `nums` все числа:

     * если встретили `0` — сразу возвращаем узел `NUM("0")`;
     * иначе перемножаем во `factor`.
   * Из `others` строим ассоциативное дерево оставшихся множителей, а затем, если `factor != 1`, приставляем числовой множитель.

6. **Итог**
   В результате на каждом узле-`OP` мы либо упрощаем его до листа-числа, либо перестраиваем поддерево согласно дистрибуции и свёртке, либо оставляем без изменений.

### Пример работы

Для выражения

```
x * (5 - a) * 3 * 2
```

1. После рекурсии мы видим узел `*` с детьми

   * левым — дерево для `x*(5-a)`,
   * правым — дерево для `* 3 2`.
2. Во вложенном поддереве `x*(5-a)` применяется дистрибуция
   ⇒ `(x*5) - (x*a)`.
3. Во внешнем узле `*` классифицируем множители:

   * `nums` = `{3,2}`,
   * `others` = `{(x*5)-(x*a)}`.
4. Получаем `factor = 3*2 = 6`,
   строим дерево `6 * ((x*5)-(x*a))`.

Таким образом, функция `processTree` последовательно применяет фундаментальные законы алгебры, приводя выражение к наиболее упрощённому виду.
