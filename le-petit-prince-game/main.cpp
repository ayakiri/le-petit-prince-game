#include <SDL.h>
#include <iostream>
#include <memory>
#include <chrono>
#include <thread>
#include "time_counter.h"
#include "map.h"
#include "fox.h"

std::shared_ptr<SDL_Texture> load_image(SDL_Renderer *renderer, const std::string &file_path) {
    SDL_Surface *surface;
    SDL_Texture *texture;

    surface = SDL_LoadBMP(file_path.c_str());
    if (!surface) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create surface from image: %s", SDL_GetError());
        throw std::invalid_argument(SDL_GetError());
    }
    SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format,0, 255, 255));
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create texture from surface: %s", SDL_GetError());
        throw std::invalid_argument(SDL_GetError());
    }
    SDL_FreeSurface(surface);
    return {texture, [](SDL_Texture *t) {
        SDL_DestroyTexture(t);
    }};
}


int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_Window   *window   = nullptr;
    SDL_Renderer *renderer = nullptr;
    double dt = 1./60.;

    if (SDL_CreateWindowAndRenderer(1024, 768, SDL_WINDOW_SHOWN | SDL_WINDOW_INPUT_FOCUS, &window, &renderer)) {
        std::cerr << "Failed to create window and renderer: " << SDL_GetError() << std::endl;
        return -1;
    }
    SDL_SetWindowTitle(window, "Eclipse game");

    auto background_texture = load_image(renderer, "assets/background.bmp");

    bool still_playing = true;
    int x = 100;
    int y = 100;
    double game_time = 0.;
    std::chrono::steady_clock::time_point current_time = std::chrono::steady_clock::now();
    Time_Counter time_counter(renderer);
    Map map(renderer);
    Fox fox(renderer);

    while (still_playing) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    still_playing = false;
                    break;
                case SDL_KEYUP:
                    if (event.key.keysym.scancode == SDL_SCANCODE_Q) still_playing = false;
                    break;
            }
        }

        game_time += dt;

        SDL_RenderClear(renderer); // re-draw the window
        SDL_RenderCopy(renderer, background_texture.get(), NULL, NULL);
        time_counter.draw();
        fox.draw();
        map.draw();
        SDL_RenderPresent(renderer);

        current_time = current_time + std::chrono::microseconds((long long int)(dt*1000000.0));
        std::this_thread::sleep_until(current_time);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

