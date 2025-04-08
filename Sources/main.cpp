#include "CommonFunc.h"
#include "BaseObj.h"
#include "map.h"
#include "MainObj.h"
#include "Timer.h"
#include "ThreadObj.h"
#include "TextObj.h"
#include "Survial.h"
#include "ExplosionObj.h"

BaseObj g_background;
BaseObj g_menu;
BaseObj g_win;
BaseObj g_lose;
TTF_Font* font_time = NULL;

bool InitData()
{
    bool success = true;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        cout << "SDL_Init thất bại: " << SDL_GetError() << endl;
        return false;
    }

    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
        cout << "Mix_OpenAudio thất bại: " << Mix_GetError() << endl;
        return false;
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    g_window = SDL_CreateWindow("Hunt Monster", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (g_window == NULL) {
        cout << "Không thể tạo cửa sổ: " << SDL_GetError() << endl;
        success = false;
    } else {
        g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (g_screen == NULL) {
            cout << "Không thể tạo renderer: " << SDL_GetError() << endl;
            success = false;
        } else {
            SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR,
                                   RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
            int imgFlags = IMG_INIT_PNG;
            if (!(IMG_Init(imgFlags) && imgFlags)) {
                cout << "IMG_Init thất bại: " << IMG_GetError() << endl;
                success = false;
            }
        }

        if (TTF_Init() == -1) {
            cout << "TTF_Init thất bại: " << TTF_GetError() << endl;
            success = false;
        }

        font_time = TTF_OpenFont("font//dlxfont_.ttf", 15);
        if (font_time == NULL) {
            cout << "Không thể tải font time: " << TTF_GetError() << endl;
            success = false;
        }

        g_sound_bullet = Mix_LoadWAV("sound//bullet.wav");
        if (g_sound_bullet == NULL) {
            cout << "Không thể tải 'bullet.wav': " << Mix_GetError() << endl;
            success = false;
        }

        g_sound_explosion = Mix_LoadWAV("sound//explosion.wav");
        if (g_sound_explosion == NULL) {
            cout << "Không thể tải 'explosion.wav': " << Mix_GetError() << endl;
            success = false;
        }

        g_sound_background = Mix_LoadMUS("sound//background.mp3");
        if (g_sound_background == NULL) {
            cout << "Không thể tải 'background.mp3': " << Mix_GetError() << endl;
            success = false;
        }
    }
    return success;
}

bool LoadBackGround()
{
    bool ret = g_background.LoadImg("img//background.png", g_screen);
    if (ret == false) {
        cout << "Không thể tải 'background.png': " << IMG_GetError() << endl;
        return false;
    }
    return true;
}

bool LoadMenuScreen()
{
    bool ret = g_menu.LoadImg("img//menu.png", g_screen);
    if (ret == false) {
        cout << "Không thể tải 'menu.png': " << IMG_GetError() << endl;
        return false;
    }
    return true;
}

bool LoadWinScreen()
{
    bool ret = g_win.LoadImg("img//win.png", g_screen);
    if (ret == false) {
        cout << "Không thể tải 'win.png': " << IMG_GetError() << endl;
        return false;
    }
    return true;
}

bool LoadLoseScreen()
{
    bool ret = g_lose.LoadImg("img//lose.png", g_screen);
    if (ret == false) {
        cout << "Không thể tải 'lose.png': " << IMG_GetError() << endl;
        return false;
    }
    return true;
}

void close()
{
    SDL_Delay(1000);

    g_background.Free();
    g_menu.Free();
    g_win.Free();
    g_lose.Free();

    SDL_DestroyRenderer(g_screen);
    g_screen = NULL;

    SDL_DestroyWindow(g_window);
    g_window = NULL;

    Mix_HaltMusic();
    Mix_FreeMusic(g_sound_background);
    Mix_CloseAudio();

    IMG_Quit();
    SDL_Quit();
}

