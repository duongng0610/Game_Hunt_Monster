#include "CommonFunc.h"
#include "BaseObj.h"
#include "map.h"
#include "MainObj.h"
#include "Timer.h"
#include "ThreadObj.h"
#include "TextObj.h"
#include "Survial.h"

BaseObj g_background;
TTF_Font* font_time = NULL;

bool InitData()
{
    bool success = true;

    int ret = SDL_Init(SDL_INIT_VIDEO);
    if(ret < 0) return false;

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    g_window = SDL_CreateWindow("Hunt Monster", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if(g_window == NULL) {
        success = false;
    }else {
        g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED| SDL_RENDERER_PRESENTVSYNC);

        if(g_screen == NULL) {
                success = false;
        }else {
            SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR,
                                   RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
            int imgFlags = IMG_INIT_PNG;
            if(!(IMG_Init(imgFlags) && imgFlags)) {
                success = false;
            }
        }

        if(TTF_Init() == -1) {
            success = false;
        }

        font_time = TTF_OpenFont("font//dlxfont_.ttf", 15);
        if(font_time == NULL) {
            success = false;
        }
    }
    return success;
}

bool LoadBackGround()
{
    bool ret = g_background.LoadImg("img//background.png", g_screen);
    if (ret == false) {
        return false;
    }
    return true;
}

void close()
{
    g_background.Free();

    SDL_DestroyRenderer(g_screen);
    g_screen = NULL;

    SDL_DestroyWindow(g_window);
    g_window = NULL;

    IMG_Quit();
    SDL_Quit();
}

vector<ThreadObj*> MakeThreadList()
{
    vector<ThreadObj*> list_thread;

    // thread dong
    ThreadObj* dynamic_threads = new ThreadObj[10];
    for(int i = 0; i < 10; i++) {
        ThreadObj* p_thread = (dynamic_threads + i);
        if(p_thread != NULL) {
            // khoi tao thread
            p_thread ->LoadImg("img//thread_left.png", g_screen);
            p_thread ->set_clips();
            p_thread ->set_type_move(ThreadObj::MOVE_IN_SPACE);
            p_thread ->Set_X_pos(1000 + i*2500);
            p_thread ->Set_Y_pos(200);
            // xet khoang di chuyen cho thread
            int pos1 = p_thread ->Get_X_pos() - SPACE_MOVE/2;
            int pos2 = p_thread ->Get_X_pos() + SPACE_MOVE/2;
            p_thread ->set_animtion_pos(pos1, pos2);
            p_thread ->set_input_left(1);

            list_thread.push_back(p_thread);
        }
    }

    // thread tinh
    ThreadObj* thread_objs = new ThreadObj[20];
    for(int i = 0; i < 20; i++) {
        ThreadObj* p_thread = (thread_objs + i);
        if(p_thread != NULL) {
            p_thread ->LoadImg("img//thread_level.png", g_screen);
            p_thread ->set_clips();
            p_thread ->Set_X_pos(650 + i*1200);
            p_thread ->Set_Y_pos(250);

            BulletObj* p_bullet = new BulletObj();
            p_thread ->InitBullet(p_bullet, g_screen);

            list_thread.push_back(p_thread);
        }
    }

    return list_thread;
}

