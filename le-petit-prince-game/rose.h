#include <utility>

#ifndef LE_PETIT_PRINCE_GAME_ROSE_H
#define LE_PETIT_PRINCE_GAME_ROSE_H

class Rose {
public:
    Rose(SDL_Renderer *renderer, std::string file_path, int i, int j)
            : renderer(renderer),texture_path(std::move(file_path)) {
        int window_w, window_h;
        SDL_GetRendererOutputSize(renderer, &window_w, &window_h);
        tile_w = window_w/16;
        tile_h = window_h/12;
        x = i*tile_w;
        y = j*tile_h;
    }

    void draw() {
        SDL_Surface *surface = SDL_LoadBMP(texture_path.c_str());
        SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format,234, 63, 247));
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect dest = { x, y, width, height };
        SDL_RenderCopy(renderer, texture, NULL, &dest);
        SDL_FreeSurface(surface);
    }

    int x, y;
    int width = 64;  // Default width of the Rose image
    int height = 64; // Default height of the Rose image
    SDL_Renderer *renderer;
    SDL_Texture *texture = nullptr;
    std::string texture_path;
    int tile_w=0, tile_h=0;
};

#endif //LE_PETIT_PRINCE_GAME_ROSE_H
