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

    animation_min = 0;
    animation_max = 0;

    input_type.left = 0;
    type_move = STATIC_THREAD;
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

SDL_Rect ThreadObj::Get_Rect_Frame()
{
    SDL_Rect tRect;

    tRect.x = rect.x;
    tRect.y = rect.y;
    tRect.w = width_frame;
    tRect.h = height_frame;

    return tRect;
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

        if(input_type.left == 1) {
            x_val -= THREAD_SPEED;
        }else if(input_type.right == 1) {
            x_val += THREAD_SPEED;
        }

        inMap(map_data);
    }
    else if(come_back > 0) {
        come_back--;
        if(come_back == 0) {
            InitThread();
        }
    }
}

void ThreadObj::InitThread()
{
    x_val = 0;
    y_val = 0;

    if(x_pos > 256) {
        x_pos -= 256;
        // cap nhat khoang di chuyen moi
        animation_min -= 256;
        animation_max -= 256;
    }else x_pos = 0;

    y_pos = 0;
    come_back = 0;
    input_type.left = 1;
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

               animation_max = x_pos; // gioi han khi gap vat can
           }
        }
        // di chuyen sang trai
        else if(x_val < 0) {
            if(map_data.tile[y1][x1] != 0 || map_data.tile[y2][x1] != 0) {
                x_pos = (x1+1)*TILE_SIZE;
                x_val = 0;

                animation_min = x_pos; // gioi han khi gap vat can
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

void ThreadObj::ImpMoveType(SDL_Renderer* screen)
{
    if(type_move == STATIC_THREAD) {
        ;//
    }else {
        if(on_ground == true) {
            if(x_pos >= animation_max) {
                input_type.left = 1;
                input_type.right = 0;
                LoadImg("img//thread_left.png", screen);
            }else if(x_pos <= animation_min) {
                input_type.left = 0;
                input_type.right = 1;
                LoadImg("img//thread_right.png", screen);
            }
        }else {
            if(input_type.left == 1) {
                LoadImg("img//thread_left.png", screen);
            }
        }
    }
}

void ThreadObj::InitBullet(BulletObj* p_bullet, SDL_Renderer* screen)
{
    if(p_bullet != NULL) {
        bool ret = p_bullet ->LoadImg("img//thread_bullet.png", screen);

        if(ret) {
            p_bullet ->Set_Is_Move(true);
            p_bullet ->Set_Bullet_Dir(BulletObj::DIR_LEFT);
            p_bullet ->SetRect(x_pos + 5, y_pos + 10);
            p_bullet ->Set_X_val(5);

            bullet_list.push_back(p_bullet);
        }
    }
}

void ThreadObj::MakeBullet(SDL_Renderer* screen, const int& x_limit, const int& y_limit, Map& map_data)
{
    for(int i = 0; i < (int)bullet_list.size(); i++) {
        BulletObj* p_bullet = bullet_list[i];
        if(p_bullet != NULL) {
            if(p_bullet ->Get_Is_Move()) {
                int bullet_distance = rect.x + width_frame - p_bullet->GetRect().x;
                if(bullet_distance < 300 && bullet_distance > 0) {
                    p_bullet ->HandleMove(x_limit, y_limit, map_data);
                    p_bullet ->Render(screen);
                }else p_bullet ->Set_Is_Move(false);

            }else {
                p_bullet ->Set_Is_Move(true);
                p_bullet ->SetRect(rect.x + 5, rect.y + 10);
            }
        }
    }
}

void ThreadObj::RemoveBullet(const int& index)
{
    int num = (int)bullet_list.size();
    if(num > 0 && index < num) {
        BulletObj* t_bullet = bullet_list[index];
        bullet_list.erase(bullet_list.begin() + index);

        if(t_bullet != NULL) {
            delete t_bullet;
            t_bullet = NULL;
        }
    }
}
