#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>
#include "stack.h"
#include "funcs.h"
#include "parse.h"

int **matrix_malloc();
void fill_zeros(int **a);
void set_default(int *bx, int *by, int *bsx, int *bsy, int *apy, int *bpy);
void spawn_gliders(int **gol, int y, int x);
void collide(int *bx, int *by, int *bsx, int *bsy, int **gol, int**graph);
void move_ball(int *bx, int *by, int *bsx, int *bsy, int *apy, int *bpy, int *as, int *bs, int **gol, int**graph);
void move_graph(int **graph);
void run_game(int **a, int **b, int**graph);
void life_act(int **a, int **b, int by, int bx);
int check_neighbours(int **a, int y, int x, int by, int bx);
void get_key(int *end, int *speed, int *apy, int *bpy);
void draw_field(int apy, int bpy, int bx, int by, int as, int bs, int **gol, int**graph);
void move_paddle(int *py, int value);

int main() {
    int **a = matrix_malloc();
    if (a != NULL) {
        int **b = matrix_malloc();
        if (b != NULL) {
            // polish function
            char func[MAX_SIZE];
            input(func, MAX_SIZE);
            char **pol_str = parse(func);
            int **graph = matrix_malloc();
            char pol_arr[MAX_SIZE];
            memset(pol_arr, '\0', sizeof(pol_arr));
            char *pol = polish(pol_str, pol_arr);
            double *values = calculate(pol);
            put_graph(values, graph);
            free(values);

            initscr();
            nodelay(stdscr, 1);
            noecho();
            fill_zeros(a);

            printw("%p", freopen("/dev/tty", "r", stdin));
            nonl();
            run_game(a, b, graph);
            free(b);
            free(graph);
            endwin();
        } else {
            printw("n/a");
        }
        free(a);
    } else {
        printw("n/a");
    }
    return 0;
}

int **matrix_malloc() {
    int **p = (int **) malloc(fieldx * fieldy * sizeof(int) + fieldy * sizeof(int *));
    if (p != NULL) {
        int *ptr = (int *) (p + fieldy);
        for (int i = 0; i < fieldy; i++) {
            p[i] = ptr + fieldx * i;
        }
    }
    return p;
}

void fill_zeros(int **a) {
    for (int **p = a; p - a < fieldy; p++) {
        for (int *m = *p; m - *p < fieldx; m++) {
            *m = 0;
        }
    }
}

void set_default(int *bx, int *by, int *bsx, int *bsy, int *apy, int *bpy) {
    *bx = fieldx / 2 + (rand() % 5 - 2);
    *by = fieldy / 2 + (rand() % 5 - 2);
    *bsx = rand() % 2 ? 1 : -1;
    *bsy = rand() % 2 ? 1 : -1;
    *apy = fieldy / 2;
    *bpy = fieldy / 2;
}

void spawn_gliders(int **gol, int y, int x) {
    int a[7][7] = { {1, 1, 0, 0, 1, 1, 1},
                    {1, 0, 1, 0, 0, 0, 1},
                    {1, 0, 0, 0, 0, 1, 0},
                    {0, 0, 0, 0, 0, 0, 0},
                    {0, 1, 0, 0, 0, 0, 1},
                    {1, 0, 0, 0, 1, 0, 1},
                    {1, 1, 1, 0, 0, 1, 1},};
    int m, n;
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 7; j++) {
            if (!a[i][j]) continue;
            m = y+i;
            n = x+j;
            if (m < 0) m = fieldy + m;
            else if (m > fieldy - 1) m = m - fieldy;
            if (n < 0) n = fieldx + n;
            else if (n > fieldx - 1) n = n - fieldx;
            gol[m][n] = 1;
        }
    }
}

void collide(int *bx, int *by, int *bsx, int *bsy, int **gol, int**graph) {
    int xf = 1, yf = 1;
    if ((gol[*by+*bsy][*bx+*bsx]) || (graph[*by+*bsy][*bx+*bsx])) {
        if (graph[*by + *bsy][*bx + *bsx]) {
            spawn_gliders(gol, *by + *bsy, *bx + *bsx);
            graph[*by + *bsy][*bx + *bsx] = 0;
        }
        if (*by-*bsy < 0 || *by - *bsy > fieldy - 1) {
            yf = 0;
        } else {
            *bsy = -*bsy;
        }
        if (*bx-*bsx < 0 || *bx - *bsx > fieldx - 1) {
            xf = 0;
        } else {
            *bsx = -*bsx;
        }
    }
    if (xf) *bx += *bsx;
    if (yf) *by += *bsy;
}

void move_ball(int *bx, int *by, int *bsx, int *bsy, int *apy, int *bpy, int *as, int *bs, int **gol, int**graph) {
    if (*bx+*bsx < 1) {
        if (*by + *bsy - *apy >= -2 && *by + *bsy - *apy <= 2) {
            *bsx = -*bsx;
        } else {
            (*bs)++;
            set_default(bx, by, bsx, bsy, apy, bpy);
        }
    } else if (*bx+*bsx > fieldx - 2) {
        if (*by + *bsy - *bpy >= -2 && *by + *bsy - *bpy <= 2) {
            *bsx = -*bsx;
        } else {
            (*as)++;
            set_default(bx, by, bsx, bsy, apy, bpy);
        }
    }
    if (*by + *bsy < 0) {
        *bsy = -*bsy;
    } else if (*by + *bsy > fieldy - 1) {
        *bsy = -*bsy;
    }
    collide(bx, by, bsx, bsy, gol, graph);
}

