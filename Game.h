#ifndef GAME_H
#define GAME_H
#include "Piece.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <chrono>
#include <vector>
#include <iostream>
#include <map>
#include <tuple>
#include <random>
class Game
{

public:
    const int GAME_WIDTH = 10;
    const int GAME_HEIGHT = 20;
    const int SCREEN_WIDTH = 300;
    const int SCREEN_HEIGHT = 2*SCREEN_WIDTH;
    const int UNIT = SCREEN_WIDTH / GAME_WIDTH; // How big 1 small square is
private:
    SDL_Renderer *gRenderer;
    SDL_Window*gWindow;
    bool init();
    void handle_input(SDL_Event e,  bool *quit);
    float falling_speed = 30;
    float base_falling_speed = 50;
    float faster_falling_speed = 300;
	float y = 1;
    float x = 1;
    int current_piece_width = 2;
    int current_piece_height = 2;
    SDL_Texture* loadTexture( std::string path, SDL_Renderer *gRenderer);
    std::vector<Piece> pieces;
    bool **P;
public:
    Game();
    ~Game();
    void run();
    SDL_Texture*loadTexture( std::string path);
    void render_piece(float x, float y, std::vector<std::tuple<int,int>> coords);
    void render_square(float x, float y, int h, int w, int r, int g, int b, int a);
    void render_square(float x, float y);
    void render_t(float x, float y);
    void render_line(float x, float y);
    void render_L(float x, float y);
    void render_L_inverse(float x, float y);
    void render_background();
    void render_grid(int columns, int rows);
    void render_P();
    void render_to_screen(){SDL_RenderPresent(gRenderer);}
    SDL_Renderer*get_renderer(){return gRenderer;}
};
#endif