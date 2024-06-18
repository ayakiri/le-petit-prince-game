#ifndef LE_PETIT_PRINCE_GAME_FOX_H
#define LE_PETIT_PRINCE_GAME_FOX_H
#include <array>
#include <cmath>
#include "map.h"
#include "rose.h"

struct Fox {
    enum States { REST=0, JUMP=1, RUN=2, FALL=3 };

    Fox(SDL_Renderer *renderer) :
            renderer(renderer),
            sprites{Animation(renderer, "assets/fox_idle.bmp",    128, 1.0, true ),
                    Animation(renderer, "assets/fox_jump.bmp",  128, 1.3, false ),
                    Animation(renderer, "assets/fox_run.bmp",  128, 1.0, true ),
                    Animation(renderer, "assets/fox_fall.bmp",  128, 1.0, true )} {
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

    void handle_keyboard(const double dt, const Map &map) {
        const Uint8 *kbstate = SDL_GetKeyboardState(NULL);
        if (state==RUN && !kbstate[SDL_SCANCODE_RIGHT] && !kbstate[SDL_SCANCODE_LEFT])
            set_state(REST);
        if ((state==REST || state==RUN) && kbstate[SDL_SCANCODE_UP]) {
            if (kbstate[SDL_SCANCODE_LEFT] || kbstate[SDL_SCANCODE_RIGHT]) {
                jumpvx =  200; // long jump
                jumpvy = -200;
            } else {
                jumpvx =   50; // high jump
                jumpvy = -300;
            }
            set_state(JUMP);
        }
        if (state==REST && (kbstate[SDL_SCANCODE_LEFT] || kbstate[SDL_SCANCODE_RIGHT])) {
            backwards = kbstate[SDL_SCANCODE_LEFT];
            set_state(RUN);
        }
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

    // zwykła kolizja - do poprawy
    bool collides_with(const Rose& rose) const {
        return !(x + sprite_w / 2 < rose.x || x - sprite_w / 2 > rose.x + rose.width ||
                 y < rose.y || y - sprite_h > rose.y + rose.height);
    }

    void draw() {
        SDL_Rect src = sprites[state].rect(timestamp);
        SDL_Rect dest = { int(x)-sprite_w/2, int(y)-sprite_h, sprite_w, sprite_h };
        SDL_RenderCopyEx(renderer, sprites[state].texture, &src, &dest, 0, nullptr, backwards ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
    }

    double x = 750, y = 200; // coordinates of the character
    double vx = 0, vy = 0;   // speed
    bool backwards = true;  // facing left or right
    double jumpvx = 0, jumpvy = 0; // will be used to differentiate high jump from a long jump

    int state = REST;         // current sprite
    TimeStamp timestamp = Clock::now();
    SDL_Renderer *renderer;

    const int sprite_w = 96; // size of the sprite on the screen
    const int sprite_h = 96;
    const std::array<Animation,4> sprites; // sprite sequences to be drawn
};
#endif //LE_PETIT_PRINCE_GAME_FOX_H
