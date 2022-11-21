#include "Game.h"
Game::Game()
{
	pieces.push_back(Piece("Square"));
	pieces.push_back(Piece("Line"));
	pieces.push_back(Piece("L"));
	pieces.push_back(Piece("Inverse_L"));
	pieces.push_back(Piece("T"));
	P = new bool*[GAME_HEIGHT]; 
	for(int i=0; i<GAME_HEIGHT; i++){
		P[i] = new bool[GAME_WIDTH];
	}
	for(int i=0; i<GAME_WIDTH; i++){
		for(int j=0;j<GAME_HEIGHT;j++){
			P[i][j] = false;
		}
	}
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

bool Game::check_boundary(int bound, bool left){
	int unit_x = (int)((x/SCREEN_WIDTH) * GAME_WIDTH);
	bool limit = false;
	for(auto &p:clone->get_coords()){
		int adjusted_x = unit_x + std::get<0>(p);
		if(left && adjusted_x <= bound){
			return true;
		}
		if(!left && adjusted_x>=bound){
			return true;
		}
	}
	return false;
}

bool Game::check_bottom(){
	for(auto &p:clone->get_coords()){
		if(unit_y + std::get<1>(p) == GAME_HEIGHT){
			return true;
		}
	}
	return false;
}

void Game::handle_input(SDL_Event e, bool*quit){
	if( e.type == SDL_QUIT ){ *quit = true; }
	switch (e.type)
	{
	case SDL_KEYDOWN:
		if(e.key.keysym.sym == SDLK_a){
			if(!check_boundary(0, true)){
				x -= UNIT;
			}
		}
		if(e.key.keysym.sym == SDLK_d){
			if(!check_boundary(GAME_WIDTH-1, false)){
				x += UNIT;
			}
		}
		if(e.key.keysym.sym == SDLK_s){
			falling_speed = faster_falling_speed;
		}
		break;
	case SDL_KEYUP:
		if(e.key.keysym.sym == SDLK_s){
			falling_speed = base_falling_speed;
		}
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
    int w = UNIT*2; 
	int h = UNIT*2;
    float delta_time = 0;
	SDL_Renderer *gRenderer = get_renderer();
	bool spawn_new_piece = true;
	int r =0;
	int c = 0;
	while( !quit )
	{
        auto start = std::chrono::high_resolution_clock::now();
		if(SDL_PollEvent(&e))
		{
			handle_input(e, &quit);
		}
		render_background();
		render_grid(GAME_HEIGHT, GAME_WIDTH);

		if(spawn_new_piece){
			std::uniform_int_distribution<int> piece_generator(0,NUMBER_OF_PIECES - 1); // Guaranteed unbiased
			r = piece_generator(rng);
			clone = pieces[r].clone();
			y = 1;
			x = 2 + (SCREEN_WIDTH / 2) - ((int)clone->get_width()/2) - UNIT;
			spawn_new_piece = false;
		}

		render_piece(x, y, pieces[r].get_coords());
		render_P();
		render_to_screen();
        auto end = std::chrono::high_resolution_clock::now();
        auto dur = end - start;
        delta_time = std::chrono::duration_cast<std::chrono::duration<float>>(dur).count();               
        y+=(delta_time*falling_speed);
		unit_y = (int)((y/SCREEN_HEIGHT) * GAME_HEIGHT) + clone->get_height(); 
		unit_x = (int)((x/SCREEN_WIDTH) * GAME_WIDTH);
		
		for(auto &p : pieces[r].get_coords()){
			// instead of checking all the boolean things
			// we just check the neighbours of the piece
			if(check_bottom()){
				spawn_new_piece = true;
				break;
			}
			int adjusted_x = unit_x + std::get<0>(p);
			int adjusted_y = unit_y -  clone->get_height()+ std::get<1>(p) + 1; // +1 to check below 1
			if(P[adjusted_x][adjusted_y]){
				spawn_new_piece = true;
				break;
			}
		}

		if(spawn_new_piece){
			for(auto &p : pieces[r].get_coords()){
				int adjusted_x = unit_x + std::get<0>(p);
				int adjusted_y = unit_y -  clone->get_height() + std::get<1>(p); 
				P[adjusted_x][adjusted_y] = true;
			}
		}

		if(spawn_new_piece){
			for(int j=0;j<GAME_HEIGHT;j++){
				for(int i=0; i<GAME_WIDTH; i++){
					std::cout<<(int)P[i][j]<<" ";
				}
				std::cout<<std::endl;
			}
			std::cout<<std::endl;
		}
		//if(y>SCREEN_HEIGHT - ( clone->get_height() * UNIT) - 1){
		//	y = SCREEN_HEIGHT - ( clone->get_height() * UNIT) - 1;
		//	
		//}
	}
	// TODO: stop at floor and accumulate pieces (you need them to make tetris)
	// TODO: spawn more than one piece
	// TODO: Make pieces stack
	// TODO: Make that if there's a line, it gets deleted
}
void Game::render_P(){
	for(int i=0; i<GAME_WIDTH; i++){
		for(int j=0;j<GAME_HEIGHT;j++){
			if(P[i][j]){
				SDL_Rect fillRect = { i*UNIT, j*UNIT, UNIT, UNIT};
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );		
				SDL_RenderFillRect( gRenderer, &fillRect );
			}
		}
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
