#include <curses.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define MAX_SHOTS 100

typedef struct POINT {
    int x;
    int y;
} Point;

typedef struct BOX {
    Point s;
    int radius;
} Box;

int getchar_point(Point a);
int draw_point(Point a, char c);
int draw_circle(Box a, char c, int color_pair, Box *enemy, Box *shots, int num_shots, int *hit_flag, int *enemy_hit_flag, int *scored_hit_flag, Point cat_points[], int num_cat_points);
void move_point(Point *a, int dir_y, int y_max);
void move_circle(Box *a, int dir_y, int y_max);
void printGameOver(Point rec, double clock);
void draw_cat(Point cat_points[], char cat_chars[], int num_points);

int main() {
    int x_max, y_max;
    char key;
    int t = rand() % 2, O_size = 3;
    int wait_interval = 30000;
    int wait_interval2 = 5000000;
    int red_hit_count = 0;
    int blue_hit_count = 0;
    int red_dir = 1;
    int blue_dir = 0;
    int hit_flag = 0;
    int enemy_hit_flag = 0;
    int scored_hit_flag = 0;
    int sift_cat = 0;
    int x_start = 3;
    int y_start = 6;

    char cat_chars[] = {
        '/', '\\', '_', '/',  
        '\\',        
        '(',' ', 'o', '.',   
        'o', ' ',')',          
        '>', ' ','^',' ', '<',    
    };

    Point p, q, rec;
    Box p1 = {p, O_size};
    Box p2 = {q, O_size};
    Box blue_shots[MAX_SHOTS];
    Box red_shots[MAX_SHOTS];

    Point cat_points[] = {
        {x_start + 0, y_start + 0}, {x_start + 1, y_start + 0}, {x_start + 2, y_start + 0}, {x_start + 3, y_start + 0}, 
        {x_start + 4, y_start + 0},      
        {x_start - 1, y_start + 1}, {x_start + 0, y_start + 1}, {x_start + 1, y_start + 1},
        {x_start + 2, y_start + 1}, {x_start + 3, y_start + 1}, {x_start + 4, y_start + 1}, {x_start + 5, y_start + 1}, 
        {x_start + 0, y_start + 2}, {x_start + 1, y_start + 2}, {x_start + 2, y_start + 2}, {x_start + 3, y_start + 2}, {x_start + 4, y_start + 2} 
    };

    initscr();
    start_color();
    noecho();
    timeout(0);
    curs_set(0);
    cbreak();

    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(4, COLOR_BLUE, COLOR_BLACK);

    getmaxyx(stdscr, y_max, x_max);

    p.x = x_max - 5;
    p.y = 5;
    q.x = 5;
    q.y = y_max / 2;
    p1.s = p;
    p2.s = q;
    rec.x = x_max / 2;
    rec.y = y_max / 2;

    for (int i = 0; i < MAX_SHOTS; i++) {
        blue_shots[i].s.x = 0;
        blue_shots[i].s.y = 0;
        blue_shots[i].radius = 1;

        red_shots[i].s.x = 0;
        red_shots[i].s.y = 0;
        red_shots[i].radius = 1;
    }

    clock_t start_time = clock();

    while (1) {
        key = getch();

        if (key == 'a') {
            blue_dir = -1;
        } else if (key == 'd') {
            blue_dir = 1;
        }
        if (key == 'q') break;

        if (key == 'p') {
            blue_hit_count -= 1;
        }

        if (key == 'c') {
            sift_cat++;
        }

        if (sift_cat == 0) {
            if (blue_dir != 0) {
                move_circle(&p2, blue_dir, y_max);
            }

            move_circle(&p1, red_dir, y_max);
            if (p1.s.y == 4 || p1.s.y == y_max - 4) {
                red_dir *= -1;
            }
            if (rand() % (3 - t) == 0) {
                for (int i = 0; i < MAX_SHOTS; i++) {
                    if (red_shots[i].s.x == 0) {
                        red_shots[i].s.x = p1.s.x - O_size;
                        red_shots[i].s.y = p1.s.y;
                        break;
                    }
                    t = rand() % 3;
                }
            }

            clear();

            draw_circle(p1, 'O', 1, &p2, blue_shots, MAX_SHOTS, &hit_flag, &enemy_hit_flag, &scored_hit_flag, cat_points, 17);
            draw_circle(p2, 'O', 4, &p1, red_shots, MAX_SHOTS, &hit_flag, &enemy_hit_flag, &scored_hit_flag, cat_points, 17);

        } else {

            draw_cat(cat_points, cat_chars, 17);
            draw_circle(p1, 'O', 1, &p2, blue_shots, MAX_SHOTS, &hit_flag, &enemy_hit_flag, &scored_hit_flag, cat_points, 17);

            clear();
            if (blue_dir != 0) {
                for (int u = 0; u < 17; u++) {
                    move_point(&cat_points[u], blue_dir, y_max);
                }
            }

            move_circle(&p1, red_dir, y_max);
            if (p1.s.y == 4 || p1.s.y == y_max - 4) {
                red_dir *= -1;
            }

            if (rand() % (2 - t) == 0) {
                for (int i = 0; i < MAX_SHOTS; i++) {
                    if (red_shots[i].s.x == 0) {
                        red_shots[i].s.x = p1.s.x - O_size;
                        red_shots[i].s.y = p1.s.y;
                        break;
                    }
                    t = rand() % 1;

                }
            }

            clear();

            draw_cat(cat_points, cat_chars, 17);
            draw_circle(p1, 'O', 1, &p2, blue_shots, MAX_SHOTS, &hit_flag, &enemy_hit_flag, &scored_hit_flag, cat_points, 17);
        }

        for (int i = 0; i < MAX_SHOTS; i++) {
            if (red_shots[i].s.x > 0) {
                red_shots[i].s.x -= 2;
                if (red_shots[i].s.x < 0) {
                    red_shots[i].s.x = 0;
                } else {
                    draw_circle(red_shots[i], 'o', 1, &p2, blue_shots, MAX_SHOTS, &hit_flag, &enemy_hit_flag, &scored_hit_flag, cat_points, 17);
                }

                if (hit_flag) {
                    red_shots[i].s.x = 0;
                    blue_hit_count++;
                    if (blue_hit_count >= 3) {
                        break;
                    }
                }
            }
        }

        refresh();
        usleep(wait_interval);

        if (blue_hit_count >= 3) {
            break;
        }
    }

    clock_t end_time = clock();
    double total_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printGameOver(rec, total_time);
    mvprintw(y_max / 2 - 2, x_max / 2 - 2, "Thank you for playing");
    mvprintw(y_max / 2, x_max / 2 - 2, "Time : %.2e[sec]", total_time);
    refresh();
    sleep(2);
    endwin();

    return 0;
}

