#ifndef EXPLOSION_OBJ_H_
#define EXPLOSION_OBJ_H_

#include "CommonFunc.h"
#include "BaseObj.h"
#include "ExplosionObj.h"

#define NUM_FRAME_EXP 8

class ExplosionObj : public BaseObj
{
public:
    ExplosionObj();
    ~ExplosionObj();

    void set_clips();
    void set_frame(const int& _frame) {frame = _frame;}
    virtual bool LoadImg(string path, SDL_Renderer* screen);
    void Shown(SDL_Renderer* screen);
    int get_width_frame() const {return width_frame;}
    int get_height_frame() const {return height_frame;}

private:
    int width_frame;
    int height_frame;

    int frame;
    SDL_Rect frame_clip[8];
};
#endif // EXPLOSION_OBJ_H_
