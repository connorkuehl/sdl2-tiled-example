#include "assets.hpp"
#include <iostream>

assets::assets() {}

assets::~assets() {
    for (auto tex : textures) {
        if (tex.second) {
            SDL_DestroyTexture(tex.second);
            tex.second = nullptr;
        }
    }
}

assets& assets::instance() {
    static std::unique_ptr<assets> inst(new assets());
    return *inst;
}

SDL_Texture* assets::load_texture(const std::string& path, SDL_Renderer* ren) {
    auto already_loaded = textures.find(path);
    if (already_loaded != textures.end()) {
        return already_loaded->second;
    }

    auto surf = IMG_Load(path.c_str());
    if (!surf) {
        std::cerr << "ERROR: failed to load " << path << ": " 
                << IMG_GetError() << "\n";
        return nullptr;
    }

    auto tex = SDL_CreateTextureFromSurface(ren, surf);
    if (!tex) {
        std::cerr << "ERROR: failed to create texture: " 
                 << SDL_GetError() << "\n";
    }

    textures.insert(std::pair<std::string, SDL_Texture*>(path, tex));

    return tex;
}