int draw_point(Point a, char c) {
    int status = 0;
    int x_max, y_max;

    getmaxyx(stdscr, y_max, x_max);

    if (0 <= a.x && a.x < x_max && 0 <= a.y && a.y < y_max) {
        move(a.y, a.x);
        addch(c);
    } else {
        status = 1;
    }

    return status;
}

int draw_circle(Box a, char c, int color_pair, Box *enemy, Box *shots, int num_shots, int *hit_flag, int *enemy_hit_flag, int *scored_hit_flag, Point cat_points[], int num_cat_points) {
    int x = 0;
    int y = a.radius;
    int d = 3 - 2 * a.radius;
    int status = 0;

    *hit_flag = 0;
    *enemy_hit_flag = 0;
    *scored_hit_flag = 0;

    attron(COLOR_PAIR(color_pair));

    while (y >= x) {
        Point points[8] = {
            {a.s.x + x, a.s.y + y},
            {a.s.x - x, a.s.y + y},
            {a.s.x + x, a.s.y - y},
            {a.s.x - x, a.s.y - y},
            {a.s.x + y, a.s.y + x},
            {a.s.x - y, a.s.y + x},
            {a.s.x + y, a.s.y - x},
            {a.s.x - y, a.s.y - x}
        };

        for (int i = 0; i < 8; i++) {
            draw_point(points[i], c);

            Point enemy_points[8] = {
                {enemy->s.x + x, enemy->s.y + y},
                {enemy->s.x - x, enemy->s.y + y},
                {enemy->s.x + x, enemy->s.y - y},
                {enemy->s.x - x, enemy->s.y - y},
                {enemy->s.x + y, enemy->s.y + x},
                {enemy->s.x - y, enemy->s.y + x},
                {enemy->s.x + y, enemy->s.y - x},
                {enemy->s.x - y, enemy->s.y - x}
            };

            for (int j = 0; j < 8; j++) {
                if (points[i].x == enemy_points[j].x && points[i].y == enemy_points[j].y) {
                    *hit_flag = 1;
                }
            }
            
        }

        if (d < 0) {
            d = d + 4 * x + 6;
        } else {
            d = d + 4 * (x - y) + 10;
            y--;
        }
        x++;
    }

    attroff(COLOR_PAIR(color_pair));

    return status;
}