vector<ThreadObj*> MakeThreadList()
{
    vector<ThreadObj*> list_thread;

    ThreadObj* dynamic_threads = new ThreadObj[10];
    for (int i = 0; i < 10; i++) {
        ThreadObj* p_thread = (dynamic_threads + i);
        if (p_thread != NULL) {
            p_thread->LoadImg("img//thread_left.png", g_screen);
            p_thread->set_clips();
            p_thread->set_type_move(ThreadObj::MOVE_IN_SPACE);
            p_thread->Set_X_pos(1000 + i * 2500);
            p_thread->Set_Y_pos(200);
            int pos1 = p_thread->Get_X_pos() - SPACE_MOVE / 2;
            int pos2 = p_thread->Get_X_pos() + SPACE_MOVE / 2;
            p_thread->set_animtion_pos(pos1, pos2);
            p_thread->set_input_left(1);

            list_thread.push_back(p_thread);
        }
    }

    ThreadObj* thread_objs = new ThreadObj[20];
    for (int i = 0; i < 20; i++) {
        ThreadObj* p_thread = (thread_objs + i);
        if (p_thread != NULL) {
            p_thread->LoadImg("img//thread_level.png", g_screen);
            p_thread->set_clips();
            p_thread->Set_X_pos(650 + i * 1200);
            p_thread->Set_Y_pos(250);

            BulletObj* p_bullet = new BulletObj();
            p_thread->InitBullet(p_bullet, g_screen);

            list_thread.push_back(p_thread);
        }
    }

    return list_thread;
}