void move_graph(int **graph) {
    int *tmp = malloc(fieldy * sizeof(int));
    for (int i = 0; i < fieldx-1; i++) {
        for (int j = 0; j < fieldy; j++) {
            tmp[i] = graph[j][i];
            graph[j][i] = graph[j][i+1];
            graph[j][i+1] = tmp[i];
        }
    }
    free(tmp);
}

void run_game(int **a, int **b, int**graph) {
    srand(time(NULL));
    int ticks = 0;
    int speed = 8;
    int end = 0;
    int apy = fieldy / 2, bpy = fieldy / 2;
    int bx = fieldx / 2 + (rand() % 5 - 2), by = fieldy / 2 + (rand() % 5 - 2);
    int bsx = rand() % 2 ? 1 : -1, bsy = rand() % 2 ? 1 : -1;
    int a_score = 0, b_score = 0;
    while (end == 0 && a_score < 21 && b_score < 21) {
        clear();
        timeout(0);
        get_key(&end, &speed, &apy, &bpy);
        if (ticks % 2 == 0) {
            move_graph(graph);
            life_act(a, b, by, bx);
            move_ball(&bx, &by, &bsx, &bsy, &apy, &bpy, &a_score, &b_score, b, graph);
            draw_field(apy, bpy, bx, by, a_score, b_score, b, graph);
        } else {
            move_graph(graph);
            life_act(b, a,  by, bx);
            move_ball(&bx, &by, &bsx, &bsy, &apy, &bpy, &a_score, &b_score, a, graph);
            draw_field(apy, bpy, bx, by, a_score, b_score, a, graph);
        }
        refresh();
        ticks++;
        usleep(500000/speed+25000);
    }
    if (a_score >= 21) {
        printw("Player 1 wins!\n");
    } else if (b_score >= 21) {
        printw("Player 2 wins!\n");
    }
}

int check_neighbours(int **a, int y, int x, int by, int bx) {
    int neighbours = 0;
    int m, n;
    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            if (i == 0 && j == 0) {
                continue;
            }
            m = y+i;
            n = x+j;
            if (m < 0) m = fieldy + m;
            else if (m > fieldy - 1) m = m - fieldy;
            if (n < 0) n = fieldx + n;
            else if (n > fieldx - 1) n = n - fieldx;
            if (a[m][n] || (m == by && n == bx)) neighbours++;
        }
    }
    return neighbours;
}

void life_act(int **a, int **b, int by, int bx) {
    int n;
    for (int i = 0; i < fieldy; i++) {
        for (int j = 0; j < fieldx; j++) {
            n = check_neighbours(a, i, j, by, bx);
            if (a[i][j] == 0) {
                if (n == 3) {
                    b[i][j] = 1;
                } else {
                    b[i][j] = 0;
                }
            } else {
                if (n < 2 || n > 3) {
                    b[i][j] = 0;
                } else {
                    b[i][j] = 1;
                }
            }
        }
    }
}

void get_key(int *end, int *speed, int *apy, int *bpy) {
    char c = getch();
    fpurge(stdin);
    if (c == '1' && *speed > 1) {
        *speed -= 1;
    } else if (c == '2' && *speed < 20) {
        *speed += 1;
    } else if (c == '0') {
        *speed = 8;
    } else if (c == 'q') {
        *end = 1;
    } else if (c == 'a' || c == 'A') {
        move_paddle(apy, -1);
    } else if (c == 'z' || c == 'Z') {
        move_paddle(apy, 1);
    } else if (c == 'k' || c == 'K') {
        move_paddle(bpy, -1);
    } else if (c == 'm' || c == 'M') {
        move_paddle(bpy, 1);
    }
}

void draw_field(int apy, int bpy, int bx, int by, int as, int bs, int **gol, int**graph) {
    for (int y = -3; y < fieldy+1; y++) {
        printw("|");
        if (y == -2) {
            int sign = fieldx / 2;
            int a_chars = 1;
            int b_chars = 1;
            if (as > 9) {
                a_chars++;
            }
            if (bs > 9) {
                b_chars++;
            }
            for (int x = 0; x < sign-a_chars; x++) {
                printw(" ");
            }
            printw("%d:%d", as, bs);
            for (int x = sign+1+b_chars; x < fieldx; x++) {
                printw(" ");
            }
            printw("|");
            printw("\n");
            continue;
        }
        for (int x = 0; x < fieldx; x++) {
            if (y == -3 || y == -1 || y == fieldy) {
                printw("-");
            } else if (x == 0 && (apy == y || apy == y-1 || apy == y+1)) {
                printw("|");
            } else if (x == fieldx-1 && (bpy == y || bpy == y-1 || bpy == y+1)) {
                printw("|");
            } else if (x == bx && y == by) {
                printw("@");
            } else if (gol[y][x]) {
                printw("#");
            } else if (graph[y][x]) {
                printw("*");
            } else {
                printw(" ");
            }
        }
        printw("|");
        printw("\n");
    }
}

void move_paddle(int *py, int value) {
    *py += value;
    if (*py < 1) *py = 1;
    if (*py > fieldy - 1) *py = fieldy - 1;
}