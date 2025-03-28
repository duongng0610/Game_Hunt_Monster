#include "map.h"

void GameMap::LoadMap(string name)
{
    ifstream file(name);

    if(!file.is_open()) {
        cout << "Mission failed" << endl;
        return;
    }

    game_map.max_x = 0;
    game_map.max_y = 0;

    for(int i = 0; i < MAX_MAP_Y; i++) {
        for(int j = 0; j < MAX_MAP_X; j++) {
            int val;
            // kiem tra loi doc file
            if (!(file >> val)) {
                cout << "Error reading map data at row: " << i << " col: " << j << endl;
                file.close();
                return;
            }
            // kiem tra gia tri val bat thuong
            if (val < 0 || val >= MAX_TILES) {
                cout << "Invalid value " << val << " at row: " << i << " col: " << j << endl;
            }

            game_map.tile[i][j] = val;

            if(val > 0) {
                if(j > game_map.max_x) {
                    game_map.max_x = j;
                }

                if(i > game_map.max_y) {
                    game_map.max_y = i;
                }
            }
        }
    }
    // kiem tra file con du lieu rac
    if (!file.eof()) {
        cout << "File may not contain enough data";
    }


    game_map.max_x = (game_map.max_x + 1) * TILE_SIZE;
    game_map.max_y = (game_map.max_y + 1) * TILE_SIZE;

    game_map.start_x = 0;
    game_map.start_y = 0;

    game_map.file_name = name;

    file.close();
}

// tai truoc hinh anh vao tile mat
void GameMap::LoadTiles(SDL_Renderer* screen)
{
    string name;
    ifstream file;

    for(int i = 0; i < MAX_TILES; i++) {
        string num_img = to_string(i);
        name = "map//" + num_img + ".png";
        file.open(name);

        if(!file.is_open()) {
            cout << "Can not open: " << name << endl;
            continue;
        }

        file.close();
        tile_mat[i].LoadImg(name, screen);
    }
}

// dien hinh anh vao cac o
void GameMap::DrawMap(SDL_Renderer* screen)
{
    int x1 = 0;
    int x2 = 0;

    int y1 = 0;
    int y2 = 0;

    int map_x = 0;
    int map_y = 0;

    map_x = game_map.start_x / TILE_SIZE;

    x1 = (game_map.start_x % TILE_SIZE)* (-1);
    x2 = (x1 == 0) ? x1 + SCREEN_WIDTH : x1 + SCREEN_WIDTH + TILE_SIZE;

    map_y = game_map.start_y / TILE_SIZE;

    y1 = (game_map.start_y % TILE_SIZE)* (-1);
    y2 = (y1 == 0) ? y1 + SCREEN_HEIGHT : y1 + SCREEN_HEIGHT + TILE_SIZE;

    for(int i = y1; i < y2; i += TILE_SIZE) {
        map_x = game_map.start_x / TILE_SIZE; // reset lai map_x
        for(int j = x1; j < x2; j += TILE_SIZE) {
            int val = game_map.tile[map_y][map_x];
            if(val > 0 && val < MAX_TILES) {
                tile_mat[val].SetRect(j, i);
                tile_mat[val].Render(screen);
            }
            map_x++;
        }
        map_y++;
    }
}



