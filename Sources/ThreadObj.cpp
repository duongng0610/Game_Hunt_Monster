#include "ThreadObj.h"

ThreadObj::ThreadObj()
{
    width_frame = 0;
    height_frame = 0;
    x_val = 0;
    y_val = 0;
    x_pos = 0;
    y_pos = 0;
    on_ground = false;
    come_back = 0;
    frame = 0;
}

ThreadObj::~ThreadObj()
{

}

bool ThreadObj::LoadImg(string path, SDL_Renderer* screen)
{
    bool ret = BaseObj::LoadImg(path, screen);
    if(ret) {
        width_frame = rect.w/THREAD_FRAME_NUM;
        height_frame = rect.h;
    }
    return ret;
}

void ThreadObj::set_clips()
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

void ThreadObj::Shown(SDL_Renderer* des)
{
    if(come_back == 0) {
        rect.x = x_pos - map_x;
        rect.y = y_pos - map_y;

        frame++;
        if(frame >= 8) {
            frame = 0;
        }

        SDL_Rect* current_clip = &frame_clip[frame];
        SDL_Rect renderQuad = {rect.x, rect.y, width_frame, height_frame};
        SDL_RenderCopy(des, p_obj, current_clip, &renderQuad);
    }
}

void ThreadObj::DoPlayer(Map& map_data)
{
    if(come_back == 0) {
        x_val = 0;
        y_val += THREAD_GRAVITY_SPEED;

        if(y_val >= THREAD_MAX_SPEED_FALL) {
            y_val = THREAD_MAX_SPEED_FALL;
        }

        inMap(map_data);
    }
    else if(come_back > 0) {
        come_back--;
        if(come_back == 0) {
            x_val = 0;
            y_val = 0;

            if(x_pos > 256) {
                x_pos -= 256;
            }else x_pos = 0;

            y_pos = 0;
            come_back = 0;
        }
    }
}

void ThreadObj::inMap(Map& map_data)
{
    int x1 = 0;
    int x2 = 0;

    int y1 = 0;
    int y2 = 0;

    //check horizontal
    int height_min = min(height_frame, TILE_SIZE);

    x1 = (x_pos + x_val)/TILE_SIZE; // tim vi tri o hien tai
    x2 = (x_pos + x_val + width_frame-1)/TILE_SIZE;

    y1 = (y_pos)/TILE_SIZE;
    y2 = (y_pos + height_min-1)/TILE_SIZE;

    if(x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y) {
        // di chuyen sang phai
        if(x_val > 0) {
            if(map_data.tile[y1][x2] != 0 || map_data.tile[y2][x2] != 0) {
               x_pos = x2*TILE_SIZE;
               x_pos -= width_frame + 1;
               x_val = 0;
           }
        }
        // di chuyen sang trai
        else if(x_val < 0) {
            if(map_data.tile[y1][x1] != 0 || map_data.tile[y2][x1] != 0) {
                x_pos = (x1+1)*TILE_SIZE;
                x_val = 0;
            }
        }
    }

    // check vertical
    int width_min = min(width_frame, TILE_SIZE);

    x1 = x_pos/TILE_SIZE;
    x2 = (x_pos + width_min)/TILE_SIZE;

    y1 = (y_pos + y_val)/TILE_SIZE;
    y2 = (y_pos + y_val + height_frame - 1)/TILE_SIZE;

    if(x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y) {
        // roi xuong
        if(y_val > 0) {
            if(map_data.tile[y2][x1] != 0 || map_data.tile[y2][x2] != 0) {
                y_pos = y2*TILE_SIZE;
                y_pos -= (height_frame+1);
                y_val = 0;
                on_ground = true;
            }
        }
        // nhay len
        else if(y_val < 0) {
            if(map_data.tile[y1][x1] != 0 || map_data.tile[y1][x2] != 0) {
                y_pos = (y1+1)*TILE_SIZE;
                y_val = 0;
            }
        }

    }

    x_pos += x_val;
    y_pos += y_val;

    // gioi han ban do
    if(x_pos < 0) x_pos = 0;
    else if(x_pos + width_frame > map_data.max_x) x_pos = map_data.max_x - width_frame - 1;

    if(y_pos > map_data.max_y) {
        come_back = 60;
    }
}
