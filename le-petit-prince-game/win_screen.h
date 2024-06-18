#ifndef WIN_SCREEN_H
#define WIN_SCREEN_H

#include <SDL.h>
#include <memory>
#include <string>

std::shared_ptr<SDL_Texture> load_image(SDL_Renderer *renderer, const std::string &file_path);

int show_win_screen(SDL_Renderer* renderer, SDL_Texture* win_background, const Time_Counter& time_counter) {
    SDL_Event event;

    while (true) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    return 0;
                case SDL_KEYDOWN:
                    if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) return 1;
                    if (event.key.keysym.scancode == SDL_SCANCODE_Q) return 0;
                break;
            }
        }

        SDL_RenderClear(renderer); // Clear the screen
        SDL_RenderCopy(renderer, win_background, NULL, NULL);
        time_counter.render(512, 494);
        SDL_RenderPresent(renderer); // Present the screen
    }
}

#endif // WIN_SCREEN_H
