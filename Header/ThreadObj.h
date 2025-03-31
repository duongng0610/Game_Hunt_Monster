#ifndef THREAD_OBJ_H_
#define THREAD_OBJ_H

#include "BaseObj.h"
#include "CommonFunc.h"
#include "BulletObj.h"

#define THREAD_FRAME_NUM 8
#define THREAD_GRAVITY_SPEED 0.8
#define THREAD_MAX_SPEED_FALL 10
#define THREAD_SPEED 5
#define SPACE_MOVE 200

class ThreadObj : public BaseObj
{
public:
    ThreadObj();
    ~ThreadObj();

    enum TypeMove
    {
        STATIC_THREAD = 0,
        MOVE_IN_SPACE = 1,
    };

    void Set_X_val(const float& _x_val) {
        x_val = _x_val;
    }

    void Set_Y_val(const float& _y_val) {
        y_val = _y_val;
    }

    void Set_X_pos(const float& _x_pos) {
        x_pos = _x_pos;
    }

    void Set_Y_pos(const float& _y_pos) {
        y_pos = _y_pos;
    }

    float Get_X_pos() const {return x_pos;}
    float Get_Y_pos() const {return y_pos;}

    void SetMapXY(const int& _map_x, const int& _map_y) {
        map_x = _map_x;
        map_y = _map_y;
    }

    void set_clips();
    bool LoadImg(string path, SDL_Renderer* screen);
    SDL_Rect Get_Rect_Frame();
    void Shown(SDL_Renderer* des);

    int Get_Width_frame() const {return width_frame;}
    int Get_Height_frame() const {return height_frame;}

    void DoPlayer(Map& map_data);
    void InitThread();
    void inMap(Map& map_data);

    void set_type_move(const int& _type_move) {
        type_move = _type_move;
    }

    void set_animtion_pos(const int& pos_min, const int& pos_max) {
        animation_max = pos_max;
        animation_min = pos_min;
    }

    void set_input_left(const int& ipLeft) {
        input_type.left = ipLeft;
    }

    void ImpMoveType(SDL_Renderer* screen);

    vector<BulletObj*> get_bullet_list() const {return bullet_list;}
    void set_bullet_list(const vector<BulletObj*>& _bullet_list) {bullet_list = _bullet_list;}
    void InitBullet(BulletObj* p_bullet, SDL_Renderer* screen);
    void MakeBullet(SDL_Renderer* screen, const int& x_limit, const int& y_limit, Map& map_data);
    void RemoveBullet(const int& index);

private:
    SDL_Rect frame_clip[THREAD_FRAME_NUM];

    float x_pos;
    float y_pos;

    float x_val;
    float y_val;

    bool on_ground;
    int come_back;

    int map_x;
    int map_y;

    int width_frame;
    int height_frame;
    int frame;

    // khoang di chuyen cua thread
    int animation_min;
    int animation_max;

    int type_move;
    Input input_type;

    vector<BulletObj*> bullet_list;
};
#endif // THREAD_OBJ_H_
