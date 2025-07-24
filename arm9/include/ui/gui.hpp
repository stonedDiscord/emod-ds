#ifndef GAME_UI_HPP
#define GAME_UI_HPP

#include <nds.h>
#include <nf_lib.h>

#include "map.hpp"

#define MAP_W     64    // 512 pixels / 8 = 64 tiles
#define MAP_H     64
#define BASE_W     4    // 32 pixels / 8 = 4 tiles
#define BASE_H     4

class GameUI
{
    int scrollX, scrollY, frameCount = 0;
public:
    void initAndLoadUIResources()
    {
        // gui default background
        NF_InitTiledBgBuffers();
        NF_InitTiledBgSys(1);

        NF_LoadTilesForBg("bg/emodbg", "emodbg", 512, 512, 0, 15);
        NF_CreateTiledBg(1, 3, "emodbg");
        
        // generate a map from the tiles
        for(int map_y = 0; map_y < MAP_H; map_y++)
        {
            for(int map_x = 0; map_x < MAP_W; map_x++)
            {
                // basically line up all of the 16 tiles in order
                // BITMASK MODULO MY BELOVED. write an article on this mf
                int tx = map_x & (BASE_W - 1);   // same as map_x % 4
                int ty = map_y & (BASE_H - 1);   // same as map_y % 4

                int tileIndex = ty * BASE_W + tx;

                // put that tile into the VRAM map
                NF_SetTileOfMap(1, 3, map_x, map_y, tileIndex);
            }
        }

        // push the filled map up to VRAM
        NF_UpdateVramMap(1, 3);

        // gui text
        NF_InitTextSys(1);
        NF_LoadTextFont("fnt/default", "normal", 256, 256, 0);
        NF_CreateTextLayer(1, 0, 0, "normal");
    };

    void scrollGUIBackground()
    {
        frameCount++;

        scrollX += frameCount & 1;
        scrollY += frameCount & 1;

        scrollX &= 255;
        scrollY &= 255;

        NF_ScrollBg(1, 3, scrollX, scrollY);
    };

    void writeMapName(GameMap* map)
    {
        if (map->mapName != nullptr)
            NF_WriteText(1, 0, 1, 22, map->mapName);
    };
};

#endif