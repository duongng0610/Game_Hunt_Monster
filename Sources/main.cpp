#include "CommonFunc.h"
#include "BaseObj.h"
#include "map.h"
#include "MainObj.h"
#include "Timer.h"
#include "ThreadObj.h"

BaseObj g_background;

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
        g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);

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
    ThreadObj* thread_objs = new ThreadObj[20];
    for(int i = 0; i < 20; i++) {
        ThreadObj* p_thread = (thread_objs + i);
        if(p_thread != NULL) {
            p_thread ->LoadImg("img//thread_level.png", g_screen);
            p_thread ->set_clips();
            p_thread ->Set_X_pos(700 + i*1200);
            p_thread ->Set_Y_pos(250);

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

    vector<ThreadObj*> thread_list = MakeThreadList();

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

        p_player.HandleBullet(g_screen);
        p_player.setMapXY(map_data.start_x, map_data.start_y);
        p_player.DoPlayer(map_data);
        p_player.Shown(g_screen);

        // cap nhat ban do khi di chuyen
        game_map.setMap(map_data);
        game_map.DrawMap(g_screen);

        for(int i = 0; i < (int)thread_list.size(); i++) {
            ThreadObj* p_thread = thread_list[i];
            if(p_thread != NULL) {
                p_thread ->SetMapXY(map_data.start_x, map_data.start_y);
                p_thread ->DoPlayer(map_data);
                p_thread ->Shown(g_screen);
            }
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

    close();

    return 0;
}
