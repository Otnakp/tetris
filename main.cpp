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
#include "Game.h"
//Screen dimension constants
const int SCREEN_WIDTH = 300;
const int SCREEN_HEIGHT = 2*SCREEN_WIDTH;
const int UNIT = SCREEN_WIDTH / 10; // How big 1 small square is



bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Nothing to load
	return success;
}

SDL_Texture* loadTexture( std::string path, SDL_Renderer *gRenderer)
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
	Game *game = new Game();
		
	//Main loop flag
	bool quit = false;
	//Event handler
	SDL_Event e;
	//While application is running
    float x = 1; float y = 1;
    int w = UNIT*2; int h = UNIT*2;
    auto t1 = std::chrono::high_resolution_clock::now();
    auto t2 = std::chrono::high_resolution_clock::now();
    float delta_time = 0;
    float falling_speed = 30;
	SDL_Renderer *gRenderer = game->get_renderer();
	while( !quit )
	{
        auto start = std::chrono::high_resolution_clock::now();
		while( SDL_PollEvent( &e ) != 0 )
		{
			if( e.type == SDL_QUIT )
			{
				quit = true;
			}
		}
		
		game->render_background();
		game->render_grid(20, 10);
		game->render_square((int)x, (int)y, SCREEN_WIDTH/5, SCREEN_WIDTH/5,0xFF, 0xFF, 0xFF, 0xFF);


		game->render_to_screen();
        auto end = std::chrono::high_resolution_clock::now();
        auto dur = end - start;
        delta_time = std::chrono::duration_cast<std::chrono::duration<float>>(dur).count();                
        y+=(delta_time*falling_speed);
	}
		
	

	return 0;
}