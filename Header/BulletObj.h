#ifndef BULLET_OBJ_H_
#define BULLET_OBJ_H_

#include "BaseObj.h"
#include "CommonFunc.h"


class BulletObj : public BaseObj
{
public:
    BulletObj();
    virtual ~BulletObj();

    enum BulletDir
    {
        DIR_RIGHT = 20,
        DIR_LEFT = 21
    };

    void Set_X_val(const int& x) {
        x_val = x;
    }

    void Set_Y_val(const int& y) {
        y_val = y;
    }
    int Get_X_val() const {return x_val;}
    int Get_Y_val() const {return y_val;}

    void Set_Is_Move(const bool& _is_move) {
        is_move = _is_move;
    }
    bool Get_Is_Move() const {return is_move;}

    void Set_Bullet_Dir(const int _bullet_dir) {
        bullet_dir = _bullet_dir;
    }
    int Get_Bullet_Dir() const {return bullet_dir;}

    void HandleMove(const int& x_border, const int& y_border, Map& map_data);
private:
    int x_val;
    int y_val;
    bool is_move;
    int bullet_dir;
};
#endif // _BULLET_OBJ_H_
