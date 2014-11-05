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
#include <unistd.h>
#include "Bullet.h"
#include "calculation.h"
#include "drawing.h"
#include "tank.h"

#define Forward 1
#define Back    0
#define Right   3
#define Left    2
#define Stop    4
#define MAXnumberBul 100
#define H  30
#define W  40

SDL_Surface *screen;
SDL_Surface *Wall, *background;


bool processInput(Tank * my_tank)
{
    bool done = false;
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
                // exit if the window is closed
            case SDL_QUIT:
            {
                done = true;
                break;
            }
                // check for keypresses
            case SDL_KEYDOWN:
            {
                switch(event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                    {
                        done = true;
                        break;
                    }
                    case SDLK_TAB:
                    {
                        CreateBullet(my_tank);
                        break;
                    }
                }
            }
           /* case SDL_KEYUP:
            {
                switch(event.key.keysym.sym)
                    case SDLK_TAB:
                    {
                        CreateBullet();
                        break;
                    }
            } */
        }
    }
    Uint8 *keystates = SDL_GetKeyState( NULL );
    if (keystates[SDLK_UP]) {
            my_tank->up(NotWall(my_tank, Forward));
    } else
    if (keystates[SDLK_DOWN]) {
            my_tank->down(NotWall(my_tank, Back));
    } else
    if (keystates[SDLK_RIGHT]) {
            my_tank->right(NotWall(my_tank, Right));
    } else
    if (keystates[SDLK_LEFT]) {
            my_tank->left(NotWall(my_tank, Left));
    }
    return done;
}

void updatePhysic(Tank *my_tank, Tank *gray_tank)
{
    FlyBullet(my_tank, gray_tank);
     FlyBullet(gray_tank, my_tank);
    GoTank(my_tank, gray_tank);
}
void Init(Tank *my_tank, Tank *gray_tank)
{
    // initialize SDL video
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
        printf( "Unable to init SDL: %s\n", SDL_GetError() );
        exit(1);
    }
    // make sure SDL cleans up before exit
    atexit(SDL_Quit);

    if (InitImages(my_tank, gray_tank) < 0) {
        printf("Unable to load bitmap: %s\n", SDL_GetError());
        exit(1);
    }

    // create a new window
    int h = Wall->h;
    int w = Wall->w;
    //SDL_FULLSCREEN - полноэкранный режим
    screen = SDL_SetVideoMode(w * W, h * H, 16, SDL_HWSURFACE | SDL_DOUBLEBUF);
       if ( !screen ) {
        printf("Unable to set 640x480 video: %s\n", SDL_GetError());
        exit(1);
    }
    InitTank(my_tank);
    InitGrayTank(gray_tank);

    if ( !InitWall("map") ) {
        printf("Unable to open file\n");
        exit(1);
    }
}
int main ( int argc, char** argv )
{
    Tank my_tank, gray_tank;
    Init(&my_tank, &gray_tank);

    bool done = false;

    const int TICKS_PER_SECOND = 50;
    const int SKIP_TICKS = 1000 / TICKS_PER_SECOND;
    const int MAX_FRAMESKIP = 10;

    int next_game_tick = SDL_GetTicks();
    int loops;


    while (!done)
    {
        loops = 0;
        while( SDL_GetTicks() > next_game_tick && loops < MAX_FRAMESKIP)
        {
            done = processInput(&my_tank);
            updatePhysic(&my_tank, &gray_tank);
            next_game_tick += SKIP_TICKS;
            loops++;
        }

        Draw(&my_tank, &gray_tank);

        // DRAWING STARTS HERE
        SDL_Flip(screen);
    }

    // free loaded bitmap
    for (int i = 0; i < 4; i++)
        SDL_FreeSurface(my_tank.picture(i));
    for (int i = 0; i < 4; i++)
        SDL_FreeSurface(gray_tank.picture(i));

    SDL_FreeSurface(Wall);
    SDL_FreeSurface(background);
    // all is well ;)
    printf("Exited cleanly\n");
    return 0;
}
