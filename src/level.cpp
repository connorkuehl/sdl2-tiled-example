#include "level.hpp"
#include "assets.hpp"
#include "tmxlite/Map.hpp"
#include "tmxlite/Layer.hpp"
#include "tmxlite/TileLayer.hpp"
#include <iostream>
#include <algorithm>

tile::tile(SDL_Texture* tset, int x, int y, int tx, int ty, int w, int h) 
: sheet(tset), x(x), y(y), tx(tx), ty(ty), width(w), height(h) {

}

void tile::draw(SDL_Renderer* ren) {
    if (!ren || !sheet)
        return;

    SDL_Rect src;
    src.x = tx;
    src.y = ty;
    src.w = width;
    src.h = height;

    SDL_Rect dest;
    dest.x = x;
    dest.y = y;
    dest.w = src.w;
    dest.h = src.h;

    SDL_RenderCopy(ren, sheet, &src, &dest);
}

level::level(const std::string& name) 
:  name(name), rows(0), cols(0) {

}

void level::load(const std::string& path, SDL_Renderer* ren) {
    // Load and parse the Tiled map with tmxlite
    tmx::Map tiled_map;
    tiled_map.load(path);

    // We need to know the size of the map (in tiles)
    auto map_dimensions = tiled_map.getTileCount();
    rows = map_dimensions.y;
    cols = map_dimensions.x;
    // We also need to know the dimensions of the tiles.
    auto tilesize = tiled_map.getTileSize();
    tile_width = tilesize.x;
    tile_height = tilesize.y;

    // Load all of the tilesets and store them in a data structure.
    // I chose to store them in a map.
    auto& map_tilesets = tiled_map.getTilesets();
    for (auto& tset : map_tilesets) {
        auto tex = assets::instance()
            .load_texture(tset.getImagePath(), ren);
        tilesets.insert(std::pair<gid, SDL_Texture*>(tset.getFirstGID(), tex));
    }

    // This is the hard part; iterate through each layer in the map,
    // poke each tile for the information you need, and store it in
    // our tiles data structure. 
    //
    // We start at the bottom most layer, and work our way up with this
    // outer for-loop.
    auto& map_layers = tiled_map.getLayers();
    for (auto& layer : map_layers) {
        // We're only looking to render the tiles on the map, so if
        // this layer isn't a tile layer, we'll move on.
        if (layer->getType() != tmx::Layer::Type::Tile) {
            continue;
        }

        auto* tile_layer = dynamic_cast<const tmx::TileLayer*>(layer.get());

        // Grab all of this layer's tiles. 
        auto& layer_tiles = tile_layer->getTiles();

        // Remember when we needed the size of the Tiled map? This
        // is where it comes into play. Because we're making a simple
        // 2D game, each tile belongs in an x,y coordinate, so using
        // a loop can help us visit each place if there's a tile there.
        for (auto y = 0; y < rows; ++y) {
            for (auto x = 0; x < cols; ++x) {
                // the `layer_tiles` vector that our `getTiles()` call
                // returned on line 78 isn't a 2D array, but we can
                // convert our x, y coordinates into an index that we
                // can access it at.
                auto tile_index = x + (y * cols);
                // Grab the GID of the tile we're at.
                auto cur_gid = layer_tiles[tile_index].ID;
                // If the GID is 0, that means it's an empty tile,
                // we don't want to waste time on nothing, nor do we
                // want to store an empty tile.
                if (cur_gid == 0) {
                    continue;
                }

                // Find the tileset that this tile belongs to. The tileset
                // whose first GID is less-than-or-equal to this GID, is
                // the tileset our GID belongs to. A tileset with a first
                // GID that is greater than this tile means that this tile
                // does not belong to that tileset.
                auto tset_gid = -1;
                for (auto& ts : tilesets) {
                    if (ts.first <= cur_gid) {
                        tset_gid = ts.first;
                        break;
                    }
                }
                // If we didn't find a valid tileset, skip the tile. We can't
                // render it if we don't have a tileset to pull from.
                if (tset_gid == -1) {
                    continue;
                }

                // Normalize the GID.
                cur_gid -= tset_gid;
                // Find the dimensions of the tile sheet. This is important,
                // because our tile is only a small sprite on that sheet, not
                // the whole sheet.
                auto ts_width = 0;
                auto ts_height = 0;
                SDL_QueryTexture(tilesets[tset_gid],
                    NULL, NULL, &ts_width, &ts_height);

                // Calculate the area on the tilesheet to draw from.
                auto region_x = (cur_gid % (ts_width / tile_width)) * tile_width;
                auto region_y = (cur_gid / (ts_width / tile_width)) * tile_height;

                // Calculate the world position of our tile. This is easy,
                // because we're using nested for-loop to visit each x,y
                // coordinate.
                auto x_pos = x * tile_width;
                auto y_pos = y * tile_height;

                // Phew, all done. 
                tile t(tilesets[tset_gid], x_pos, y_pos, 
                    region_x, region_y, tile_width, tile_height);
                tiles.push_back(t);
            }
        }
    }
}

void level::draw(SDL_Renderer* ren) {
    for (auto& tile : tiles) {
        tile.draw(ren);
    }
}

