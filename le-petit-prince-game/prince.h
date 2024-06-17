#ifndef LE_PETIT_PRINCE_GAME_PRINCE_H
#define LE_PETIT_PRINCE_GAME_PRINCE_H
#include <array>
#include <cmath>
#include "map.h"

struct Prince{
        enum States { REST=0, JUMP=1, RUN=2, FALL=3 };

    Prince(SDL_Renderer *renderer) :
    renderer(renderer),
    sprites{Animation(renderer, "assets/prince_idle.bmp",    128, 1.0, true ),
            Animation(renderer, "assets/prince_jump.bmp",  128, 1.3, false ),
            Animation(renderer, "assets/prince_run.bmp",  128, 1.0, true ),
            Animation(renderer, "assets/prince_fall.bmp",  128, 1.0, true )} {
    }

    void set_state(int s) {
        timestamp = Clock::now();
        state = s;
        if (state!=JUMP && state!=RUN)
            vx = 0;
        else if (state==RUN)
            vx = backwards ? -150 : 150;
        else if (state==JUMP) {
            vy = jumpvy;
            vx = backwards ? -jumpvx : jumpvx;
        }
    }

    void handle_keyboard() {
        const Uint8 *kbstate = SDL_GetKeyboardState(NULL);
        if (state==RUN && !kbstate[SDL_SCANCODE_D] && !kbstate[SDL_SCANCODE_A])
            set_state(REST);
        if ((state==REST || state==RUN) && kbstate[SDL_SCANCODE_W]) {
            if (kbstate[SDL_SCANCODE_A] || kbstate[SDL_SCANCODE_D]) {
                jumpvx =  200; // long jump
                jumpvy = -200;
            } else {
                jumpvx =   50; // high jump
                jumpvy = -300;
            }
            set_state(JUMP);
        }
        if (state==REST && (kbstate[SDL_SCANCODE_A] || kbstate[SDL_SCANCODE_D])) {
            backwards = kbstate[SDL_SCANCODE_A];
            set_state(RUN);
        }
    }

    void update_state(const double dt, const Map &map) {
        if (state==JUMP && sprites[state].animation_ended(timestamp))
            set_state(REST);
        if (state!=JUMP && map.is_empty(x/map.tile_w, y/map.tile_h + 1))
            set_state(FALL);   // put free falling sprite if no ground under the feet

        x += dt*vx; // prior to collision detection
        if (!map.is_empty(x/map.tile_w, y/map.tile_h)) { // horizontal collision detection
            int snap = std::round(x/map.tile_w)*map.tile_w; // snap the coorinate to the boundary of last free tile
            x = snap + (snap>x ? 1 : -1);
            vx = 0; // stop
        }

        y  += dt*vy;  // prior to collision detection
        vy += dt*300; // gravity
        if (!map.is_empty(x/map.tile_w, y/map.tile_h)) { // vertical collision detection
            int snap = std::round(y/map.tile_h)*map.tile_h;   // snap the coorinate to the boundary of last free tile
            y = snap + (snap>y ? 1 : -1);
            vy = 0; // stop
            if (state==JUMP || state==FALL)
                set_state(REST);
        }
    }


    void draw() {
        SDL_Rect src = sprites[state].rect(timestamp);
        SDL_Rect dest = { int(x)-sprite_w, int(y)-sprite_h, sprite_w, sprite_h };
        SDL_RenderCopyEx(renderer, sprites[state].texture, &src, &dest, 0, nullptr, backwards ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
    }

    double x = 200, y = 200; // coordinates of the character
    double vx = 0, vy = 0;   // speed
    bool backwards = false;  // facing left or right
    double jumpvx = 0, jumpvy = 0; // will be used to differentiate high jump from a long jump

    int state = REST;         // current sprite
    TimeStamp timestamp = Clock::now();
    SDL_Renderer *renderer;

    const int sprite_w = 128; // size of the sprite on the screen
    const int sprite_h = 128;
    const std::array<Animation,4> sprites; // sprite sequences to be drawn
};
#endif //LE_PETIT_PRINCE_GAME_PRINCE_H