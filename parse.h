#ifndef SRC_PARSE_H_
#define SRC_PARSE_H_

#include <string.h>
#include <stdlib.h>

char **parse(char *func);
int isOneChar(char ch);
int isFunction(char ch);
int isNumber(char ch);
int isUnaryMinus(char ch, char chPrev);
int isPi(char ch);

#endif  // SRC_PARSE_H_
