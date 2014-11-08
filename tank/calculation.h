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

bool free_place (int x, int y)
{
    if (x < 0 || y < 0 || x + 2 >= W || y + 2 >= H)
        return false;
    for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++)
        if (arrayWall[x + i][y + j])
            return false;
    return true;
}
void GoTank(Tank *my_tank, Tank *gray_tank)
{
    if (!gray_tank->exist() || !my_tank->exist())
        return;

    static int timesCalled = 0;
//    int dy = my_tank->y() - gray_tank->y();
//    int dx = my_tank->x() - gray_tank->x();

    if (timesCalled++ > 15)
    {
        timesCalled = 0;
        CreateBullet(gray_tank);
    }
    static bool notDone = true;
    struct point {
        int x;
        int y;
    };
    static struct point path[200];
    static int index;
    int x = gray_tank->x() / Wall->w;
    int y = gray_tank->y() / Wall->h;
    int goal_x = my_tank->x() / Wall->w;
    int goal_y = my_tank->y() / Wall->h;

    struct cell {
        int status;
        int path;
        int prev_x;
        int prev_y;
    };
    static struct cell _map[W][H];
if (notDone) {


    // status = 0 - не посещено
    // status = 1 - посещено
    #define Infinity 1000000
    for (int i = 0; i < W; i++)
    for (int j = 0; j < H; j++) {
        _map[i][j].status = 0;
        _map[i][j].path = Infinity;
    }
    _map[x][y].path = 0;
    _map[x][y].prev_x = -1;
    _map[x][y].prev_y = -1;

    int t = 1;
    while (t)
    {
        t = 0;
        for (int i = 0; i < W; i++)
        for (int j = 0; j < H; j++)
            if (_map[i][j].status == 0 && _map[i][j].path < Infinity)
            {
                t++;
                if (free_place(i-1, j) &&
                    _map[i-1][j].path > _map[i][j].path + 1) {
                            _map[i-1][j].path = _map[i][j].path + 1;
                            _map[i-1][j].prev_x = i;
                            _map[i-1][j].prev_y = j;
                }
                if (free_place(i+1, j) &&
                    _map[i+1][j].path > _map[i][j].path + 1) {
                            _map[i+1][j].path = _map[i][j].path + 1;
                            _map[i+1][j].prev_x = i;
                            _map[i+1][j].prev_y = j;
                }

                if (free_place(i, j-1) &&
                    _map[i][j-1].path > _map[i][j].path + 1) {
                            _map[i][j-1].path = _map[i][j].path + 1;
                            _map[i][j-1].prev_x = i;
                            _map[i][j-1].prev_y = j;
                }

                if (free_place(i, j+1) &&
                    _map[i][j+1].path > _map[i][j].path + 1) {
                            _map[i][j+1].path = _map[i][j].path + 1;
                            _map[i][j+1].prev_x = i;
                            _map[i][j+1].prev_y = j;
                }
                _map[i][j].status = 1;
            }
    }

    for (int j = 0; j < H; j++) {
    for (int i = 0; i < W; i++)
        printf("%d ", _map[i][j].status);
    printf("\n");
    }

    int i = 0;

    do {
        path[i].x = goal_x;
        path[i].y = goal_y;
        goal_x = _map[path[i].x][path[i].y].prev_x;
        goal_y = _map[path[i].x][path[i].y].prev_y;
        i++;
    } while (goal_x >= 0 && goal_y >= 0 && _map[goal_x][goal_y].path < Infinity);
    index = i - 2;
    printf("\nindex = %d\n", index);
    for (int k = i-1; k > 0; k--)
        printf("%d %d\n", path[k].x, path[k].y);
}

    bool f = true;
//    if (NotWall(gray_tank, gray_tank->napr()))
        f = gray_tank->go_into(path[index].x*Wall->w, path[index].y*Wall->h);
    if (!f) index--;
    notDone = false;
    if (gray_tank->time() + 3000 < SDL_GetTicks() || index == 0) {
        notDone = true;
        gray_tank->time(SDL_GetTicks());
    }

/*
    static bool isOK;
    isOK = false;
   if (abs(dy) > 10)
   {
        if (dy > 0)
            gray_tank->napr(Back);
        else
            gray_tank->napr(Forward);

        if (NotWall(gray_tank, gray_tank->napr()))
            isOK = true;

    }
    if (!isOK && abs(dx) > 10)
   {
        if (dx > 0)
            gray_tank->napr(Right);
        else
            gray_tank->napr(Left);
        if (NotWall(gray_tank, gray_tank->napr()))
                isOK = true;
    }


 if (NotWall(gray_tank, gray_tank->napr()))
            gray_tank->run();
*/
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

bool InitWallandTanks(const char *file, Tank *my_tank, Tank *gray_tank)
{
    FILE *f = fopen(file, "r");
    if (f != NULL) {
        int i, j, c;
        for (j = 0; j < H; j++)
        for (i = 0; i < W; i++) {
            fscanf(f, "%d", &c);
            if (c < 2) arrayWall[i][j] = c;
            else {
                arrayWall[i][j] = 0;
                if (c == 2) {
                    my_tank->x(Wall->w * i);
                    my_tank->y(Wall->h * j);
                } else
                if (c == 3) {
                    gray_tank->x(Wall->w * i);
                    gray_tank->y(Wall->h * j);
                }
            }
        }
        my_tank->napr (Forward);
        my_tank->w (my_tank->picture(0)->w);
        my_tank->h (my_tank->picture(0)->h);
        gray_tank->napr (Back);
        gray_tank->w (gray_tank->picture(0)->w);
        gray_tank->h (gray_tank->picture(0)->h);
        return true;
    }
    else
        return false;
}

#endif // _calculation
