#ifndef GAME_MAP_H
#define GAME_NAP_H

#include "CommonFunc.h"
#include "BaseObj.h"

const int MAX_TILES = 20;

class TileMat : public BaseObj
{
public:
    TileMat() {}
    ~TileMat() {}
};

class GameMap
{
public:
    GameMap() {}
    ~GameMap() {}

    void LoadMap(char* name);
    void LoadTiles(SDL_Renderer* screen);
    void DrawMap(SDL_Renderer* screen);
    Map getMap() const {return game_map;};
    void setMap(Map& map_data) {game_map = map_data;};

private:
    Map game_map;
    TileMat tile_mat[MAX_TILES];
};

#endif // GAME_MAP_H
