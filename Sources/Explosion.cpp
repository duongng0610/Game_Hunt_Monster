#include "ExplosionObj.h"

ExplosionObj::ExplosionObj()
{
    frame = 0;
    width_frame = 0;
    height_frame = 0;
}

ExplosionObj::~ExplosionObj()
{

}

bool ExplosionObj::LoadImg(string path, SDL_Renderer* screen)
{
    bool ret = BaseObj::LoadImg(path, screen);
    if(ret) {
        width_frame = rect.w/NUM_FRAME_EXP;
        height_frame = rect.h;
    }
    return ret;
}

void ExplosionObj::set_clips()
{
    if(width_frame > 0 && height_frame > 0) {
        frame_clip[0].x = 0;
        frame_clip[0].y = 0;
        frame_clip[0].w = width_frame;
        frame_clip[0].h = height_frame;

        for(int i = 1; i < 8; i++) {
            frame_clip[i].x = i*width_frame;
            frame_clip[i].y = 0;
            frame_clip[i].w = width_frame;
            frame_clip[i].h = height_frame;
        }
    }
}

void ExplosionObj::Shown(SDL_Renderer* screen)
{
    SDL_Rect* current_clip = &frame_clip[frame];
    SDL_Rect renderQuad = {rect.x, rect.y, width_frame, height_frame};
    SDL_RenderCopy(screen, p_obj, current_clip, &renderQuad);
}
