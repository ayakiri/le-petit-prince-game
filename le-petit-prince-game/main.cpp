#include <SDL.h>
#include <iostream>
#include "time_counter.h"

void main_loop(SDL_Renderer *renderer, Time_Counter &time_counter, SDL_Texture* background_texture) {
    while (true) {
        SDL_Event event;
        if (SDL_PollEvent(&event) && (SDL_QUIT==event.type || (SDL_KEYDOWN==event.type && SDLK_ESCAPE==event.key.keysym.sym)))
            break;
        SDL_RenderClear(renderer); // re-draw the window
        SDL_RenderCopy(renderer, background_texture, NULL, NULL);
        time_counter.draw();
        SDL_RenderPresent(renderer);
    }
}

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_Window   *window   = nullptr;
    SDL_Renderer *renderer = nullptr;
    if (SDL_CreateWindowAndRenderer(1024, 768, SDL_WINDOW_SHOWN | SDL_WINDOW_INPUT_FOCUS, &window, &renderer)) {
        std::cerr << "Failed to create window and renderer: " << SDL_GetError() << std::endl;
        return -1;
    }
    SDL_SetWindowTitle(window, "Eclipse game");

    SDL_Surface *background_surface = SDL_LoadBMP("assets/background.bmp");
    if (!background_surface) {
        std::cerr << "Failed to load background image: " << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }
    SDL_Texture *background_texture = SDL_CreateTextureFromSurface(renderer, background_surface);
    SDL_FreeSurface(background_surface);
    if (!background_texture) {
        std::cerr << "Failed to create background texture: " << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    Time_Counter time_counter(renderer);
    main_loop(renderer, time_counter, background_texture);

    SDL_DestroyTexture(background_texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
