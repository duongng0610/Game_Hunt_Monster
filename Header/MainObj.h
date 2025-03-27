#ifndef MAIN_OBJ_H_
#define MAIN_OBJ_H_

#include "CommonFunc.h"
#include "BaseObj.h"

#define GRAVITY_SPEED 0.8
#define MAX_FALL_SPEED 10
#define PLAYER_SPEED 8
#define PLAYER_JUMP 12


class MainObj : public BaseObj
{
public:
    MainObj();
    ~MainObj();

    enum WalkType
    {
        WALK_RIGHT = 0,
        WALK_LEFT = 1
    };

    bool LoadImg(string path, SDL_Renderer* screen);
    void Shown(SDL_Renderer* des);
    void HandleInputAction(SDL_Event events, SDL_Renderer* screen);
    void set_clip();

    void DoPlayer(Map& map_data);
    void inMap(Map& map_data);

    void setMapXY(const int _map_x, const int _map_y) {map_x = _map_x; map_y = _map_y;};
    void CenterEntityOnMap(Map& map_data);
    void UpdateImgPlayer(SDL_Renderer* des);

private:
    float x_val;
    float y_val;

    float x_pos;
    float y_pos;

    int width_frame;
    int height_frame;

    int map_x;
    int map_y;

    int frame;
    SDL_Rect frame_clip[8];

    Input input_type; // move status
    int status; // left or right
    bool on_ground;
    int come_back_time;

};


#endif // MAIN_OBJ_H_
