//Using SDL and standard IO
#include <SDL2/SDL.h>
#include <stdio.h>
#include <iostream>
#include<string>
#include <SDL2/SDL_image.h>
//Screen dimension constants
const int SCREEN_WIDTH = 480;
const int SCREEN_HEIGHT = 640;
SDL_Surface* gScreenSurface = NULL;
SDL_Window* gWindow = NULL;
SDL_Surface* gHelloWorld = NULL;
enum KeyPressSurfaces // first is 0, second is 1 and so on
{
    KEY_PRESS_SURFACE_DEFAULT,
    KEY_PRESS_SURFACE_UP,
    KEY_PRESS_SURFACE_DOWN,
    KEY_PRESS_SURFACE_LEFT,
    KEY_PRESS_SURFACE_RIGHT,
    KEY_PRESS_SURFACE_TOTAL
};
bool init(SDL_Window *&gWindow, SDL_Surface *&gSurface)
{
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        success = false;
    }
    else
    {
        //Create window
        gWindow = SDL_CreateWindow( "Tetris", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( gWindow == NULL )
        {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            //Get window surface
            gSurface = SDL_GetWindowSurface( gWindow );
        }
    }
}
int main(){
    SDL_Surface* gSurface = NULL;
    SDL_Window* gWindow = NULL;
    init(gWindow, gSurface);
    if(gSurface == NULL || gWindow == NULL){
        return -1;
    }
    SDL_Event e;
    bool quit = false;
    while( quit == false ){
        SDL_UpdateWindowSurface(gWindow);//Hack to get window to stay up
        while(SDL_PollEvent(&e)){ 
            if( e.type == SDL_QUIT ) {quit = true;} 
        }
    }
    return 0;
}