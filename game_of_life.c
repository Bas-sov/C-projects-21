#include <ncurses.h>
#include <stdio.h>

#define WIDTH 80
#define HEIGHT 25

void input_field(int field[HEIGHT][WIDTH]);
void draw_field(const int field[HEIGHT][WIDTH]);
int count_neighbors(const int field[HEIGHT][WIDTH], int y, int x);
void update_gen(int field[HEIGHT][WIDTH], int next[HEIGHT][WIDTH]);
void handle_input(int ch, int *ms, int *lvl, int *run);
void copy_field(int field[HEIGHT][WIDTH], int next[HEIGHT][WIDTH]);

int main() {
    int field[HEIGHT][WIDTH];
    int delay_ms = 100, speed_lvl = 5, running = 1;

    input_field(field);

    if (freopen("/dev/tty", "r", stdin) != NULL) {
        initscr();
        noecho();
        curs_set(0);
        int next[HEIGHT][WIDTH];

        while (running) {
            draw_field(field);
            timeout(delay_ms);
            int ch = getch();

            handle_input(ch, &delay_ms, &speed_lvl, &running);
            update_gen(field, next);
            copy_field(field, next);
        }
        endwin();
    }

    return 0;
}

void input_field(int field[HEIGHT][WIDTH]) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (scanf("%d", &field[i][j]) != 1) {
                field[i][j] = 0;
            }
        }
    }
}

void draw_field(const int field[HEIGHT][WIDTH]) {
    move(0, 0);
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (field[i][j] == 0)
                addch(' ');
            else if (field[i][j] == 1)
                addch('.');
            else if (field[i][j] == 2)
                addch('o');
            else
                addch('@');
        }
        addch('\n');
    }
    refresh();
}

int count_neighbors(const int field[HEIGHT][WIDTH], int y, int x) {
    int count = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i != 0 || j != 0) {
                if (field[(y + i + HEIGHT) % HEIGHT][(x + j + WIDTH) % WIDTH] > 0) {
                    count++;
                }
            }
        }
    }
    return count;
}

void update_gen(int field[HEIGHT][WIDTH], int next[HEIGHT][WIDTH]) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            int c = count_neighbors(field, i, j);
            if (field[i][j] > 0) {
                next[i][j] = (c == 2 || c == 3) ? (field[i][j] % 3) + 1 : 0;
            } else {
                next[i][j] = (c == 3) ? 1 : 0;
            }
        }
    }
}

void handle_input(int ch, int *ms, int *lvl, int *run) {
    if ((ch == 'a' || ch == 'A') && *lvl < 10) {
        *lvl += 1;
        *ms = (*ms > 20) ? *ms - 20 : 10;
    } else if ((ch == 'z' || ch == 'Z') && *lvl > 1) {
        *lvl -= 1;
        *ms += 20;
    } else if (ch == ' ') {
        *run = 0;
    }
}

void copy_field(int field[HEIGHT][WIDTH], int next[HEIGHT][WIDTH]) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            field[i][j] = next[i][j];
        }
    }
}