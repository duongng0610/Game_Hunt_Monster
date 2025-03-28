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

void BulletObj::HandleMove(const int& x_border, const int& y_border)
{
    if(bullet_dir == DIR_RIGHT) {
        rect.x += x_val;
        if(rect.x > x_border) {
           is_move = false;
        }
    }else if(bullet_dir == DIR_LEFT) {
        rect.x -= x_val;
        if(rect.x < 0) {
            is_move = false;
        }
    }
}
