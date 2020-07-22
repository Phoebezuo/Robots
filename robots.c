#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DIM (8)
#define BUF_LEN (100)
#define ST (0)
#define NT (1)
#define ET (2)
#define WT (3)

typedef struct {
    int x;
    int y;
} robot;

void print_board(char board[DIM][DIM]) {
    puts("");
    for (int i = 0; i < DIM; i++) {
        printf("|");
        for (int j = 0; j < DIM; j++) {
            printf("%c|", board[i][j]);
        }
        puts("");
    }
    puts("");
}

int human_move(int dir, int x[], int y[], char obj, char board[DIM][DIM]) {
    int horizon[4] = {0, 0, 1, -1};
    int vertical[4] = {1, -1, 0, 0};

    board[*y][*x] = ' ';
    *x += horizon[dir];
    *y += vertical[dir];
    // printf("in funtion x: %d, y: %d\n", *x, *y);
    board[*y][*x] = obj;
    return dir;
}

int robot_move(int dir, int x[], int y[], char obj, char board[DIM][DIM]) {
    int horizon[4] = {0, 0, 1, -1};
    int vertical[4] = {1, -1, 0, 0};

    board[*y][*x] = ' ';
    *x += horizon[dir];
    *y += vertical[dir];
    // printf("x: %d, y: %d\n", *x, *y);
    if (board[*y][*x] == 'R') {
        board[*y][*x] = 'D';
        return 1;
    } else {
        board[*y][*x] = obj;
        return 0;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        return 1;
    } else {
        srand(atoi(argv[1]));
    }

    while (1) {
        char board[DIM][DIM];
        memset(board, ' ', DIM * DIM);
        char buff[BUF_LEN];

        int human_x;
        int human_y;
        int exit_x;
        int exit_y;
        int robot_no;
        robot robots[BUF_LEN];

        char ipt[BUF_LEN];
        char dir[BUF_LEN];

        int flag = 0;

        printf("Human starting point: ");
        while (fgets(buff, BUF_LEN, stdin)) {
            // puts(buff);
            if (sscanf(buff, "%d %d", &human_x, &human_y) == 2
                && human_x >= 0 && human_x <= 7
                && human_y >= 0 && human_y <= 7) {
                board[human_y][human_x] = 'H';
                exit_x = rand() % 8;
                exit_y = rand() % 8;
                board[exit_y][exit_x] = 'E';
                break;
            } else {
                puts("Invalid Coordinates, Try again");
                printf("Human starting point: ");
            }
        }

        printf("Number of robots: ");
        while (fgets(buff, BUF_LEN, stdin)) {
            // puts(buff);
            if (sscanf(buff, "%d", &robot_no) == 1 && robot_no > 0) {
                int i = 0;
                while (i < robot_no) {
                    printf("Robot %d: ", i+1);
                    while (fgets(buff, BUF_LEN, stdin)) {
                        if (sscanf(buff, "%d %d", &robots[i].x, &robots[i].y) == 2 && board[robots[i].y][robots[i].x] != 'R') {
                            // puts(buff);
                            if ((abs(robots[i].x - human_x)>= 2 || abs(robots[i].y - human_y) >= 2)
                                && robots[i].x >= 0 && robots[i].x <= 7
                                && robots[i].y >= 0 && robots[i].y <= 7) {
                                board[robots[i].y][robots[i].x] = 'R';
                                break;
                            } else {
                                puts("Invalid Coordinates, Try again");
                                printf("Robot %d: ", i+1);
                            }
                        } else {
                            puts("Invalid Coordinates, Try again");
                            printf("Robot %d: ", i+1);
                        }
                    }
                    i++;
                }
                break;
            } else {
                puts("Invalid number, Try again");
                printf("Number of robots: ");
            }
        }
        puts("");

        while (fgets(buff, BUF_LEN, stdin)) {
            // puts(buff);
            if ((sscanf(buff, "%s %s", ipt, dir) == 2) && (strcmp(ipt, "move") == 0)) {
                int human_movement;
                if (strcmp(dir, "NT") == 0) {
                    human_movement = human_move(NT, &human_x, &human_y, 'H', board);
                } else if (strcmp(dir, "ST") == 0) {
                    human_movement = human_move(ST, &human_x, &human_y, 'H', board);
                } else if (strcmp(dir, "WT") == 0) {
                    human_movement = human_move(WT, &human_x, &human_y, 'H', board);
                } else if (strcmp(dir, "ET") == 0) {
                    human_movement = human_move(ET, &human_x, &human_y, 'H', board);
                } else {
                    puts("Invalid");
                }
                // printf("out of funtion human x: %d, y: %d\n", human_x, human_y);
                for (int i = 0; i < robot_no; i++) {
                    int diff_x = human_x - robots[i].x;
                    int diff_y = human_y - robots[i].y;
                    // printf("%d\n", i);
                    // printf("robot before move x: %d, y: %d\n", robots[i].x, robots[i].y);
                    int crush;
                    if (abs(diff_x) < abs(diff_y)) {
                        if (human_y < robots[i].y) {
                            crush = robot_move(NT, &robots[i].x, &robots[i].y, 'R', board);
                        } else {
                            crush = robot_move(ST, &robots[i].x, &robots[i].y, 'R', board);
                        }
                    } else if (abs(diff_x) > abs(diff_y)) {
                        if (human_x > robots[i].x) {
                            crush = robot_move(ET, &robots[i].x, &robots[i].y, 'R', board);
                        } else {
                            crush = robot_move(WT, &robots[i].x, &robots[i].y, 'R', board);
                        }
                    } else {
                        if (human_movement == NT || human_movement == ST) {
                            if (human_x < robots[i].x) {
                                crush = robot_move(WT, &robots[i].x, &robots[i].y, 'R', board);
                            } else {
                                crush = robot_move(ET, &robots[i].x, &robots[i].y, 'R', board);
                            }
                        } else if (human_movement == WT || human_movement == ET) {
                            if (human_y < robots[i].y) {
                                crush = robot_move(NT, &robots[i].x, &robots[i].y, 'R', board);
                            } else {
                                crush = robot_move(ST, &robots[i].x, &robots[i].y, 'R', board);
                            }
                        }
                    }
                    // printf("robot after move x: %d, y: %d\n", robots[i].x, robots[i].y);
                    // if (crush == 1) {
                    //     robots[i] = NULL;
                    // }

                    // printf("%d\n", i);
                    if (robots[i].x == human_x && robots[i].y == human_y) {
                        flag = 1;
                        break;
                    }
                }

                if (flag == 1) {
                    puts("\nThe robots got you! We are doomed!\n");
                    break;
                }

                if (human_x == exit_x && human_y == exit_y) {
                    puts("\nWell done! You survived, but it isn't over!\n");
                    puts("");
                    break;
                }
                puts("");
            } else if (sscanf(buff, "%s", ipt) == 1) {
                if (strcmp(ipt, "exit") == 0) {
                    return 1;
                } else if (strcmp(ipt, "display") == 0) {
                    print_board(board);
                }
            }
        }
    }

    return 0;
}