int main(int argc, char* argv[])
{
    Timer fps_timer;

    if(InitData() == false) {
        cout << "Can not load data";
        return -1;
    }

    if(LoadBackGround() == false) {
        cout << "Can not load backgroud";
        return -1;
    }

    GameMap game_map;
    game_map.LoadMap("map//map01.dat");
    game_map.LoadTiles(g_screen);

    MainObj p_player;
    p_player.LoadImg("img//player_right.png", g_screen);
    p_player.set_clip();

    Survial pSurvival;
    pSurvival.Init(g_screen);

    vector<ThreadObj*> thread_list = MakeThreadList();

    int num_die = 0;

    //Time text
    TextObj time_game;
    time_game.SetColor(TextObj::WHITE);

    bool running = true;
    while(running) {
        fps_timer.start();

        while(SDL_PollEvent(&g_event) != 0) {
            if(g_event.type == SDL_QUIT) {
                running = false;
            }
            p_player.HandleInputAction(g_event, g_screen);
        }

        SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR,
                               RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
        SDL_RenderClear(g_screen);

        g_background.Render(g_screen, NULL);

        Map map_data = game_map.getMap();

        p_player.HandleBullet(g_screen, map_data);
        p_player.setMapXY(map_data.start_x, map_data.start_y);
        p_player.DoPlayer(map_data);
        p_player.Shown(g_screen);

        // cap nhat ban do khi di chuyen
        game_map.setMap(map_data);
        game_map.DrawMap(g_screen);

        pSurvival.Shown(g_screen);

        for(int i = 0; i < (int)thread_list.size(); i++) {
            ThreadObj* p_thread = thread_list[i];
            if(p_thread != NULL) {
                p_thread ->SetMapXY(map_data.start_x, map_data.start_y);
                p_thread ->ImpMoveType(g_screen);
                p_thread ->DoPlayer(map_data);
                p_thread ->MakeBullet(g_screen, SCREEN_WIDTH, SCREEN_HEIGHT, map_data);
                p_thread ->Shown(g_screen);

                //kiem tra va cham cua player voi dan cua thread
                SDL_Rect pRect = p_player.Get_Rect_Frame();
                bool bCol1 = false;
                vector<BulletObj*> tBullet_list = p_thread->get_bullet_list();
                for(int j = 0; j < (int)tBullet_list.size(); j++) {
                    BulletObj* pt_bullet = tBullet_list[j];
                    if(pt_bullet != NULL) {
                        SDL_Rect bRect = pt_bullet ->GetRect();
                        bCol1 = SDLCommonFunc::CheckCollision(bRect, pRect);
                        if(bCol1) {
                            p_thread ->InitBullet(pt_bullet, g_screen);
                            break;
                        }
                    }
                }

                // kiem tra va cham cua player voi thread
                SDL_Rect tRect = p_thread ->Get_Rect_Frame();
                bool bCol2 = SDLCommonFunc::CheckCollision(pRect, tRect);
                if(bCol1 || bCol2) {
                    num_die++;

                    if(num_die <= 3) {
                        p_player.SetRect(0, 0);
                        p_player.set_come_back_time(60);
                        SDL_Delay(1000);
                        pSurvival.Decrease();
                        pSurvival.Render(g_screen);
                        continue;
                    }else {
                        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Info", "GAME OVER", g_window);
                        p_thread->Free();
                        close();
                        return 0;
                    }

                }
            }
        }

        // kiem tra xem da ve dich chua
        if(p_player.get_winner_status()) {
           SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Congratulation", "YOU ARE WINNER!!!", g_window);
           running = false;
           break;
        }

        // kiem tra dan cua player
        vector<BulletObj*> bullet_arr = p_player.get_bullet_list();
        for(int i = 0; i < (int)bullet_arr.size(); ++i) {
            BulletObj* p_bullet = bullet_arr[i];
            if(p_bullet != NULL) {
                // kiem tra va cham voi thread
                for(int j = 0; j < (int)thread_list.size(); ++j) {
                    ThreadObj* thread_obj = thread_list[j];
                    if(thread_obj != NULL) {
                        // vi tri cua thread
                        SDL_Rect tRect = thread_obj ->Get_Rect_Frame();
                        // vi tri dan cua player
                        SDL_Rect bRect = p_bullet ->GetRect();
                        // kiem tra va cham
                        bool bCol = SDLCommonFunc::CheckCollision(bRect, tRect);

                        if(bCol) {
                            p_player.RemoveBullet(i);
                            thread_obj ->Free();
                            thread_list.erase(thread_list.begin() + j);
                        }
                    }
                }
            }
        }

        // in ra thoi gian trong game
        string str_time = "Time: ";
        Uint32 time_val = SDL_GetTicks() / 1000; // tinh theo s
        Uint32 val_time = 300 - time_val;

        if(val_time <= 0) {
           SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Info", "GAME OVER", g_window);
           running = false;
           break;
        }else {
            string time = to_string(val_time);
            str_time += time;

            time_game.SetText(str_time);
            time_game.LoadFromRenderText(font_time, g_screen);
            time_game.RenderText(g_screen, SCREEN_WIDTH - 200, 15);
        }

        SDL_RenderPresent(g_screen);

        // xu ly thoi gian (tao do tre)
        int real_time = fps_timer.get_ticks();
        int time_per_frame = 1000/FRAME_PER_SECOND; // ms

        if(real_time < time_per_frame) {
            int delay_time = time_per_frame - real_time;
            if(delay_time >= 0) {
                SDL_Delay(delay_time);
            }
        }
    }

    // loai bo thread khi dong chuong trinh
    for(int i = 0; i < (int)thread_list.size(); i++) {
        ThreadObj* p_thread = thread_list[i];
        if(p_thread != NULL) {
            p_thread ->Free();
            p_thread = NULL;
        }
    }
    thread_list.clear();

    close();

    return 0;
}
