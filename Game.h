#ifndef GAME_H
#define GAME_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
class Game
{
private:
    const int SCREEN_WIDTH = 300;
    const int SCREEN_HEIGHT = 2*SCREEN_WIDTH;
    const int UNIT = SCREEN_WIDTH / 10; // How big 1 small square is
    SDL_Renderer *gRenderer;
    SDL_Window*gWindow;
    bool init();
    SDL_Texture* loadTexture( std::string path, SDL_Renderer *gRenderer);

public:
    Game(/* args */);
    ~Game();
    void render_square(float x, float y, int h, int w, int r, int g, int b, int a);
    void render_background();
    void render_grid(int columns, int rows);
    void render_to_screen(){SDL_RenderPresent(gRenderer);}
    SDL_Renderer*get_renderer(){return gRenderer;}
};


#endif