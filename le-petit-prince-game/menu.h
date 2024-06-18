#ifndef MENU_H
#define MENU_H

#include <SDL.h>
#include "settings.h"

class Menu {
public:
    Menu(SDL_Renderer *renderer, SDL_Texture *background_texture,
         SDL_Texture *easy_texture, SDL_Texture *easy_active_texture,
         SDL_Texture *medium_texture, SDL_Texture *medium_active_texture,
         SDL_Texture *hard_texture, SDL_Texture *hard_active_texture,
         SDL_Texture *settings_texture, SDL_Texture *settings_active_texture,
         SDL_Texture *settings_background_texture, SDL_Texture *back_button_texture, SDL_Texture *back_button_active_texture)
        : renderer(renderer), background_texture(background_texture),
          easy_texture(easy_texture), easy_active_texture(easy_active_texture),
          medium_texture(medium_texture), medium_active_texture(medium_active_texture),
          hard_texture(hard_texture), hard_active_texture(hard_active_texture),
          settings_texture(settings_texture), settings_active_texture(settings_active_texture),
          chosen_level(0),
          settings(renderer, settings_background_texture, back_button_texture, back_button_active_texture) {}

    ~Menu() {
        SDL_DestroyTexture(background_texture);
        SDL_DestroyTexture(easy_texture);
        SDL_DestroyTexture(easy_active_texture);
        SDL_DestroyTexture(medium_texture);
        SDL_DestroyTexture(medium_active_texture);
        SDL_DestroyTexture(hard_texture);
        SDL_DestroyTexture(hard_active_texture);
        SDL_DestroyTexture(settings_texture);
        SDL_DestroyTexture(settings_active_texture);
    }

    void show() {
        SDL_Texture* textures[] = {easy_texture, medium_texture, hard_texture, settings_texture};
        SDL_Texture* active_textures[] = {easy_active_texture, medium_active_texture, hard_active_texture, settings_active_texture};
        int selected = 0;
        bool selecting = true;

        while (selecting) {
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                switch (event.type) {
                    case SDL_QUIT:
                        selecting = false;
                        exit(0);
                        break;
                    case SDL_KEYUP:
                        if (event.key.keysym.scancode == SDL_SCANCODE_Q) exit(0);
                        if (event.key.keysym.scancode == SDL_SCANCODE_UP) {
                            selected = (selected + 3) % 4;
                        } else if (event.key.keysym.scancode == SDL_SCANCODE_DOWN) {
                            selected = (selected + 1) % 4;
                        } else if (event.key.keysym.scancode == SDL_SCANCODE_RETURN) {
                            if (selected == 3) {
                                settings.show();
                            } else {
                                selecting = false;
                                chosen_level = selected;
                            }
                        }
                        break;
                }
            }

            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, background_texture, nullptr, nullptr);

            // Render buttons
            for (int i = 0; i < 4; ++i) {
                SDL_Rect dest_rect = {130, 240 + i * 120, 270, 90};
                if (i == selected) {
                    SDL_RenderCopy(renderer, active_textures[i], nullptr, &dest_rect);
                } else {
                    SDL_RenderCopy(renderer, textures[i], nullptr, &dest_rect);
                }
            }

            SDL_RenderPresent(renderer);
        }
    }

    int get_chosen_level() const {
        return chosen_level;
    }

private:
    SDL_Renderer *renderer;
    SDL_Texture *background_texture;
    SDL_Texture *easy_texture;
    SDL_Texture *easy_active_texture;
    SDL_Texture *medium_texture;
    SDL_Texture *medium_active_texture;
    SDL_Texture *hard_texture;
    SDL_Texture *hard_active_texture;
    SDL_Texture *settings_texture;
    SDL_Texture *settings_active_texture;
    int chosen_level;
    Settings settings;
};

#endif // MENU_H
