#include "BulletObj.h"

BulletObj::BulletObj()
{
     x_val = 0;
     y_val = 0;
     is_move = false;

}

BulletObj::~BulletObj()
{

}

void BulletObj::HandleMove(const int& x_border, const int& y_border, Map& map_data)
{

    int x1 = (rect.x + map_data.start_x) / TILE_SIZE;
    int y1 = (rect.y + map_data.start_y) / TILE_SIZE;
    int x2 = (rect.x + rect.w - 1 + map_data.start_x) / TILE_SIZE;
    int y2 = (rect.y + rect.h - 1 + map_data.start_y) / TILE_SIZE;

    if(bullet_dir == DIR_RIGHT) {
        rect.x += x_val;
        if(rect.x > x_border) {
           is_move = false;
        }
        // kiem tra va cham voi tuong
        if(map_data.tile[y1][x2] != 0 || map_data.tile[y2][x2] != 0) {
            is_move = false;
        }
    }else if(bullet_dir == DIR_LEFT) {
        rect.x -= x_val;
        if(rect.x < 0) {
            is_move = false;
        }
        // kiem tra va cham voi tuong
        if(map_data.tile[y1][x1] != 0 || map_data.tile[y2][x1] != 0) {
            is_move = false;
        }
    }
}
