#include "stack.h"

int pushc(stack_char *s, char value) {
    int err = 0;
    if (s->size == MAX_SIZE) {
        err++;
    } else {
        s->data[s->size] = value;
        s->size++;
    }
    return err;
}

char popc(stack_char *s) {
    s->size--;
    return s->data[s->size];
}

char peekc(stack_char *s) {
    return s->data[s->size-1];
}

int pushd(stack_double *s, double value) {
    int err = 0;
    if (s->size == MAX_SIZE) {
        err++;
    } else {
        s->data[s->size] = value;
        s->size++;
    }
    return err;
}

double popd(stack_double *s) {
    s->size--;
    return s->data[s->size];
}

double peekd(stack_double *s) {
    return s->data[s->size-1];
}
