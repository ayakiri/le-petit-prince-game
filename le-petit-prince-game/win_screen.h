#ifndef WIN_SCREEN_H
#define WIN_SCREEN_H

#include <SDL.h>
#include <memory>
#include <string>

std::shared_ptr<SDL_Texture> load_image(SDL_Renderer *renderer, const std::string &file_path);

void show_win_screen(SDL_Renderer* renderer, SDL_Texture* win_background) {
    SDL_Event event;

    while (true) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    return;
                case SDL_KEYDOWN:
                    if (event.key.keysym.scancode == SDL_SCANCODE_RETURN) {
                        return; // End win screen on Enter key press
                    }
                break;
                // dodać powrót do menu?
            }
        }

        SDL_RenderClear(renderer); // Clear the screen
        SDL_RenderCopy(renderer, win_background, NULL, NULL);
        SDL_RenderPresent(renderer); // Present the screen
    }
}

#endif // WIN_SCREEN_H
