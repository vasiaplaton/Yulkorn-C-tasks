#include "lexer.h"

typedef enum State {
    INIT,
    END,
    // ...
} State;

Queue tokenize(char *expr) {
    Queue out = createQueue();

    int i = 0;
    State st = INIT;

    char numBuf[16];
    int bufPos = 0;

    while (st != END) {
        if (st == INIT) {
            char letter = expr[i];
            
            if ('a' <= letter && letter <= 'z') {
                char c[2];
                c[0] = letter;
                c[1] = '\0';

                pushQueue(out, createToken(c, VAR));
                i++;
            }

            // ...
        } 
    }

    return out;
}