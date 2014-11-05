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
    Tank (int x = 0, int y = 0, int napr = 0, int w = 0, int h = 0, int speed = 1, bool exist = true) {
        _x = x;
        _y = y;
        _napr = napr;
        _w = w;
        _h = h;
        _speed = speed;
        _bullets = new ArrayBullet;
        _exist = exist;
        _time_fire = 0;
    }
    Tank (const Tank &t) {
        _x = t._x;
        _y = t._y;
        _napr = t._napr;
        _w = t._w;
        _h = t._h;
        _speed = t._speed;
        _bullets = new ArrayBullet;
        _exist = t._exist;
        _time_fire = t._time_fire;
    }
    void x(int i) { _x = i;}
    void y(int i) { _y = i;}
    void napr(int i) { _napr = i;}
    void h(int i) { _h = i;}
    void w(int i) { _w = i;}
    void exist(bool f) { _exist = f;}
    void time_fire(int i) { _time_fire = i;}
    void picture(SDL_Surface *t, int i) {
        _picture[i] = t;
    }
    void fire(SDL_Surface *t, int i) {
        _fire[i] = t;
    }

    int x(void) { return _x;}
    int y(void) { return _y;}
    int napr(void) { return _napr;}
    int h(void) { return _h;}
    int w(void) { return _w;}
    int time_fire(void) { return _time_fire;}
    bool exist(void) { return _exist;}
    SDL_Surface * picture(int i) {
        return _picture[i];
    }
    ArrayBullet * bullets (void) {
        return _bullets;
    }
    SDL_Surface *fire(int i) {
        return _fire[i];
    }

    void up(bool f) {
        if (f) _y -= _speed;
        _napr = Forward;
    }
    void down(bool f) {
        if (f) _y += _speed;
        _napr = Back;
    }
    void left(bool f) {
        if (f) _x -= _speed;
        _napr = Left;
    }
    void right(bool f) {
        if (f) _x += _speed;
        _napr = Right;
    }
    void run(void);
private:
    int _x;
    int _y;
    int _napr;
    int _w;
    int _h;
    int _speed;
    int _time_fire;
    SDL_Surface *_picture[4];
    SDL_Surface *_fire[5];
    ArrayBullet *_bullets;
    bool _exist;
};
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