void move_point(Point *a, int dir_y, int y_max) {
    a->y += dir_y;

    if (a->y < 1) a->y = 1;
    if (a->y >= y_max - 1) a->y = y_max - 2;
}


void move_circle(Box *a, int dir_y, int y_max) {
    a->s.y += dir_y;
    if (a->s.y < 1) a->s.y = 1;
    if (a->s.y >= y_max - 1) a->s.y = y_max - 2;
}

void printGameOver(Point rec, double clock)
{
    mvprintw(LINES / 2 - 4, (COLS - 70) / 2, " GGGG    AAA   M     M  EEEEE        OOOO   V     V  EEEEE  RRRR       ");
    mvprintw(LINES / 2 - 3, (COLS - 70) / 2, "G    G  A   A  MM   MM  E           O    O  V     V  E      R   R      ");
    mvprintw(LINES / 2 - 2, (COLS - 70) / 2, "G       A   A  M M M M  E           O    O  V     V  E      R    R     ");
    mvprintw(LINES / 2 - 1, (COLS - 70) / 2, "G       AAAAA  M  M  M  EEEEE       O    O  V     V  EEEEE  RRRRR      ");
    mvprintw(LINES / 2, (COLS - 70) / 2,     "G       A   A  M     M  E           O    O  V     V  E      R  R       ");
    mvprintw(LINES / 2 + 1, (COLS - 70) / 2, "G   GGG A   A  M     M  E           O    O   V   V   E      R   R      ");
    mvprintw(LINES / 2 + 2, (COLS - 70) / 2, "G    G  A   A  M     M  E           O    O    V V    E      R    R     ");
    mvprintw(LINES / 2 + 3, (COLS - 70) / 2, " GGGG   A   A  M     M  EEEEE        OOOO      V     EEEEE  R     R    ");

    refresh();
    sleep(2);

    mvprintw(LINES / 2 - 4, (COLS - 70) / 2, "                                                                       ");
    mvprintw(LINES / 2 - 3, (COLS - 70) / 2, "                                                                       ");
    mvprintw(LINES / 2 - 2, (COLS - 70) / 2, "                                                                       ");
    mvprintw(LINES / 2 - 1, (COLS - 70) / 2, "                                                                       ");
    mvprintw(LINES / 2, (COLS - 70) / 2,     "                                                                       ");
    mvprintw(LINES / 2 + 1, (COLS - 70) / 2, "                                                                       ");
    mvprintw(LINES / 2 + 2, (COLS - 70) / 2, "                                                                       ");
    mvprintw(LINES / 2 + 3, (COLS - 70) / 2, "                                                                       ");

    refresh();

    clear();
}

void draw_cat(Point cat_points[], char cat_chars[], int num_points) {
    for (int i = 0; i < num_points; i++) {
        draw_point(cat_points[i], cat_chars[i]);
    }
}
