#ifndef LE_PETIT_PRINCE_GAME_FOX_H
#define LE_PETIT_PRINCE_GAME_FOX_H
#include <array>
#include <cmath>
#include "map.h"

struct Fox {
    enum States {
        REST=0, JUMP=2
    };

    Fox(SDL_Renderer *renderer) :
            renderer(renderer),
            sprites{Animation(renderer, "assets/fox_idle.bmp",    128, 1.0, true ),
                    Animation(renderer, "assets/fox_jump.bmp",  128, 1.3, false )} {
    }

    void draw() {
        SDL_Rect src = sprites[state].rect(timestamp);
        SDL_Rect dest = { int(x)-sprite_w, int(y)-sprite_h, sprite_w, sprite_h };
        SDL_RenderCopyEx(renderer, sprites[state].texture, &src, &dest, 0, nullptr, backwards ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
    }

    double x = 150, y = 200; // coordinates of the player
    bool backwards = false;  // left or right

    int state = REST;
    TimeStamp timestamp = Clock::now();

    const int sprite_w = 128; // size of the sprite on the screen
    const int sprite_h = 128;

    SDL_Renderer *renderer;   // draw here
    std::array<Animation,2> sprites; // sprite sequences to be drawn
};
#endif //LE_PETIT_PRINCE_GAME_FOX_H
