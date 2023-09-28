#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define X_SIZE 15
#define Y_SIZE 30

typedef struct snake {
    int x;
    int y;
    int x_dir;
    int y_dir;
    int len;
} SNAKE;

void next_eat (int campo[X_SIZE][Y_SIZE]) {
    int found_eat = 0;
    for (int i = 0; i < X_SIZE; i++) {
        for (int j = 0; j < Y_SIZE; j++) {
            if (campo[i][j] == 3) {
                found_eat++;
                break;
            }
        }
        if (found_eat > 0) {
            break;
        }
    }

    if (found_eat == 0) {
        int found = 1;
        do {
            int x_rnd = rand() % X_SIZE;
            int y_rnd = rand() % Y_SIZE;

            if (campo[x_rnd][y_rnd] == 0) {
                campo[x_rnd][y_rnd] = 3;
                found = 0;
            }
        } while (found != 0);
    }
}

int next_move (int campo[X_SIZE][Y_SIZE], SNAKE *h) {
    int old_x = h->x;
    int old_y = h->y;

    h->x += h->x_dir;
    h->y += h->y_dir;

    if (h->x > X_SIZE-1 || h->y > Y_SIZE-1 || h->x < 0 || h->y < 0) {
        return -1;
    }

    // check eat here;
    if (campo[h->x][h->y] == 3) {
        h->len++;
    }

    campo[h->x][h->y] = 1;
    campo[old_x][old_y] = 0;

    next_eat (campo);
    return 0;
}

int go_up (int campo[X_SIZE][Y_SIZE], SNAKE *h) {

    if (h->x_dir == -1 && h->y_dir == 0) {
        return next_move (campo, h);
    }

    if ( (h->x_dir == 0 && h->y_dir == 1) || (h->x_dir == 0 && h->y_dir == -1)) {
        h->x_dir = -1;
        h->y_dir = 0;

        return next_move (campo, h);
    }

    return 0;
}

int go_down (int campo[X_SIZE][Y_SIZE], SNAKE *h) {

    if (h->x_dir == 1 && h->y_dir == 0) {
        return next_move (campo, h);
    }

    if ( (h->x_dir == 0 && h->y_dir == 1) || (h->x_dir == 0 && h->y_dir == -1)) {
        h->x_dir = +1;
        h->y_dir = 0;

        return next_move (campo, h);
    }

    return 0;
}

int go_left (int campo[X_SIZE][Y_SIZE], SNAKE *h) {

    if (h->x_dir == 0 && h->y_dir == -1) {
        return next_move(campo, h);
    }

    if ( (h->x_dir == 1 && h->y_dir == 0) || (h->x_dir == -1 && h->y_dir == 0)) {
        h->x_dir = 0;
        h->y_dir = -1;

        return next_move (campo, h);
    }

    return 0;
}

int go_right (int campo[X_SIZE][Y_SIZE], SNAKE *h) {

    if (h->x_dir == 0 && h->y_dir == 1 ) {
        return next_move(campo, h);
    }

    if ( (h->x_dir == 1 && h->y_dir == 0) || (h->x_dir == -1 && h->y_dir == 0)) {
        h->x_dir = 0;
        h->y_dir = +1;

        return next_move (campo, h);
    }

    return 0;
}

void init (int campo[X_SIZE][Y_SIZE], SNAKE *s) {
    s->x = 10;
    s->y = 10;
    s->x_dir = 0;
    s->y_dir = 1;
    s->len = 3;

    campo[s->x][s->y] = 1;
}

void print_snake (int campo[X_SIZE][Y_SIZE], SNAKE *s) {
    printf("\e[1;1H\e[2J");
    printf("\n|------------------------------|\n");
    for (int i = 0; i < X_SIZE; i++) {
        printf("|");
        for (int j = 0; j < Y_SIZE; j++) {
            if (campo[i][j] == 0) {
                printf (" ");
            } else if (campo[i][j] == 1) {
                printf("O");
            } else if (campo[i][j] == 2) {
                printf("=");
            } else if (campo[i][j] == 3) {
                printf("*");
            }
        }
        printf("|\n");
    }
    printf("|------------------------------|\n");
}

int main (){
    srand(time(NULL));

    int campo[X_SIZE][Y_SIZE];
    for (int i = 0; i < X_SIZE; i++) {
        for (int j = 0; j < Y_SIZE; j++) {
            campo[i][j] = 0;
        }
    }

    SNAKE *snake;
    snake = (SNAKE*) calloc(1, sizeof(SNAKE));

    init(campo, snake);

    int ret, c_in;
    do {
        print_snake (campo, snake);

        system ("/bin/stty raw");

        c_in = getchar();

        switch (c_in) {
            case 'A':
            case 'a':
                ret = go_up (campo, snake);
                break;
            case 'B':
            case 'b':
                ret = go_down (campo, snake);
                break;
            case 'C':
            case 'c':
                ret = go_right (campo, snake);
                break;
            case 'D':
            case 'd':
                ret = go_left (campo, snake);
                break;
            case 'Q':
            case 'q':
                ret = -2;
                return 0;
        }

        system ("/bin/stty cooked");

    } while (ret != -1 && ret != -2);

    if (ret == -1) {
        printf(" XXX Dead XXX \n");
    }

    return 0;
}


