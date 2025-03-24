#include "MainObj.h"

#define GRAVITY_SPEED 0.8
#define MAX_FALL_SPEED 10
#define PLAYER_SPEED 8

MainObj::MainObj()
{
    frame = 0;

    x_pos = 0;
    y_pos = 0;

    x_val = 0;
    y_val = 0;

    width_frame = 0;
    height_frame = 0;

    status = -1;

    input_type.left = 0;
    input_type.right = 0;
    input_type.down = 0;
    input_type.up = 0;
    input_type.jump = 0;

    on_ground = false;

    map_x = 0;
    map_y = 0;
}

MainObj::~MainObj()
{

}

bool MainObj::LoadImg(string path, SDL_Renderer* screen)
{
    bool ret = BaseObj::LoadImg(path, screen); // tan dung ham BaseObj da co
    if(ret == true) {
        // co tam anh theo chieu ngang nen phai chia thanh 8 frame
        width_frame = (rect.w)/8;
        height_frame = rect.h;
    }
    return ret;
}

void MainObj::set_clip()
{
    if(width_frame > 0 && height_frame > 0) {
        frame_clip[0].x = 0;
        frame_clip[0].y = 0;
        frame_clip[0].w = width_frame;
        frame_clip[0].h = height_frame;
    }

    for(int i = 1; i < 8; i++) {
        frame_clip[i].x = i*width_frame;
        frame_clip[i].y = 0;
        frame_clip[i].w = width_frame;
        frame_clip[i].h = height_frame;
    }
}

void MainObj::Shown(SDL_Renderer* des)
{
    if(status == WALK_LEFT) {
        LoadImg("img//player_left.png", des);
    }else LoadImg("img//player_right.png", des);

    if(input_type.left == 1 || input_type.right ==1) {
        frame++;
    }else frame = 0;

    if(frame >= 8) frame = 0;

    rect.x = x_pos - map_x;// cap nhat vi tri sau khi keo man hinh
    rect.y = y_pos - map_y;

    SDL_Rect* current_clip = &frame_clip[frame];
    SDL_Rect renderQuad = {rect.x, rect.y, width_frame, height_frame};
    SDL_RenderCopy(des, p_obj, current_clip, &renderQuad);
}

void MainObj::HandleInputAction(SDL_Event events, SDL_Renderer* screen)
{
    if(events.type == SDL_KEYDOWN) {
        switch (events.key.keysym.sym) {
            case SDLK_RIGHT: {
                status = WALK_RIGHT;
                input_type.right = 1;
                input_type.left = 0;
                break;
            }
            case SDLK_LEFT: {
                status = WALK_LEFT;
                input_type.left = 1;
                input_type.right = 0;
                break;
            }
            default: break;
        }
    }
    else if(events.type == SDL_KEYUP) {
        switch (events.key.keysym.sym) {
            case SDLK_RIGHT: {
                status = WALK_RIGHT;
                input_type.right = 0;
                break;
            }
            case SDLK_LEFT: {
                status = WALK_LEFT;
                input_type.left = 0;
                break;
            }
            default: break;
        }
    }
}

void MainObj::inMap(Map& map_data)
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
            if(map_data.tile[y1][x1] != 0 && map_data.tile[y2][x1] != 0) {
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
    y2 = (y_pos + y_val + height_frame + 1)/TILE_SIZE;

    if(x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y) {
        // roi xuong
        if(y_val > 0) {
            if(map_data.tile[y2][x1] != 0 && map_data.tile[y2][x2] != 0) {
                y_pos = y2*TILE_SIZE;
                y_pos -= (height_frame+1);
                y_val = 0;
                on_ground = true;

            }
        }
        // nhay len
        else if(y_val < 0) {
            if(map_data.tile[y1][x1] != 0 && map_data.tile[y1][x2] != 0) {
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
}

void MainObj::CenterEntityOnMap(Map& map_data)
{
    map_data.start_x = x_pos - (SCREEN_WIDTH/2);
    if(map_data.start_x < 0) {
        map_data.start_x = 0;
    }else if (map_data.start_x + SCREEN_WIDTH >= map_data.max_x) {
        map_data.start_x = map_data.max_x - SCREEN_WIDTH;
    }

    map_data.start_y = y_pos - (SCREEN_HEIGHT/2);
    if(map_data.start_y < 0) {
        map_data.start_y = 0;
    }else if (map_data.start_y + SCREEN_HEIGHT >= map_data.max_y) {
        map_data.start_y = map_data.max_y - SCREEN_HEIGHT;
    }
}

void MainObj::DoPlayer(Map& map_data)
{
    x_val = 0;
    y_val += GRAVITY_SPEED; // chuyen dong roi (vi truc y trong sdl nguoc voi truc y trong decate nen phai +

    if(y_val >= MAX_FALL_SPEED) {
        y_val = MAX_FALL_SPEED;
    }

    if(input_type.left == 1) {
        x_val -= PLAYER_SPEED;
    }
    else if(input_type.right == 1) {
        x_val += PLAYER_SPEED;
    }
    inMap(map_data);
    CenterEntityOnMap(map_data);
}

