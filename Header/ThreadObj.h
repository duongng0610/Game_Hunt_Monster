#ifndef THREAD_OBJ_H_
#define THREAD_OBJ_H

#include "BaseObj.h"
#include "CommonFunc.h"

#define THREAD_FRAME_NUM 8
#define THREAD_GRAVITY_SPEED 0.8
#define THREAD_MAX_SPEED_FALL 10

class ThreadObj : public BaseObj
{
public:
    ThreadObj();
    ~ThreadObj();

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
    void Shown(SDL_Renderer* des);

    int Get_Width_frame() const {return width_frame;}
    int Get_Height_frame() const {return height_frame;}

    void DoPlayer(Map& map_data);
    void inMap(Map& map_data);

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
};
#endif // THREAD_OBJ_H_
