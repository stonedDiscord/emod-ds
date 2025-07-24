#ifndef GAME_MAP_HPP
#define GAME_MAP_HPP

#include <nds.h>
#include <NEMain.h>

class GameMap
{
public:
    virtual void initAndLoadMapResources();
    virtual void drawMap();

    const char* mapName = nullptr;
};

#endif