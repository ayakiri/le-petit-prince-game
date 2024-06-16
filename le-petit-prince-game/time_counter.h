#ifndef C___TIME_COUNTER_H
#define C___TIME_COUNTER_H

#include <chrono>
#include "sprite.h"


using Clock = std::chrono::high_resolution_clock;
using TimeStamp = std::chrono::time_point<Clock>;

struct Time_Counter {
    Time_Counter(SDL_Renderer *renderer) : renderer(renderer), numbers(renderer, "nums.bmp", 24) {}

    void draw() {
        double dt = std::chrono::duration<double>(Clock::now() - timestamp).count();
        if (dt >= 1.0) { // every second update the time counter
            seconds++;
            timestamp = Clock::now();
        }
        SDL_Rect dst = {4, 16, numbers.width, numbers.height}; // first character will be drawn here
        for (const char c : std::to_string(seconds)) { // extract individual digits of seconds_elapsed
            SDL_Rect src = numbers.rect(c - '0'); // crude conversion of numeric characters to int: '7'-'0'=7
            SDL_RenderCopy(renderer, numbers.texture, &src, &dst); // draw current digit
            dst.x += numbers.width + 4; // draw characters left-to-right, +4 for letter spacing
        }
    }

    int seconds = 0;
    TimeStamp timestamp = Clock::now();
    SDL_Renderer *renderer;
    const Sprite numbers;
};

#endif //C___TIME_COUNTER_H