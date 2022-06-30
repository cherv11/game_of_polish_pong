#include "parse.h"
#include "funcs.h"

char **parse(char *func);
int isOneChar(char ch);
int isFunction(char ch);
int isNumber(char ch);
int isUnaryMinus(char ch, char chPrev);
int isPi(char ch);

char **parse(char *func) {
    int i = 0, token = 0;
    char **tokens = malloc(MAX_SIZE * MAX_DOUBLE_CHARS * sizeof(char) + MAX_SIZE * sizeof(char *));
    char *ptr = (char *) (tokens + MAX_SIZE);
    for (int i = 0; i < MAX_SIZE; i++) {
        tokens[i] = ptr + MAX_DOUBLE_CHARS * i;
    }
    while (func[i] != '\0') {
        if (func[i] == ' ') {
            i++;
        } else if (isUnaryMinus(func[i], i == 0 ? '\0' : func[i-1]) && !isNumber(func[i+1])) {
            tokens[token] = "-1\0";
            token++;
            tokens[token] = "*\0";
            token++;
            i++;
        } else if (isPi(func[i]) == 1) {
            tokens[token] = "pi\0";
            token++;
            i += 2;
        } else if (isFunction(func[i]) == 1) {
            for (int j = 0; j < MAX_FUNCTION_CHARS; j++) {
                tokens[token][j] = '\0';
                if (func[i] != '(') {
                    tokens[token][j] = func[i];
                    i++;
                } else if (func[i] == '(') {
                    tokens[token][j] = func[i];
                    break;
                }
            }
            token++;
            i++;
        } else if (isNumber(func[i]) == 1 || isUnaryMinus(func[i], i == 0 ? '\0' : func[i-1])) {
            for (int j = 0; j < MAX_DOUBLE_CHARS; j++) {
                if (j == 0 && isUnaryMinus(func[i], i == 0 ? '\0' : func[i-1])) {
                    tokens[token][j] = '-';
                    i++;
                } else if (isNumber(func[i]) == 1) {
                    tokens[token][j] = func[i];
                    i++;
                } else {
                    tokens[token][j] = '\0';
                }
            }
            token++;
        } else if (isOneChar(func[i]) == 1) {
            tokens[token][0] = func[i];
            tokens[token][1] = '\0';
            token++;
            i++;
        }
    }
    tokens[token] = "~";
    for (int i = 0; i < MAX_SIZE; i++) {
        if (!strcmp(tokens[i], "~")) break;
        tokens[i] = format(tokens[i]);
    }
    return tokens;
}

int isOneChar(char ch) {
    int ret = 0;
    if (ch == '-' || ch == '+' || ch == '*' || ch == '/' || ch == ')'
    || ch == 'x' || ch == '^' || ch == '(') {
        ret = 1;
    }
    return ret;
}

int isNumber(char ch) {
    int ret = 0;
    if ((ch >= 48 && ch <= 57) || ch == '.') {
        ret = 1;
    }
    return ret;
}

int isFunction(char ch) {
    int ret = 0;
    if (ch >= 97 && ch <= 122 && ch != 'x' && ch != 'p') {
        ret = 1;
    }
    return ret;
}

int isUnaryMinus(char ch, char chPrev) {
    int ret = 0;
    if (ch == '-' && (chPrev == '\0' || chPrev == '(')) {
        ret = 1;
    }
    return ret;
}

int isPi(char ch) {
    int ret = 0;
    if (ch == 'p') {
        ret = 1;
    }
    return ret;
}
