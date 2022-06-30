#ifndef SRC_STACK_H_
#define SRC_STACK_H_

#define MAX_SIZE 200
#include <stdio.h>

typedef struct stack_char_tag {
    char data[MAX_SIZE];
    size_t size;
} stack_char;

int pushc(stack_char *s, char value);
char popc(stack_char *s);
char peekc(stack_char *s);

typedef struct stack_double_tag {
    double data[MAX_SIZE];
    size_t size;
} stack_double;

int pushd(stack_double *s, double value);
double popd(stack_double *s);
double peekd(stack_double *s);

#endif  // SRC_STACK_H_
