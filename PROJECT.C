#include<ncurses.h>
#include<time.h>
#include<stdlib.h>
#include<unistd.h>
int score=0;
int high_score=0;
int obstacles_count = 2;
int obstacles[10];
int height = 25;
int width = 50;
int jumping = 0;
int jump_step = 0;
int jump_height = 3;
int human_x = 2;
int human_y = height -3;
int speed = 50000;
int game_mode=1;
char human[3][5] =
{
    " o ",
    "<||>",
    " || "
};
void day_mode()
{
    start_color();
    init_pair(1,COLOR_BLACK,COLOR_WHITE);
    attron(COLOR_PAIR(1));
    bkgd(COLOR_PAIR(1));
}
void night_mode()
{
    start_color();
    init_pair(2,COLOR_WHITE,COLOR_BLACK);
    attron(COLOR_PAIR(2));
    bkgd(COLOR_PAIR(2));
}
void print_score()
{
    move(0,0);
    if (high_score<score)
    {
        high_score = score;
    }
    printw("SCORE : %d , HIGH_SCORE : %d",score,high_score);
}
void init_obstacles()
{
    int i;
    obstacles[0] = width - 10;
    for (i=1;i<obstacles_count;i++)
    {
        obstacles[i] = obstacles[i-1] - (rand() % 20 + 10);
    }
}
void print_obstacles()
{
    int i;
    for (i=0;i<obstacles_count;i++)
    {
        if (obstacles[i] >= width || obstacles[i] < 0)
        {
            continue;
        }
        move(height - 1 ,obstacles[i]);
        printw("|");
    }
}
void update_obstacles()
{
    int i;
    for (i=0;i < obstacles_count;i++)
    {
        obstacles[i]--;
        if (obstacles[i] < 0)
        {
            obstacles[i] = width - 1;
        }
    }
}
void check_score()
{
    int i;
    for (i = 0; i < obstacles_count; i++)
    {
        if (obstacles[i] == human_x - 1)
        {
            score++;
            if (score % 20 == 0)
            {
                if (game_mode == 1)
                {
                    night_mode();
                    game_mode = 2;
                }
                else
                {
                    day_mode();
                    game_mode = 1;
                }
            }
            if (score % 10 == 0 && speed > 10000)
            {
                speed -= 1000;
            }
        }
    }
}
int print_human(int x,int y)
{
    int j,c,y2;
    init_pair(3,COLOR_WHITE,COLOR_BLACK);
    attron(COLOR_PAIR(3));
    for (y2 = y;y2 < y + 3;y2++)
    {
        for (j = x;j < x + 4;j++)
        {
            c = mvinch(y2,j);
            if (c == '|')
            {
                return 1;
            }
            move(y2,j);
            printw("%c",human[y2-y][j-x]);
        }
    }
    attroff(COLOR_PAIR(3));
    return 0;
}
int main()
{
    int crash;
    initscr();
    noecho();
    timeout(0);
    srand(time(NULL));
    init_obstacles();
    day_mode();
    while (1)
    {
        int ch = getch();
        if (ch == ' ' && !jumping)
        {
            jumping = 1;
            jump_step = jump_height;
        }
        if (jumping)
        {
            human_y--;
            jump_step--;
            if (jump_step<=0)
            {
                jumping = 0;
            }
        }
        else if (human_y < height - 3)
        {
            human_y++;
        }
        clear();
        print_score();
        crash = print_human(human_x,human_y);
        if (crash)
        {
            move(height / 2 , width / 2 - 5);
            printw("GAME_OVER :(");
            refresh();
            usleep(200000);
            break;
        }
        print_obstacles();
        update_obstacles();
        check_score();
        refresh();
        usleep(speed);
    }
    endwin();
    printw("GAME OVER :( HIGH SCORE : %d",high_score);
    return 0;
}