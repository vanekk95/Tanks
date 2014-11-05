#ifndef _drawing
#define _drawing

#ifdef __cplusplus
    #include <cstdlib>
#else
    #include <stdlib.h>
#endif
#ifdef __APPLE__
#include <SDL/SDL.h>
#else
#include <SDL.h>
#endif
#include <SDL_ttf.h>
#include <stdio.h>
#include <cstdlib>
#include <ctime>
#include "tank.h"


extern SDL_Surface *screen;
extern SDL_Surface *Wall, *background;

int InitImages(Tank *tank, Tank *gray_tank)
{
    for (int i = 0; i < 4; i++)
    {
        char path[20];
        for (int j = 0; j < 20; j++)
            path[j] = 0;
        sprintf(path, "tank%c.bmp", (char)(i + '0'));
        tank->picture(SDL_LoadBMP(path), i);
        SDL_SetColorKey(tank->picture(i),
                        SDL_SRCCOLORKEY,
                        SDL_MapRGB(tank->picture(i)->format,
                        0, 0, 0));
        if (!tank->picture(i))
            return -1;
    }
    gray_tank->fire(SDL_LoadBMP("fire.bmp"));
    SDL_SetColorKey(gray_tank->fire(),
                        SDL_SRCCOLORKEY,
                        SDL_MapRGB(gray_tank->fire()->format,
                        255, 255, 255));
    if (!gray_tank->fire())
            return -1;
    for (int i = 0; i < 4; i++)
    {
        char path[20];
        for (int j = 0; j < 20; j++)
            path[j] = 0;
        sprintf(path, "gray_tank%c.bmp", (char)(i + '0'));
        gray_tank->picture(SDL_LoadBMP(path), i);
        SDL_SetColorKey(gray_tank->picture(i),
                        SDL_SRCCOLORKEY,
                        SDL_MapRGB(gray_tank->picture(i)->format,
                        255, 255, 255));
        if (!gray_tank->picture(i))
            return -1;
    }
    tank->bullets()->picture = SDL_LoadBMP("bullet.bmp");
    gray_tank->bullets()->picture = SDL_LoadBMP("bullet.bmp");
    SDL_SetColorKey(tank->bullets()->picture,
                    SDL_SRCCOLORKEY,
                    SDL_MapRGB(tank->bullets()->picture->format,
                    0, 0, 0));
    Wall = SDL_LoadBMP("wall.bmp");
    background = SDL_LoadBMP("background.bmp");
    if (tank->bullets()->picture == 0)
        return -1;
    if (Wall == 0 || background == 0)
        return -1;
    return 1;
}

void DrawImage(SDL_Surface * img, int x, int y, int w, int h, int sx, int sy)
{
    SDL_Rect desc;
    desc.x = x;
    desc.y = y;
    SDL_Rect src;
    src.x = sx;
    src.y = sy;
    src.w = w;
    src.h = h;
    SDL_BlitSurface(img, &src, screen, &desc);
}

void DrawBackground(void)
{
    int h = background->h;
    int w = background->w;
    int i, j;
    for (i = 0; i < W; i++)
    for (j = 0; j < H; j++)
    {
        SDL_Rect desc;
        desc.x = i * w;
        desc.y = j * h;
        SDL_Rect src;
        src.x = 0;
        src.y = 0;
        src.w = w;
        src.h = h;
        SDL_BlitSurface(background, &src, screen, &desc);
    }
}

void DrawBullet(Tank *tank)
{
    for (int i = 0; i < MAXnumberBul; i++)
    {
        if (tank->bullets()->arr[i].exist())
            DrawImage(tank->bullets()->picture,
                    tank->bullets()->arr[i].x(),
                    tank->bullets()->arr[i].y(),
                    tank->bullets()->picture->w,
                    tank->bullets()->picture->h,
                    0, 0);
    }
}

void DrawWall(int i, int j)
{
    DrawImage(Wall, i * Wall->w, j * Wall->h, Wall->w, Wall->h, 0, 0);
}
void PrintWall(void)
{
    int i, j;
    for (j = 0; j < W; j++)
    for (i = 0; i < H; i++)
        if (arrayWall[j][i])
            DrawWall(j, i);
}

void DrawTank(Tank *tank)
{
    if (tank->exist())
        DrawImage(tank->picture(tank->napr()),
                tank->x(),
                tank->y(),
                tank->w(),
                tank->h(),
                0, 0);
    else {
        if (tank->frame_fire() < tank->max_frame_fire()) {
            DrawImage(tank->fire(),
                    tank->x(),
                    tank->y(),
                    tank->w(),
                    tank->h(),
                    0, tank->h()*(tank->frame_fire()) );
        }
        if (tank->time() + tank->speed_fire() < SDL_GetTicks()) {
            tank->frame_fire(tank->frame_fire() + 1);
            tank->time(SDL_GetTicks());
        }
    }
}

void Draw(Tank *my_tank, Tank *gray_tank)
{
        DrawBackground();
        PrintWall();
        DrawBullet(my_tank);
        DrawBullet(gray_tank);
        DrawTank(gray_tank);
        DrawTank(my_tank);
}
#endif // _drawing
