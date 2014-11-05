#ifndef _bullet
#define _bullet


#define Forward 1
#define Back    0
#define Right   3
#define Left    2
#define Stop    4
#define MAXnumberBul 100
#define SpeedBul 3

extern SDL_Surface *screen;

class Bullet {
public:
    Bullet(int x = 0, int y = 0, int napr = 0, bool exist = 0) {
        _x = x;
        _y = y;
        _napr = napr;
        _exist = exist;
    }
    Bullet(const Bullet &bul) {
        _x = bul._x;
        _y = bul._y;
        _napr = bul._napr;
        _exist = bul._exist;
    }
    int x(void) { return _x;}
    int y(void) { return _y;}
    int napr(void) {return _napr;}
    bool exist(void) { return _exist;}

    void x(int i) { _x = i;}
    void y(int i) { _y = i;}
    void napr(int i) {_napr = i;}
    void exist(bool f) {_exist = f;}

    void create(int x, int y, int napr, int w, int h);
    void fly(void);
    bool go_out(void);
private:
    int _x;
    int _y;
    int _napr;
    bool _exist;
};

class ArrayBullet {
public:
    Bullet arr[MAXnumberBul];
    SDL_Surface *picture;
    int index;
    void new_bullet(int x, int y, int napr, int w, int h);
    void go_out(void);
};


void ArrayBullet::new_bullet(int x, int y, int napr, int w, int h)
{
    arr[index].napr(napr);
    arr[index].exist(true);
    switch (napr)
    {
        case Forward:
        {
            arr[index].x(x + (w - picture->w) / 2);
            arr[index].y(y);
            break;
        }
        case Back:
        {
            arr[index].x(x + (w - picture->w) / 2);
            arr[index].y(y + h);
            break;
        }
        case Left:
        {
            arr[index].x(x);
            arr[index].y(y + (h - picture->h) / 2);
            break;
        }
        case Right:
        {
            arr[index].x(x + w);
            arr[index].y(y + (h - picture->h) / 2);
            break;
        }
    }
}

void Bullet::fly(void)
{
    if (_exist) {
        switch(_napr)
        {
            case Forward:
            {
                _y -= SpeedBul;
                break;
            }
            case Back:
            {
                _y += SpeedBul;
                break;
            }
            case Right:
            {
                _x += SpeedBul;
                break;
            }
            case Left:
            {
                _x -= SpeedBul;
            }
        }
    }
}
void ArrayBullet::go_out(void)
{
    for (int i = 0; i < index; i++)
    {
        if (arr[i].x() < -picture->w || arr[i].x() > screen->w ||
            arr[i].y() < -picture->h || arr[i].y() > screen->h)
            arr[i].exist(false);
    }
}

#endif


