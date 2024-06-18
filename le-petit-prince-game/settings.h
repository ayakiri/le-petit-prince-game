#ifndef SETTINGS_H
#define SETTINGS_H

#include <SDL.h>

class Settings {
public:
    Settings(SDL_Renderer *renderer, SDL_Texture *background_texture, SDL_Texture *back_button_texture, SDL_Texture *back_button_active_texture)
        : renderer(renderer), background_texture(background_texture),
          back_button_texture(back_button_texture), back_button_active_texture(back_button_active_texture) {}

    ~Settings() {
        SDL_DestroyTexture(background_texture);
        SDL_DestroyTexture(back_button_texture);
        SDL_DestroyTexture(back_button_active_texture);
    }

    void show() {
        bool in_settings = true;
        int selected = 0;

        while (in_settings) {
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                switch (event.type) {
                    case SDL_QUIT:
                        in_settings = false;
                    exit(0);
                    break;
                    case SDL_KEYUP:
                        if (event.key.keysym.scancode == SDL_SCANCODE_RETURN) {
                            in_settings = false;
                        }
                    break;
                }
            }

            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, background_texture, nullptr, nullptr);

            // bakc button
            SDL_Rect dest_rect = {170, 380, 350, 120};
            SDL_RenderCopy(renderer, selected == 0 ? back_button_active_texture : back_button_texture, nullptr, &dest_rect);

            SDL_RenderPresent(renderer);
        }
    }

private:
    SDL_Renderer *renderer;
    SDL_Texture *background_texture;
    SDL_Texture *back_button_texture;
    SDL_Texture *back_button_active_texture;
};

#endif // SETTINGS_H
