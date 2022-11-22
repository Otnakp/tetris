/*This source code copyrighted by Lazy Foo' Productions (2004-2022)
and may not be redistributed without written permission.*/
#include <SDL2/SDL.h>
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
int main( int argc, char* args[] )
{
	//Start up SDL and create window
	Game *game = new Game();
	//Main loop flag
	game->run();
	return 0;
}