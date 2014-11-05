#ifndef _calculation
#define _calculation

#include <stdio.h>
#include "tank.h"

#define Forward 1
#define Back    0
#define Right   3
#define Left    2
#define Stop    4
#define MAXnumberBul 100
#define SpeedBul 3
#define H  30
#define W  40

int arrayWall[W][H];
extern SDL_Surface *Wall;

void CreateBullet(Tank* tank);

void InitTank(Tank *my_tank)
{
    my_tank->x ((screen->w - my_tank->picture(0)->w) / 2);
    my_tank->y ((screen->h - my_tank->picture(0)->h) / 2);
    my_tank->napr (Forward);
    my_tank->w (my_tank->picture(0)->w);
    my_tank->h (my_tank->picture(0)->h);
}

void InitGrayTank(Tank *tank)
{
    tank->x (3*(screen->w - tank->picture(0)->w) / 4);
    tank->y ((screen->h - tank->picture(0)->h) / 4);
    tank->napr (Back);
    tank->w (tank->picture(0)->w);
    tank->h (tank->picture(0)->h);
}
bool In_screen(Tank *tank, int napr)
{
    switch(napr)
    {
        case Forward:
        {
            if (tank->y() < 0)
                return false;
            break;
        }
        case Back:
        {
            if (tank->y() + tank->picture(0)->h >= H * Wall->h)
                return false;
            break;
        }
        case Right:
        {
            if (tank->x() + tank->picture(0)->w >= W * Wall->w)
                return false;
            break;
        }
        case Left:
        {
            if (tank->x() < 0)
                return false;
            break;
        }
    }
    return true;
}
bool NotWall(Tank *tank, int napr)
{
    if (!In_screen(tank, napr))
        return false;
    int k = tank->picture(0)->h / Wall->h + 1;
    int X = tank->x() / Wall->w;
    int Y = tank->y() / Wall->h;

    for (int i = (X > k) ? X - k : 0; i < W; i++)
    for (int j = (Y > k) ? Y - k : 0; j < H; j++)
    {
        if (arrayWall[i][j])
            switch (napr)
            {
                case Forward:
                {
                    if (tank->y() == (j+1)*Wall->h &&
                        ((tank->x() < i*Wall->w && i*Wall->w < tank->x() + tank->picture(0)->w) ||
                        (tank->x() < (i+1)*Wall->w && (i+1)*Wall->w < tank->x() + tank->picture(0)->w)))
                        return false;
                    break;
                }
                case Back:
                {
                    if (tank->y() + tank->picture(0)->h == j*Wall->h &&
                        ((tank->x() < i*Wall->w && i*Wall->w < tank->x() + tank->picture(0)->w) ||
                        (tank->x() < (i+1)*Wall->w && (i+1)*Wall->w < tank->x() + tank->picture(0)->w)))
                        return false;
                    break;
                }
                case Right:
                {
                    if  (tank->x() + tank->picture(0)->w == i*Wall->w &&
                        ((tank->y() < j*Wall->h && j*Wall->h < tank->y() + tank->picture(0)->h) ||
                        (tank->y() < (j+1)*Wall->h && (j+1)*Wall->h < tank->y() + tank->picture(0)->h)))
                        return false;
                    break;
                }
                case Left:
                {
                    if  (tank->x() == (i+1)*Wall->w &&
                        ((tank->y() < j*Wall->h && j*Wall->h < tank->y() + tank->picture(0)->h) ||
                        (tank->y() < (j+1)*Wall->h && (j+1)*Wall->h < tank->y() + tank->picture(0)->h)))
                        return false;
                    break;
                }
            }
    }
    return true;
}
/*
void GoTank(Tank *tank)
{
    if (tank->exist()) {
        if (NotWall(tank, tank->napr()))
            tank->run();
        else {
            tank->napr((tank->napr() + 3) % 4);
        }
    }
}
*/

void GoTank(Tank *my_tank, Tank *gray_tank)
{
    if (!gray_tank->exist() || !my_tank->exist())
        return;


    static int timesCalled = 0;
    int dy = my_tank->y() - gray_tank->y();
    int dx = my_tank->x() - gray_tank->x();

    if (timesCalled++ >10)
    {
        timesCalled = 0;
        CreateBullet(gray_tank);
    }
   if (abs(dy) > 10)
   {
        if (dy > 0)
            gray_tank->napr(Back);
        else
            gray_tank->napr(Forward);
    }
    else if (abs(dx) > 10)
   {
        if (dx > 0)
            gray_tank->napr(Right);
        else
            gray_tank->napr(Left);
    }

     if (NotWall(gray_tank, gray_tank->napr()))
            gray_tank->run();


}


void CreateBullet(Tank *tank)
{
    if (tank->bullets()->index >= MAXnumberBul) {
        tank->bullets()->index = 0;
    }
    tank->bullets()->new_bullet(tank->x(), tank->y(), tank->napr(), tank->w(), tank->h());
    tank->bullets()->index++;
}

void FlyBullet(Tank * tank, Tank *gray_tank)
{
    int ii, jj;
    for (int i = 0; i < MAXnumberBul; i++)
    {
        if (tank->bullets()->arr[i].exist()) {
            tank->bullets()->arr[i].fly();
            int X = tank->bullets()->arr[i].x() + tank->bullets()->picture->w / 2;
            int Y = tank->bullets()->arr[i].y() + tank->bullets()->picture->h / 2;
            ii = (X)/ Wall->h;
            jj = (Y)/ Wall->w;
            if (ii >= 0 && jj >= 0 && ii < W && jj < H && arrayWall[ii][jj]) {
                arrayWall[ii][jj] = 0;
                tank->bullets()->arr[i].exist(false);
            }
            if (gray_tank->exist())
                if (gray_tank->x() < X && X < gray_tank->x() + gray_tank->picture(0)->w &&
                    gray_tank->y() < Y && Y < gray_tank->y() + gray_tank->picture(0)->h) {
                        gray_tank->exist(false);
                        tank->bullets()->arr[i].exist(false);
                }
        }
    }
    tank->bullets()->go_out();
}

bool InitWall(const char *file)
{
    FILE *f = fopen(file, "r");
    if (f != NULL) {
        int i, j;
        for (i = 0; i < H; i++)
        for (j = 0; j < W; j++)
            fscanf(f, "%d", &arrayWall[j][i]);
        return true;
    }
    else
        return false;
}

#endif // _calculation
