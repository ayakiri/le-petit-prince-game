#include <SDL.h>
#include <iostream>
#include <memory>
#include <chrono>
#include <thread>
#include "time_counter.h"
#include "map.h"
#include "fox.h"
#include "prince.h"
#include "menu.h"


std::shared_ptr<SDL_Texture> load_image(SDL_Renderer *renderer, const std::string &file_path) {
    SDL_Surface *surface;
    SDL_Texture *texture;

    surface = SDL_LoadBMP(file_path.c_str());
    if (!surface) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create surface from image: %s", SDL_GetError());
        throw std::invalid_argument(SDL_GetError());
    }
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
        SDL_Quit();
        return -1;
    }
    SDL_SetWindowTitle(window, "Le petit prince game");

    auto background_texture = load_image(renderer, "assets/background.bmp");
    auto menu_background_texture = load_image(renderer, "assets/menu_background.bmp");
    auto easy_texture = load_image(renderer, "assets/button_easy.bmp");
    auto easy_active_texture = load_image(renderer, "assets/button_easy_active.bmp");
    auto medium_texture = load_image(renderer, "assets/button_medium.bmp");
    auto medium_active_texture = load_image(renderer, "assets/button_medium_active.bmp");
    auto hard_texture = load_image(renderer, "assets/button_hard.bmp");
    auto hard_active_texture = load_image(renderer, "assets/button_hard_active.bmp");

    // Instantiate Menu with loaded textures
    Menu menu(renderer, menu_background_texture.get(),
              easy_texture.get(), easy_active_texture.get(),
              medium_texture.get(), medium_active_texture.get(),
              hard_texture.get(), hard_active_texture.get());
    menu.show();
    int chosen_level = menu.get_chosen_level();
    const char *current_level;
    switch (chosen_level) {
    case 0:
        current_level = Map::level_easy;
        break;
    case 1:
        current_level = Map::level_medium;
        break;
    case 2:
        current_level = Map::level_hard;
        break;
    default:
        current_level = Map::level_easy;
        break;
    }

    bool still_playing = true;
    double game_time = 0.;
    auto current_time = std::chrono::steady_clock::now();
    Time_Counter time_counter(renderer);
    Map map(renderer, current_level);
    Fox fox(renderer);
    Prince prince(renderer);

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
        fox.handle_keyboard(dt, map);
        prince.handle_keyboard(dt, map);

        game_time += dt;


        SDL_RenderClear(renderer); // re-draw the window
        SDL_RenderCopy(renderer, background_texture.get(), NULL, NULL);
        time_counter.draw();
        map.draw();
        fox.draw();
        prince.draw();

        SDL_RenderPresent(renderer);

        current_time = current_time + std::chrono::microseconds((long long int)(dt*1000000.0));
        std::this_thread::sleep_until(current_time);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
