#ifndef LE_PETIT_PRINCE_GAME_FOX_H
#define LE_PETIT_PRINCE_GAME_FOX_H
#include <array>
#include <cmath>
#include "map.h"

struct Fox {
    enum States {
        REST=0, JUMP=1, RUN=2
    };

    Fox(SDL_Renderer *renderer) :
            renderer(renderer),
            sprites{Animation(renderer, "assets/fox_idle.bmp",    128, 1.0, true ),
                    Animation(renderer, "assets/fox_jump.bmp",  128, 1.3, false ),
                    Animation(renderer, "assets/fox_run.bmp",  128, 1.0, true )} {
    }

    void set_state(int s) {
        timestamp = Clock::now();
        state = s;
        if (state==REST)
            vx = 0;
        if (state==RUN)
            vx = backwards ? -150 : 150;
    }

    void handle_keyboard() {
        const Uint8 *kbstate = SDL_GetKeyboardState(NULL);
        if (state==RUN && !kbstate[SDL_SCANCODE_RIGHT] && !kbstate[SDL_SCANCODE_LEFT])
            set_state(REST);
        if (state==REST && (kbstate[SDL_SCANCODE_LEFT] || kbstate[SDL_SCANCODE_RIGHT])) {
            backwards = kbstate[SDL_SCANCODE_LEFT];
            set_state(RUN);
        }
    }

    void update_state(const double dt, const Map &map) {
        double cand_x = x + dt*vx; // candidate coordinates prior to collision detection

        if (map.is_empty(x/map.tile_w, y/map.tile_h + 1))
            set_state(JUMP);   // put free falling sprite if no ground under the feet

        double cand_y = y + dt*vy;
        vy += dt*300; // gravity


        if (!map.is_empty(cand_x/map.tile_w, y/map.tile_h)) { // horizontal collision detection
            int snap = std::round(cand_x/map.tile_w)*map.tile_w; // snap the coorinate to the boundary of last free tile
            cand_x = snap + (snap>cand_x ? 1 : -1);
            vx = 0; // stop
        }

        if (!map.is_empty(cand_x/map.tile_w, cand_y/map.tile_h)) { // vertical collision detection
            if (state==JUMP)
                set_state(REST);
            vy = 0; // stop
            int snap = std::round(cand_y/map.tile_h)*map.tile_h; // snap the coorinate to the boundary of last free tile
            cand_y = snap + (snap>cand_y ? 1 : -1);// be careful to snap to the left or to the right side of the free tile
        }


        x = cand_x; // final coordinates post-collision detection
        y = cand_y;
    }


    void draw() {
        SDL_Rect src = sprites[state].rect(timestamp);
        SDL_Rect dest = { int(x)-sprite_w, int(y)-sprite_h, sprite_w, sprite_h };
        SDL_RenderCopyEx(renderer, sprites[state].texture, &src, &dest, 0, nullptr, backwards ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
    }

    double x = 150, y = 200; // coordinates of the player
    double vx = 150, vy = 200; //speed
    bool backwards = false;  // left or right

    int state = REST;
    TimeStamp timestamp = Clock::now();

    const int sprite_w = 128; // size of the sprite on the screen
    const int sprite_h = 128;

    SDL_Renderer *renderer;   // draw here
    std::array<Animation,3> sprites; // sprite sequences to be drawn
};
#endif //LE_PETIT_PRINCE_GAME_FOX_H
