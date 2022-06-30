#include "funcs.h"

void input(char *str, int size) {
    char c = ' ';
    int i = 0;
    while (c != '\n' && i < size-1) {
        scanf("%c", &c);
        if (c != '\n') str[i++] = c;
    }
    str[i] = '\0';
}

int is_double(char *str) {
    int i = 0;
    int res = 1;
    if (str[0] == '-') {
        i++;
    }
    if (str[i] == '\0') res = 0;
    while (str[i] != '\0' && str[i] != '.') {
        if (str[i] < '0' || str[i] > '9') {
            res = 0;
        }
        i++;
    }
    if (str[i] == '.') {
        i++;
        while (str[i] != '\0') {
            if (str[i] < '0' || str[i] > '9') {
                res = 0;
            }
            i++;
        }
    }
    return res;
}

int double_start(char *str) {
    int i = 0;
    int res = 1;
    if (str[0] == '-') {
        i++;
    }
    if (str[i] < '0' || str[i] > '9') {
        if (str[i] == '.') {
            if (str[i] < '0' || str[i] > '9') {
                res = 0;
            }
        } else {
            res = 0;
        }
    }
    return res;
}

void put_graph(double *values, int **a) {
    double y1, y2;
    for (int i = 0; i < fieldy; i++) {
        for (int j = 0; j < fieldx; j++) {
            y1 = i * ydiff - 1;
            y2 = (i + 1) * ydiff - 1;
            if (values[j] >= y1 && values[j] <= y2) {
                a[i][j] = 1;
            } else {
                a[i][j] = 0;
            }
        }
    }
//    int yy1, yy2;
//    for (int j = 0; j < fieldx-1; j++) {
//        yy1 = -1;
//        yy2 = -1;
//        for (int i = 0; i < fieldy; i++) {
//            if (a[i][j]) {
//                yy1 = i;
//                break;
//            }
//        }
//        for (int i = 0; i < fieldy; i++) {
//            if (a[i][j+1]) {
//                yy2 = i;
//                break;
//            }
//        }
//        if (yy1 > -1 && yy2 > -1) {
//            if (yy2 > yy1) {
//                for (int i = yy1 + 1; i < yy1 + (yy2 - yy1) / 2; i++) {
//                    a[i][j] = 1;
//                }
//                for (int i = yy1 + (yy2 - yy1) / 2; i < yy2; i++) {
//                    a[i][j + 1] = 1;
//                }
//            } else {
//                for (int i = yy1 + 1; i < yy2 + (yy1 - yy2) / 2; i++) {
//                    a[i][j+1] = 1;
//                }
//                for (int i = yy2 + (yy1 - yy2) / 2; i < yy2; i++) {
//                    a[i][j] = 1;
//                }
//            }
//        }
//    }
}

char *format(char *a) {
    char *res;
    if (!strcmp(a, "sin(")) res = "s";
    else if (!strcmp(a, "cos(")) res = "c";
    else if (!strcmp(a, "tan(")) res = "t";
    else if (!strcmp(a, "ctg(")) res = "g";
    else if (!strcmp(a, "sqrt(")) res = "r";
    else if (!strcmp(a, "ln(")) res = "l";
    else if (!strcmp(a, "pi")) res = "p";
    else
        res = a;
    return res;
}

int is_bracket_sign(char a) {
    int res;
    switch (a) {
        case 's':
            res = 1; break;
        case 'c':
            res = 1; break;
        case 't':
            res = 1; break;
        case 'g':
            res = 1; break;
        case 'r':
            res = 1; break;
        case 'l':
            res = 1; break;
        case '(':
            res = 1; break;
        default:
            res = 0; break;
    }
    return res;
}

int priority(char a) {
    int res;
    switch (a) {
        case '0':
            res = 0; break;
        case 's':
            res = 0; break;
        case 'c':
            res = 0; break;
        case 't':
            res = 0; break;
        case 'g':
            res = 0; break;
        case 'r':
            res = 0; break;
        case 'l':
            res = 0; break;
        case '(':
            res = 0; break;
        case '+':
            res = 1; break;
        case '-':
            res = 1; break;
        case '*':
            res = 2; break;
        case '/':
            res = 2; break;
        case '^':
            res = 3; break;
        default:
            res = 5; break;
    }
    return res;
}

