#ifndef C___SPRITE_H
#define C___SPRITE_H

struct Sprite {
    Sprite(SDL_Renderer *renderer, const std::string filename, const int width) : width(width) {
        std::string path = "assets/" + filename;
        const char* filePath = path.c_str();
        SDL_Surface *surface = SDL_LoadBMP(filePath);
        Uint32 colorkey = SDL_MapRGB(surface->format, 234, 63, 247);
        SDL_SetColorKey(surface, SDL_TRUE, colorkey);
        if (!surface) {
            std::cerr << "Error in SDL_LoadBMP: " << SDL_GetError() << std::endl;
            return;
        }
        if (!(surface->w%width) && surface->w/width) { // image width must be a multiple of sprite width
            height  = surface->h;
            nframes = surface->w/width;
            texture = SDL_CreateTextureFromSurface(renderer, surface);
        } else
            std::cerr << "Incorrect sprite size" << std::endl;
        SDL_FreeSurface(surface);
    }

    SDL_Rect rect(const int idx) const { // choose the sprite number idx from the texture
        return { idx*width, 0, width, height };
    }

    ~Sprite() {
        if (texture) SDL_DestroyTexture(texture);
    }

    SDL_Texture *texture = nullptr; //image
    int width   = 0; // single sprite width (texture width = width * nframes)
    int height  = 0; // sprite height
    int nframes = 0; // number of frames in the animation sequence
};

#endif //C___SPRITE_H
