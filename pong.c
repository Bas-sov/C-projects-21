#include <stdio.h>

#define BORDER_UP 1
#define BORDER_DOWN 25
#define BORDER_R 80
#define BORDER_L 1

void draw_field(int paddle_x_1, int paddle_y_1, int paddle_x_2, int paddle_y_2,
                int pong_y, int pong_x);
int pong_physics(int cur_x, int cur_y, int prev_x, int prev_y, int p1, int p2);
int get_next_pos_x(int cur_x, int vel_code);
int get_next_pos_y(int cur_y, int vel_code);
int pong_direction(int last_winner);
int update_score(int pong_x);
int is_move_key(char ch);

int main() {
  int middle_vert = (BORDER_DOWN + BORDER_UP) / 2,
      middle_horiz = (BORDER_L + BORDER_R) / 2;
  int p1_y = middle_vert - 1, p2_y = middle_vert - 1, p1_x = BORDER_L + 2,
      p2_x = BORDER_R - 2, pong_y = middle_vert, pong_x = middle_horiz + 1,
      score_1 = 0, score_2 = 0, last_winner = 1, vel_code,
      prev_x = pong_x + pong_direction(last_winner),
      prev_y = pong_y + pong_direction(last_winner);
  while (score_1 < 21 && score_2 < 21) {
    printf("\033[H\033[J");
    draw_field(p1_x, p1_y, p2_x, p2_y, pong_y, pong_x);
    printf("\n%40d | %d\n", score_1, score_2);
    char command = getchar();
    if ((command == 'a' || command == 'A') && p1_y > BORDER_UP) p1_y--;
    if ((command == 'z' || command == 'Z') && p1_y < BORDER_DOWN - 2) p1_y++;
    if ((command == 'k' || command == 'K') && p2_y > BORDER_UP) p2_y--;
    if ((command == 'm' || command == 'M') && p2_y < BORDER_DOWN - 2) p2_y++;
    if (is_move_key(command)) {
      vel_code = pong_physics(pong_x, pong_y, prev_x, prev_y, p1_y, p2_y);
      prev_x = pong_x;
      prev_y = pong_y;
      pong_x = get_next_pos_x(prev_x, vel_code);
      pong_y = get_next_pos_y(prev_y, vel_code);
    }
    while ((command = getchar()) && command != '\n') {
    }
    if (update_score(pong_x) == 1) {
      score_1 += 1;
      pong_x = middle_horiz + 1;
      pong_y = middle_vert;
    } else if (update_score(pong_x) == 2) {
      score_2 += 1;
      pong_x = middle_horiz + 1;
      pong_y = middle_vert;
    } else {
    }
  }
  printf("\033[H\033[J");
  draw_field(p1_x, p1_y, p2_x, p2_y, pong_y, pong_x);
  printf("\n%35d | %-35d\n", score_1, score_2);
  printf("\t\t\t\tPlayer %d won!", (score_1 >= 21) ? 1 : 2);
  return 0;
}

int is_move_key(char ch) {
  return (ch == 'a' || ch == 'A' || ch == 'z' || ch == 'Z' || ch == 'k' ||
          ch == 'K' || ch == 'm' || ch == 'M' || ch == ' ');
}

void draw_field(int paddle_x_1, int paddle_y_1, int paddle_x_2, int paddle_y_2,
                int pong_y, int pong_x) {
  for (int y = 0; y <= BORDER_DOWN + 1; y++) {
    for (int x = 0; x <= BORDER_R + 1; x++) {
      if (x == pong_x && y == pong_y) {
        printf("o");
      } else if ((x == paddle_x_1 && y >= paddle_y_1 && y < paddle_y_1 + 3) ||
                (x == paddle_x_2 && y >= paddle_y_2 && y < paddle_y_2 + 3)) {
        printf("|");
      } else if (((y > 0) && (y < 26)) && (x == 41)) {
        printf("|");
      } else if ((y == 0) || (y == 26)) {
        if ((x == 0) || (x == 81)) {
          printf("+");
        } else {
          printf("=");
        }
      } else if ((x == 0) || (x == 81)) {
        printf("|");
      } else {
        printf(" ");
      }
    }
    printf("\n");
  }
}

int pong_physics(int cur_x, int cur_y, int prev_x, int prev_y, int p1, int p2) {
  int vel_code = 1;
  int vel_x, vel_y;
  vel_x = cur_x - prev_x;
  vel_y = cur_y - prev_y;

  if (cur_y + vel_y < BORDER_UP || cur_y + vel_y > BORDER_DOWN) {
    vel_y = vel_y * (-1);
  }
  if ((cur_x == BORDER_L + 3 && cur_y >= p1 && cur_y <= (p1 + 2)) ||
      (cur_x == BORDER_R - 3 && cur_y >= p2 && cur_y <= (p2 + 2))) {
    vel_x = vel_x * (-1);
  }
  if (vel_x > 0) {
    vel_code = vel_code + 2;
  }
  if (vel_y > 0) {
    vel_code = vel_code + 6;
  }
  return vel_code;
}

int get_next_pos_x(int cur_x, int vel_code) {
  int vel_x = -1;
  if (vel_code == 3 || vel_code == 9) {
    vel_x = 1;
  }
  return cur_x + vel_x;
}

int get_next_pos_y(int cur_y, int vel_code) {
  int vel_y = -1;
  if (vel_code == 7 || vel_code == 9) {
    vel_y = 1;
  }
  return cur_y + vel_y;
}

int pong_direction(int last_winner) {
  int next_dir = -1;
  if (last_winner == 2) {
    next_dir = 1;
  }
  return next_dir;
}

int update_score(int pong_x) {
  int result = 0;
  if (pong_x > BORDER_R) {
    result = 1;
  } else if (pong_x < BORDER_L) {
    result = 2;
  } else {
    result = 0;
  }
  return result;
}

