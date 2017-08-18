#include "level.hpp"

#include <iostream>
#include <cstdlib>
#include <string>

const auto MAP_PATH = "res/map/example.tmx";
const auto SCREEN_WIDTH = 320;
const auto SCREEN_HEIGHT = SCREEN_WIDTH;

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
bool running = true;
level tiled_map_level("Interesting Level Name");

bool init();
void shutdown(const std::string& msg = "", const int code = EXIT_SUCCESS);
void interact();
void update(float elapsed);
void render();

int main() {
    if (!init()) {
        shutdown(std::string("Failed to initialize") + SDL_GetError(), EXIT_FAILURE);
    }

    tiled_map_level.load(MAP_PATH, renderer);

    auto last_time = SDL_GetTicks();
    while (running) {
        auto time_now = SDL_GetTicks();
        auto elapsed = time_now - last_time;
        last_time = time_now;

        interact();
        update(elapsed);
        render();
    }

    shutdown("Reached Normal Shutdown");

    return EXIT_SUCCESS;
}

bool init() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        return false;
    }

    window = SDL_CreateWindow(
        "Tiled Map Example",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN
    );
    if (!window) {
        return false;
    }

    renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if (!renderer) {
        return false;
    }

    return true;
}

void shutdown(const std::string& msg, const int code) {
    if (renderer) {
        SDL_DestroyRenderer(renderer);
    }
    if (window) {
        SDL_DestroyWindow(window);
    }

    SDL_Quit();


    std::cerr << msg << "\n";
    exit(code);
}

void interact() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        }
    }
}

void update(float elapsed) {

}

void render() {
    SDL_RenderClear(renderer);
    tiled_map_level.draw(renderer);
    SDL_RenderPresent(renderer);
}

