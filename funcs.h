#ifndef SRC_FUNCS_H_
#define SRC_FUNCS_H_

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"

#define xdiff M_PI*4/159
#define ydiff 2.0/50
#define fieldx 160
#define fieldy 50
#define MAX_DOUBLE_CHARS 40
#define MAX_FUNCTION_CHARS 6

void input(char *str, int size);
int is_double(char *str);
int double_start(char *str);
void put_graph(double *values, int **a);
char *format(char *a);
int is_bracket_sign(char a);
int priority(char a);
int decide(stack_char *s, char a);
char *calc_switch(stack_double *s, char *p, int *breakFlag);
char *polish(char **a, char *pol);
double *calculate(char* pol);

#endif  // SRC_FUNCS_H_