int decide(stack_char *s, char a) {
    int res;
    char b = s->size ? peekc(s) : '0';
    if (a == ')') {
        if (is_bracket_sign(b)) {
            res = 3;
        } else if (b == '0') {
            res = 5;
        } else {
            res = 2;
        }
    } else {
        int dca = priority(a);
        int dcb = priority(b);
        if (dcb == 0 || dca == 0 || dca > dcb) {
            res = 1;
        } else {
            res = 2;
        }
    }
    return res;
}

char *calc_switch(stack_double *s, char *p, int *breakFlag) {
    double a, b;
    switch (p[0]) {
        case ' ':
            p++;
            break;
        case '+':
            b = popd(s);
            a = popd(s);
            pushd(s, a+b);
            p++;
            break;
        case '-':
            b = popd(s);
            a = popd(s);
            pushd(s, a-b);
            p++;
            break;
        case '*':
            b = popd(s);
            a = popd(s);
            pushd(s, a*b);
            p++;
            break;
        case '/':
            b = popd(s);
            a = popd(s);
            if (b == 0) *breakFlag = 1;
            else
                pushd(s, a/b);
            p++;
            break;
        case '^':
            b = popd(s);
            a = popd(s);
            if (a == 0 && b == 0) *breakFlag = 1;
            else
                pushd(s, pow(a, b));
            p++;
            break;
        case 's':
            pushd(s, sin(popd(s)));
            p++;
            break;
        case 'c':
            pushd(s, cos(popd(s)));
            p++;
            break;
        case 't':
            a = popd(s);
            if (cos(a) == 0) *breakFlag = 1;
            else
                pushd(s, tan(a));
            p++;
            break;
        case 'g':
            a = popd(s);
            if (sin(a) == 0 || cos(a) == 0) *breakFlag = 1;
            else
                pushd(s, 1/tan(a));
            p++;
            break;
        case 'r':
            a = popd(s);
            if (a < 0) *breakFlag = 1;
            else
                pushd(s, sqrt(a));
            p++;
            break;
        case 'l':
            a = popd(s);
            if (a <= 0) *breakFlag = 1;
            else
                pushd(s, sqrt(a));
            p++;
            break;
        default:
            break;
    }
    return p;
}

char *polish(char **a, char*pol) {
    stack_char *s = (stack_char *) malloc(sizeof(stack_char));
    s->size = 0;
    int i = 0;
    char sign;
    char sign_str[2] = {' ', '\0'};
    while (i < MAX_SIZE && strcmp(a[i], "~")) {
        if (is_double(a[i]) || !strcmp(a[i], "x") || !strcmp(a[i], "p")) {
            strcat(pol, a[i]);
            strcat(pol, " ");
            i++;
        } else {
            sign = a[i][0];
            switch (decide(s, sign)) {
                case 1:
                    pushc(s, sign);
                    i++;
                    break;
                case 2:
                    sign_str[0] = popc(s);
                    strcat(pol, sign_str);
                    break;
                case 3:
                    sign = popc(s);
                    if (sign != '(') {
                        sign_str[0] = sign;
                        strcat(pol, sign_str);
                    }
                    i++;
                    break;
                default:
                    break;
            }
        }
    }
    int n = s->size;
    for (i = 0; i < n; i++) {
        sign_str[0] = popc(s);
        strcat(pol, sign_str);
    }
    free(s);
    return pol;
}

double *calculate(char *pol) {
    stack_double *s = (stack_double *) malloc(sizeof(stack_double));
    double *values = (double *) malloc(fieldx * sizeof(double));
    double x;
    char *p;
    int breakFlag;
    s->size = 0;
    for (int i = 0; i < fieldx; i++) {
        x = xdiff*i;
        p = pol;
        breakFlag = 0;
        while (*p != '\0') {
            if (double_start(p)) {
                pushd(s, strtof(p, &p));
            } else if (p[0] == 'x') {
                pushd(s, x);
                p++;
            } else if (p[0] == 'p') {
                pushd(s, M_PI);
                p++;
            } else {
                p = calc_switch(s, p, &breakFlag);
            }
            if (breakFlag) break;
        }
        if (breakFlag) values[i] = 1e10;
        else
            values[i] = popd(s);
    }
    free(s);
    return values;
}
