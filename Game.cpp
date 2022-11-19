#include "Game.h"
Game::Game()
{
	pieces.push_back(Piece("Square"));
	pieces.push_back(Piece("Line"));
	pieces.push_back(Piece("L"));
	pieces.push_back(Piece("Inverse_L"));
	pieces.push_back(Piece("T"));
    this->init();
}

Game::~Game()
{
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;
	IMG_Quit();
	SDL_Quit();
}

void Game::handle_input(SDL_Event e, bool*quit){
	if( e.type == SDL_QUIT ){ *quit = true; }
	switch (e.type)
	{
	case SDL_KEYDOWN:
		if(e.key.keysym.sym == SDLK_a){
			x -= UNIT * (int)(x>1);
		}
		if(e.key.keysym.sym == SDLK_d){
			x += UNIT * (int)(x<(SCREEN_WIDTH - UNIT*current_piece_width));
		}
		break;
	case SDL_KEYUP:
        break;
	default:
		break;
	}
}

void Game::run(){
	std::random_device rd;
	std::mt19937 rng(rd());
	bool quit = false;
	SDL_Event e;
	float y = 1;
    int w = UNIT*2; 
	int h = UNIT*2;
    float delta_time = 0;
    float falling_speed = 30;
	SDL_Renderer *gRenderer = get_renderer();
	bool spawn_new_piece = true;
	int r =0;
	while( !quit )
	{
        auto start = std::chrono::high_resolution_clock::now();
		if( SDL_PollEvent( &e ) )
		{
			handle_input(e, &quit);
		}
		render_background();
		render_grid(20, 10);

		if(spawn_new_piece){
			std::uniform_int_distribution<int> piece_generator(0,4); // Guaranteed unbiased
			r = piece_generator(rng);
			current_piece_width = pieces[r].get_width();
			x = 2 + (SCREEN_WIDTH / 2) - ((int)current_piece_width/2) - UNIT;
			spawn_new_piece = false;
		}

		render_piece(x,y,pieces[r].get_coords());
		//render_L_inverse(x,y);
		


		render_to_screen();
        auto end = std::chrono::high_resolution_clock::now();
        auto dur = end - start;
        delta_time = std::chrono::duration_cast<std::chrono::duration<float>>(dur).count();               
        y+=(delta_time*falling_speed);
	}
}

bool Game::init()
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


void Game::render_piece(float x, float y, std::vector<std::tuple<int,int>> coords){
	for(int i = 0; i<coords.size(); i++){
		int a = std::get<0>(coords[i]);
		int b = std::get<1>(coords[i]);
		SDL_Rect fillRect = { (int)x + UNIT * a, (int)y+ UNIT*b, UNIT, UNIT};
		SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );		
		SDL_RenderFillRect( gRenderer, &fillRect );
	}
}

SDL_Texture* Game::loadTexture( std::string path)
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

void Game::render_square(float x, float y, int h, int w, int r, int g, int b, int a){
    SDL_Rect fillRect = { (int)x, (int)y, w, h}; // x,y,w,h -> position, dimensions
	SDL_SetRenderDrawColor( gRenderer, r, g, b, a );		
	SDL_RenderFillRect( gRenderer, &fillRect );
}

void Game::render_background(){
	SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0x00);
	SDL_RenderClear(gRenderer);
}
void Game::render_grid(int columns, int rows){
	SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0xFF, 0xFF );		
	for(int i=0; i<columns; i++){
		SDL_RenderDrawLine( gRenderer, 0, UNIT*i, SCREEN_WIDTH, UNIT*i);
	}
	SDL_RenderDrawLine( gRenderer, 0, UNIT*20-1, SCREEN_WIDTH, UNIT*20-1);
	for(int i=0; i<rows; i++){
		SDL_RenderDrawLine( gRenderer, UNIT*i, 0, UNIT*i, SCREEN_HEIGHT);
	}
	SDL_RenderDrawLine( gRenderer, UNIT*10-1, 0, UNIT*10-1, SCREEN_HEIGHT);
}

void Game::render_square(float x, float y){
	SDL_Rect fillRect = { (int)x, (int)y, SCREEN_WIDTH/5, SCREEN_WIDTH/5}; // x,y,w,h -> position, dimensions
	SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );		
	SDL_RenderFillRect( gRenderer, &fillRect );
}

void Game::render_t(float x, float y){
	SDL_Rect fillRect = { (int)x, (int)y, UNIT, UNIT};
	SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );		
	SDL_RenderFillRect( gRenderer, &fillRect );

	fillRect = { (int)x + UNIT, (int)y, UNIT, UNIT};
	SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );		
	SDL_RenderFillRect( gRenderer, &fillRect );

	fillRect = { (int)x + UNIT*2, (int)y, UNIT, UNIT};
	SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );		
	SDL_RenderFillRect( gRenderer, &fillRect );

	fillRect = { (int)x + UNIT, (int)y + UNIT, UNIT, UNIT};
	SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );		
	SDL_RenderFillRect( gRenderer, &fillRect );

}

void Game::render_line(float x, float y){
	for(int i=0; i<4; i++){
		SDL_Rect fillRect = { (int)x + UNIT*i, (int)y, UNIT, UNIT};
		SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );		
		SDL_RenderFillRect( gRenderer, &fillRect );
	}
}

void Game::render_L(float x, float y){
	for(int i=0; i<3; i++){
		SDL_Rect fillRect = { (int)x, (int)y*i, UNIT, UNIT};
		SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );		
		SDL_RenderFillRect( gRenderer, &fillRect );
	}
	SDL_Rect fillRect = { (int)x + UNIT, (int)y + UNIT*2, UNIT, UNIT};
	SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );		
	SDL_RenderFillRect( gRenderer, &fillRect );

}

void Game::render_L_inverse(float x, float y){
	for(int i=0; i<3; i++){
		SDL_Rect fillRect = { (int)x + UNIT, (int)y+ UNIT*i, UNIT, UNIT};
		SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );		
		SDL_RenderFillRect( gRenderer, &fillRect );
	}
	SDL_Rect fillRect = { (int)x, (int)y + UNIT*2, UNIT, UNIT};
	SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );		
	SDL_RenderFillRect( gRenderer, &fillRect );
}
