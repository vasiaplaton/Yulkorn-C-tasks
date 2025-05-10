
#include "lexer.h"
#include "token.h"
#include <ctype.h>
#include <string.h>

// Простейший лексер: числа, переменные, скобки, операторы + - * /
// Останавливается по ‘=’, ‘\n’ или концу строки
Queue tokenize(char *expr) {
    Queue out = createQueue();
    int i = 0;
    while (expr[i] != '\0' && expr[i] != '\n' && expr[i] != '=') {
        char c = expr[i];
        if (isspace((unsigned char)c)) {
            // пропускаем пробелы
            i++;
        }
        else if (isdigit((unsigned char)c) || (c == '.' && isdigit((unsigned char)expr[i+1]))) {
            // число (целое или дробное)
            char buf[16];
            int j = 0;
            while ((isdigit((unsigned char)expr[i]) || expr[i] == '.') && j < 15) {
                buf[j++] = expr[i++];
            }
            buf[j] = '\0';
            pushQueue(out, createToken(buf, NUM));
        }
        else if (isalpha((unsigned char)c)) {
            // переменная: буквы и цифры
            char buf[16];
            int j = 0;
            while ((isalpha((unsigned char)expr[i]) || isdigit((unsigned char)expr[i])) && j < 15) {
                buf[j++] = expr[i++];
            }
            buf[j] = '\0';
            pushQueue(out, createToken(buf, VAR));
        }
        else if (c == '(') {
            pushQueue(out, createToken("(", BR_OPEN));
            i++;
        }
        else if (c == ')') {
            pushQueue(out, createToken(")", BR_CLOSE));
            i++;
        }
        else if (c == '+' || c == '-' || c == '*' || c == '/') {
            char op[2] = { c, '\0' };
            pushQueue(out, createToken(op, OP));
            i++;
        }
        else {
            // игнорируем всё прочее
            i++;
        }
    }
    return out;
}
