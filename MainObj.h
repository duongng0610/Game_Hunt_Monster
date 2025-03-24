#ifndef MAIN_OBJ_H_
#define MAIN_OBJ_H_

#include "CommonFunc.h"
#include "BaseObj.h"

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

private:
    float x_val;
    float y_val;

    float x_pos;
    float y_pos;

    int width_frame;
    int height_frame;

    int map_x;
    int map_y;

    SDL_Rect frame_clip[8];
    Input input_type; // move status
    int frame;
    int status; // left or right
    bool on_ground;
};


#endif // MAIN_OBJ_H_