int main(int argc, char* argv[])
{
    Timer fps_timer;
    Uint32 start_time = 0; // thoi diem bat dau choi game

    if (InitData() == false) return -1;
    if (LoadBackGround() == false) return -1;
    if(LoadMenuScreen() == false) return -1;
    if (LoadWinScreen() == false) return -1;
    if (LoadLoseScreen() == false) return -1;

    GameMap game_map;
    game_map.LoadMap("map//map01.dat");
    game_map.LoadTiles(g_screen);

    MainObj p_player;
    if (!p_player.LoadImg("img//player_right.png", g_screen)) {
        cout << "Không thể tải 'player_right.png': " << IMG_GetError() << endl;
        return -1;
    }
    p_player.set_clip();

    Survial pSurvival;
    pSurvival.Init(g_screen);

    TTF_Font* menuFont = TTF_OpenFont("font//dlxfont_.ttf", 24);
    if (menuFont == NULL) {
        cout << "Không thể tải font menu: " << TTF_GetError() << endl;
        return -1;
    }

    SDL_Color textColor = {255, 255, 255, 255};
    SDL_Surface* startSurface = TTF_RenderText_Solid(menuFont, "Start Game", textColor);
    if (startSurface == NULL) {
        cout << "Không thể render 'Start Game': " << TTF_GetError() << endl;
        return -1;
    }
    SDL_Texture* startTexture = SDL_CreateTextureFromSurface(g_screen, startSurface);
    if (startTexture == NULL) {
        cout << "Không thể tạo texture 'Start Game': " << SDL_GetError() << endl;
        return -1;
    }
    SDL_FreeSurface(startSurface);

    SDL_Surface* quitSurface = TTF_RenderText_Solid(menuFont, "Quit", textColor);
    if (quitSurface == NULL) {
        cout << "Không thể render 'Quit': " << TTF_GetError() << endl;
        return -1;
    }
    SDL_Texture* quitTexture = SDL_CreateTextureFromSurface(g_screen, quitSurface);
    if (quitTexture == NULL) {
        cout << "Không thể tạo texture 'Quit': " << SDL_GetError() << endl;
        return -1;
    }
    SDL_FreeSurface(quitSurface);

    int startW, startH;
    SDL_QueryTexture(startTexture, NULL, NULL, &startW, &startH);
    SDL_Rect startRect = {(SCREEN_WIDTH - startW) / 2, SCREEN_HEIGHT / 2 - 50, startW, startH};

    int quitW, quitH;
    SDL_QueryTexture(quitTexture, NULL, NULL, &quitW, &quitH);
    SDL_Rect quitRect = {(SCREEN_WIDTH - quitW) / 2, SCREEN_HEIGHT / 2 + 20, quitW, quitH};

    int ret = Mix_PlayMusic(g_sound_background, -1);
    if (ret == -1) {
        cout << "Không thể phát nhạc nền: " << Mix_GetError() << endl;
    }

    vector<ThreadObj*> thread_list = MakeThreadList();

    ExplosionObj exp_threat;
    bool tRet = exp_threat.LoadImg("img//exp3.png", g_screen);
    if (!tRet) {
        cout << "Không thể tải 'exp3.png': " << IMG_GetError() << endl;
        return -1;
    }
    exp_threat.set_clips();

    int num_die = 0;

    TextObj time_game;
    time_game.SetColor(TextObj::WHITE);

    GameState currenState = MENU;

    while (currenState != QUIT) {
        fps_timer.start();

        while (SDL_PollEvent(&g_event) != 0) {
            if (g_event.type == SDL_QUIT) {
                currenState = QUIT;
            }

            if (g_event.type == SDL_KEYDOWN && g_event.key.keysym.sym == SDLK_ESCAPE) {
                currenState = QUIT;
            }

            if (currenState == MENU) {
                if (g_event.type == SDL_MOUSEBUTTONDOWN && g_event.button.button == SDL_BUTTON_LEFT) {
                    int mouseX = g_event.button.x;
                    int mouseY = g_event.button.y;
                    SDL_Point mousePoint = {mouseX, mouseY};

                    if (SDL_PointInRect(&mousePoint, &startRect)) {
                        currenState = PLAYING; // Nhấp vào "Start Game"
                        start_time = SDL_GetTicks(); // Đặt thời gian bắt đầu khi vào trạng thái PLAYING
                    } else if (SDL_PointInRect(&mousePoint, &quitRect)) {
                        currenState = QUIT; // Nhấp vào "Quit"
                    }
                }
                if (g_event.type == SDL_KEYDOWN && g_event.key.keysym.sym == SDLK_ESCAPE) {
                    currenState = QUIT; // Thoát bằng phím Esc
                }
            } else if (currenState == PLAYING) {
                p_player.HandleInputAction(g_event, g_screen, g_sound_bullet);
            } else if (currenState == WIN || currenState == LOSE) {
                if (g_event.type == SDL_KEYDOWN && g_event.key.keysym.sym == SDLK_RETURN) {
                    currenState = MENU;
                    // Reset lại game để chơi lại từ đầu
                    num_die = 0;
                    p_player.Reset();
                    game_map.resetMap();
                    pSurvival.Init(g_screen);
                    thread_list.clear();
                    thread_list = MakeThreadList();
                    start_time = 0;
                }
            }
        }

        SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR,
                               RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
        SDL_RenderClear(g_screen);

        if (currenState == MENU) {
            g_menu.Render(g_screen, NULL);
            SDL_RenderCopy(g_screen, startTexture, NULL, &startRect);
            SDL_RenderCopy(g_screen, quitTexture, NULL, &quitRect);
        } else if (currenState == PLAYING) {
            g_background.Render(g_screen, NULL);

            Map map_data = game_map.getMap();

            p_player.HandleBullet(g_screen, map_data);
            p_player.setMapXY(map_data.start_x, map_data.start_y);
            p_player.DoPlayer(map_data);
            p_player.Shown(g_screen);

            game_map.setMap(map_data);
            game_map.DrawMap(g_screen);

            pSurvival.Shown(g_screen);

            for (int i = 0; i < (int)thread_list.size(); i++) {
                ThreadObj* p_thread = thread_list[i];
                if (p_thread != NULL) {
                    p_thread->SetMapXY(map_data.start_x, map_data.start_y);
                    p_thread->ImpMoveType(g_screen);
                    p_thread->DoPlayer(map_data);
                    p_thread->MakeBullet(g_screen, SCREEN_WIDTH, SCREEN_HEIGHT, map_data);
                    p_thread->Shown(g_screen);

                    SDL_Rect pRect = p_player.Get_Rect_Frame();
                    bool bCol1 = false;
                    vector<BulletObj*> tBullet_list = p_thread->get_bullet_list();
                    for (int j = 0; j < (int)tBullet_list.size(); j++) {
                        BulletObj* pt_bullet = tBullet_list[j];
                        if (pt_bullet != NULL) {
                            SDL_Rect bRect = pt_bullet->GetRect();
                            bCol1 = SDLCommonFunc::CheckCollision(bRect, pRect);
                            if (bCol1) {
                                p_thread->InitBullet(pt_bullet, g_screen);
                                break;
                            }
                        }
                    }

                    SDL_Rect tRect = p_thread->Get_Rect_Frame();
                    bool bCol2 = SDLCommonFunc::CheckCollision(pRect, tRect);
                    if (bCol1 || bCol2) {
                        num_die++;

                        if (num_die <= 3) {
                            p_player.SetRect(0, 0);
                            p_player.set_come_back_time(60);
                            SDL_Delay(1000);
                            pSurvival.Decrease();
                            pSurvival.Render(g_screen);
                            continue;
                        } else {
                            currenState = LOSE;
                            break;
                        }
                    }
                }
            }

            if (p_player.get_winner_status()) {
                currenState = WIN;
            }

            int frame_exp_width = exp_threat.get_width_frame();
            int frame_exp_height = exp_threat.get_height_frame();

            vector<BulletObj*> bullet_arr = p_player.get_bullet_list();
            for (int i = 0; i < (int)bullet_arr.size(); ++i) {
                BulletObj* p_bullet = bullet_arr[i];
                if (p_bullet != NULL) {
                    for (int j = 0; j < (int)thread_list.size(); ++j) {
                        ThreadObj* thread_obj = thread_list[j];
                        if (thread_obj != NULL) {
                            SDL_Rect tRect = thread_obj->Get_Rect_Frame();
                            SDL_Rect bRect = p_bullet->GetRect();
                            bool bCol = SDLCommonFunc::CheckCollision(bRect, tRect);

                            if (bCol) {
                                for (int ex = 0; ex < NUM_FRAME_EXP; ex++) {
                                    int x_pos = p_bullet->GetRect().x - frame_exp_width * 0.5;
                                    int y_pos = p_bullet->GetRect().y - frame_exp_height * 0.5;
                                    exp_threat.set_frame(ex);
                                    exp_threat.SetRect(x_pos, y_pos);
                                    exp_threat.Shown(g_screen);
                                }

                                int ret = Mix_PlayChannel(-1, g_sound_explosion, 0);
                                if (!ret) {
                                    cout << "Không thể phát âm thanh vụ nổ: " << Mix_GetError() << endl;
                                }

                                p_player.RemoveBullet(i);
                                thread_obj->Free();
                                thread_list.erase(thread_list.begin() + j);
                            }
                        }
                    }
                }
            }

            string str_time = "Time: ";
            Uint32 time_val = (SDL_GetTicks() - start_time)  / 1000;
            Uint32 val_time = 200 - time_val;

            if (val_time <= 0) {
                currenState = LOSE;
            } else {
                string time = to_string(val_time);
                str_time += time;

                time_game.SetText(str_time);
                time_game.LoadFromRenderText(font_time, g_screen);
                time_game.RenderText(g_screen, SCREEN_WIDTH - 200, 15);
            }

        } else if (currenState == WIN) {
            g_win.Render(g_screen, NULL);

        } else if (currenState == LOSE) {
            g_lose.Render(g_screen, NULL);

        }

        SDL_RenderPresent(g_screen);

        int real_time = fps_timer.get_ticks();
        int time_per_frame = 1000 / FRAME_PER_SECOND;
        if (real_time < time_per_frame) {
            int delay_time = time_per_frame - real_time;
            if (delay_time >= 0) {
                SDL_Delay(delay_time);
            }
        }
    }

    for (int i = 0; i < (int)thread_list.size(); i++) {
        ThreadObj* p_thread = thread_list[i];
        if (p_thread != NULL) {
            p_thread->Free();
            p_thread = NULL;
        }
    }
    thread_list.clear();

    SDL_DestroyTexture(startTexture);
    SDL_DestroyTexture(quitTexture);
    TTF_CloseFont(menuFont);

    close();

    return 0;
}
