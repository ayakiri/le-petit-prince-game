#ifndef MENU_H
#define MENU_H

#include <SDL.h>

class Menu {
public:
    Menu(SDL_Renderer *renderer, SDL_Texture *background_texture,
         SDL_Texture *easy_texture, SDL_Texture *easy_active_texture,
         SDL_Texture *medium_texture, SDL_Texture *medium_active_texture,
         SDL_Texture *hard_texture, SDL_Texture *hard_active_texture)
        : renderer(renderer), background_texture(background_texture),
          easy_texture(easy_texture), easy_active_texture(easy_active_texture),
          medium_texture(medium_texture), medium_active_texture(medium_active_texture),
          hard_texture(hard_texture), hard_active_texture(hard_active_texture),
          chosen_level(0) {}

    ~Menu() {
        SDL_DestroyTexture(background_texture);
        SDL_DestroyTexture(easy_texture);
        SDL_DestroyTexture(easy_active_texture);
        SDL_DestroyTexture(medium_texture);
        SDL_DestroyTexture(medium_active_texture);
        SDL_DestroyTexture(hard_texture);
        SDL_DestroyTexture(hard_active_texture);
    }

    void show() {
        SDL_Texture* textures[] = {easy_texture, medium_texture, hard_texture};
        SDL_Texture* active_textures[] = {easy_active_texture, medium_active_texture, hard_active_texture};
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
                        if (event.key.keysym.scancode == SDL_SCANCODE_UP) {
                            selected = (selected + 2) % 3;
                        } else if (event.key.keysym.scancode == SDL_SCANCODE_DOWN) {
                            selected = (selected + 1) % 3;
                        } else if (event.key.keysym.scancode == SDL_SCANCODE_RETURN) {
                            selecting = false;
                            chosen_level = selected;
                        }
                        break;
                }
            }

            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, background_texture, nullptr, nullptr);

            // Render buttons
            for (int i = 0; i < 3; ++i) {
                SDL_Rect dest_rect = {210, 300 + i * 80, 200, 50};
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
    int chosen_level;
};

#endif // MENU_H
