/*This source code copyrighted by Lazy Foo' Productions (2004-2022)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, math, and strings
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <cmath>
#include<iostream>
#include <chrono>
//Screen dimension constants
const int SCREEN_WIDTH = 300;
const int SCREEN_HEIGHT = 2*SCREEN_WIDTH;
const int UNIT = SCREEN_WIDTH / 10; // How big 1 small square is

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//Loads individual image as texture
SDL_Texture* loadTexture( std::string path );

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "Tetris", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Nothing to load
	return success;
}

void close()
{
	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

SDL_Texture* loadTexture( std::string path )
{
	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	return newTexture;
}

int main( int argc, char* args[] )
{
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
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//While application is running
            float x = 0; float y = 0;
            int w = UNIT*2; int h = UNIT*2;
            auto t1 = std::chrono::high_resolution_clock::now();
            auto t2 = std::chrono::high_resolution_clock::now();
            float delta_time = 0;
            float falling_speed = 30;
			while( !quit )
			{
				//Handle events on queue

                auto start = std::chrono::high_resolution_clock::now();
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}
				}

				//Clear screen
				SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0x00);
				SDL_RenderClear(gRenderer);
				////Render red filled quad
				

				
				SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0xFF, 0xFF );		
				for(int i=0; i<20; i++){
					SDL_RenderDrawLine( gRenderer, 0, UNIT*i, SCREEN_WIDTH, UNIT*i);
				}
				for(int i=0; i<10; i++){
					SDL_RenderDrawLine( gRenderer, UNIT*i, 0, UNIT*i, SCREEN_HEIGHT);
				}

				SDL_Rect fillRect = { (int)x, (int)y, SCREEN_WIDTH/5, SCREEN_WIDTH/5}; // x,y,w,h -> position, dimensions
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );		
				SDL_RenderFillRect( gRenderer, &fillRect );
				//Update screen
				SDL_RenderPresent(gRenderer);

                auto end = std::chrono::high_resolution_clock::now();
                auto dur = end - start;
                delta_time = std::chrono::duration_cast<std::chrono::duration<float>>(dur).count();                
                y+=(delta_time*falling_speed);
			}
		}
	}
    /*
    
	////Render green outlined quad
	//SDL_Rect outlineRect = { SCREEN_WIDTH / 6, SCREEN_HEIGHT / 6, SCREEN_WIDTH * 2 / 3, SCREEN_HEIGHT * 2 / 3 };
	//SDL_SetRenderDrawColor( gRenderer, 0x00, 0xFF, 0x00, 0xFF );		
	//SDL_RenderDrawRect( gRenderer, &outlineRect );
	//
	////Draw blue horizontal line
	//SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0xFF, 0xFF );		
	//SDL_RenderDrawLine( gRenderer, 0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2 );
    //
	////Draw vertical line of yellow dots
	//SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0x00, 0xFF );
	//for( int i = 0; i < SCREEN_HEIGHT; i += 4 )
	//{
	//	SDL_RenderDrawPoint( gRenderer, SCREEN_WIDTH / 2, i );
	//}

    */
	//Free resources and close SDL
	close();

	return 0;
}