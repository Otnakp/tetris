//Using SDL and standard IO
#include <SDL2/SDL.h>
#include <stdio.h>
#include <SDL2/SDL_image.h>
//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
SDL_Surface* gScreenSurface = NULL;
SDL_Window* gWindow = NULL;
SDL_Surface* gHelloWorld = NULL;
bool init()
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
        gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( gWindow == NULL )
        {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            //Get window surface
            gScreenSurface = SDL_GetWindowSurface( gWindow );
        }
    }

    return success;
}

bool loadMedia()
{
    //Loading success flag
    bool success = true;

    //Load splash image
    gHelloWorld = IMG_Load("../images/cat.png" );
    if( gHelloWorld == NULL )
    {
        printf( "Unable to load image %s! SDL Error: %s\n", "02_getting_an_image_on_the_screen/hello_world.bmp", SDL_GetError() );
        success = false;
    }

    return success;
}
void close()
{
    //Deallocate surface
    SDL_FreeSurface( gHelloWorld );
    gHelloWorld = NULL;

    //Destroy window
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;

    //Quit SDL subsystems
    SDL_Quit();
}
int main( int argc, char* args[] )
{
    ////The window we'll be rendering to
    //SDL_Window* window = NULL;
    //
    ////The surface contained by the window
    //SDL_Surface* screenSurface = NULL;
    ////Initialize SDL
    //if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    //{
    //    printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
    //}
    // else
    //{
    //    //Create window
    //    window = SDL_CreateWindow( "Tetris", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
    //    if( window == NULL )
    //    {
    //        printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
    //    }
    //     else
    //    {
    //        //Get window surface
    //        screenSurface = SDL_GetWindowSurface( window );
//
    //        //Fill the surface white
    //        SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0xFF, 0xFF, 0xFF ) );
    //        
    //        //Update the surface
    //        SDL_UpdateWindowSurface( window );
//
    //        //Hack to get window to stay up
    //        SDL_Event e; 
    //        bool quit = false; 
    //        while( quit == false ){ 
    //            while( SDL_PollEvent( &e ) ){ 
    //                if( e.type == SDL_QUIT ) quit = true; 
    //            } 
    //        }
    //    }
    //}
    //    //Destroy window
    //SDL_DestroyWindow( window );
//
    ////Quit SDL subsystems
    //SDL_Quit();
    //Start up SDL and create window
    if( !init() )
    {
        printf( "Failed to initialize!\n" );
    }
    else
    {
        //Load media
        if( !loadMedia() )
        {
            printf( "Failed to load media!\n" );
        }
        else
        {
            //Apply the image
            SDL_BlitSurface( gHelloWorld, NULL, gScreenSurface, NULL );
             //Update the surface
            SDL_UpdateWindowSurface( gWindow );
                        //Hack to get window to stay up
            SDL_Event e; bool quit = false; while( quit == false ){ while( SDL_PollEvent( &e ) ){ if( e.type == SDL_QUIT ) quit = true; } }
        }
    }

    //Free resources and close SDL
    close();

    return 0;

    return 0;
}