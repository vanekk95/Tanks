#ifndef _tank
#define _tank

#define Forward 1
#define Back    0
#define Right   3
#define Left    2
#define Stop    4

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
#include "Bullet.h"

class Tank
{
public:
    Tank  ( int x = 0, int y = 0, int napr = 0,
            int w = 0, int h = 0, bool exist = true);
    Tank (const Tank &t);
    void x(int i) { _x = i;}
    void y(int i) { _y = i;}
    void napr(int i) { _napr = i;}
    void h(int i) { _h = i;}
    void w(int i) { _w = i;}
    void exist(bool f) { _exist = f;}
    void time(int t) { _time = t;}
    void frame_fire(int i) { _frame_fire = i;}
    void picture(SDL_Surface *t, int i) {
        _picture[i] = t;
    }
    void fire(SDL_Surface *t) {
        _fire = t;
    }

    int x(void) { return _x;}
    int y(void) { return _y;}
    int napr(void) { return _napr;}
    int h(void) { return _h;}
    int w(void) { return _w;}
    int time(void) { return _time;}
    int frame_fire(void) { return _frame_fire;}
    int speed_fire(void) { return _speed_fire;}
    int max_frame_fire(void) { return _max_frame_fire;}
    bool exist(void) { return _exist;}
    SDL_Surface * picture(int i) {
        return _picture[i];
    }
    ArrayBullet * bullets (void) {
        return _bullets;
    }
    SDL_Surface *fire() {
        return _fire;
    }
    void up(bool f);
    void down(bool f);
    void left(bool f);
    void right(bool f);
    void run(void);
    void clean(void);

private:
    int _x;
    int _y;
    int _napr;
    int _w;
    int _h;
    int _speed;
    int _time;
    int _speed_fire;
    int _frame_fire;
    int _max_frame_fire;
    SDL_Surface *_picture[4];
    SDL_Surface *_fire;
    ArrayBullet *_bullets;
    bool _exist;
};

void Tank::up(bool f) {
        if (f) _y -= _speed;
        _napr = Forward;
    }
void Tank::down(bool f) {
        if (f) _y += _speed;
        _napr = Back;
    }
void Tank::left(bool f) {
        if (f) _x -= _speed;
        _napr = Left;
    }
void Tank::right(bool f) {
        if (f) _x += _speed;
        _napr = Right;
    }
Tank::Tank (const Tank &t)
{
        _x = t._x;
        _y = t._y;
        _napr = t._napr;
        _w = t._w;
        _h = t._h;
        _speed = t._speed;
        _bullets = new ArrayBullet;
        _exist = t._exist;
        _frame_fire = 0;
        _speed_fire = t._speed_fire;
        _max_frame_fire = 5;
        _time = 0;
}
Tank::Tank(int x, int y, int napr,
            int w, int h, bool exist)
{
        _x = x;
        _y = y;
        _napr = napr;
        _w = w;
        _h = h;
        _speed = 1;
        _bullets = new ArrayBullet;
        _exist = exist;
        _frame_fire = 0;
        _speed_fire = 100;
        _max_frame_fire = 5;
        _time = 0;
}
void Tank::clean(void)
{
    for (int i = 0; i < 4; i++)
        SDL_FreeSurface(_picture[i]);
    SDL_FreeSurface(_fire);
    SDL_FreeSurface(_bullets->picture);
}
void Tank::run(void) {
        switch (_napr)
        {
            case Forward:
            {
                _y -= _speed;
                break;
            }
            case Back:
            {
                _y += _speed;
                break;
            }
            case Left:
            {
                _x -= _speed;
                break;
            }
            case Right:
            {
                _x += _speed;
                break;
            }
        }
    }
#endif // _tank
