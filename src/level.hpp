#pragma once

#include "SDL2/SDL.h"
#include <string>
#include <vector>
#include <map>

typedef int gid;

// Stores information about an individual tile to be
// displayed.
struct tile {
    SDL_Texture* sheet;
    // x coordinate in the world
    int x;
    // y coordinate in the world
    int y;
    // the x coordinate on the sprite sheet
    int tx;
    // the y coordinate on the sprite sheet
    int ty;
    int width;
    int height;

    tile(SDL_Texture* tset, int x = 0, int y = 0, 
        int tx = 0, int ty = 0, int w = 0, int h = 0);
    void draw(SDL_Renderer* ren);
};

class level {
public:
    level(const std::string& name);
    void load(const std::string& path, SDL_Renderer* ren);
    void draw(SDL_Renderer* ren);
private:
    std::string name;
    // Think of the dimensions as a 2D array (after all, that's what our
    // Tiled map is)
    // The rows variable is the number of tiles from top to bottom (Y axis).
    int rows;
    // The cols variable is the number of tiles from left to right (X axis).
    int cols;
    int tile_width;
    int tile_height;
    // All of the tiles we will draw to the screen.
    std::vector<tile> tiles;
    // All of the tilesets used by our Tiled map.
    std::map<gid, SDL_Texture*> tilesets;
};

