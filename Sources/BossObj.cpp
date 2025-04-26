#include "BossObj.h"
#include "CommonFunc.h"

BossObj::BossObj()
{
    health = BOSS_HEALTH;
    is_dead = false;
    is_death_animation_complete = false;
    move_timer = 0;
    is_moving_fast = false;
    player_x = 0;
    player_y = 0;
    frame = 0;
    dead_frame = 0;
    death_animation_timer = 0;
    width_frame = NORMAL_FRAME_WIDTH;
    height_frame = NORMAL_FRAME_HEIGHT;
    dead_left_texture = nullptr;
    dead_right_texture = nullptr;
    run_texture = nullptr;
    x_val = -BOSS_NORMAL_SPEED;
    direction_change_timer = 0;
    last_direction = -1; // Mặc định hướng trái
    show_explosion = false;
    explosion_frame = 0;
    exp_boss.set_clips();
    explosion_sound = NULL;
}

BossObj::~BossObj()
{
    if (dead_left_texture) {
        SDL_DestroyTexture(dead_left_texture);
    }
    if (dead_right_texture) {
        SDL_DestroyTexture(dead_right_texture);
    }
    if (run_texture) {
        SDL_DestroyTexture(run_texture);
    }
}

void BossObj::SetPlayerPosition(int x, int y)
{
    player_x = x;
    player_y = y;
    player_rect = {x, y, 64, 64};
}

bool BossObj::LoadImg(string path, SDL_Renderer* screen)
{
    bool ret = BaseObj::LoadImg(path, screen);
    if (!ret) {
        return false;
    }
    if (ret) {
        width_frame = is_moving_fast ? FAST_FRAME_WIDTH : NORMAL_FRAME_WIDTH;
        height_frame = is_moving_fast ? FAST_FRAME_HEIGHT : NORMAL_FRAME_HEIGHT;
        exp_boss.LoadImg("img//exp3.png", screen);
        exp_boss.set_clips();
    }

    // Load texture bị hạ gục hướng trái
    if (dead_left_texture == nullptr) {
        SDL_Surface* surface_left = IMG_Load("img//boss_dead_left.png");
        if (surface_left == NULL) {
            cout << "Failed to load boss_dead_left.png: " << IMG_GetError() << endl;
            return false;
        }
        dead_left_texture = SDL_CreateTextureFromSurface(screen, surface_left);
        if (dead_left_texture == NULL) {
            cout << "Failed to create dead_left_texture: " << SDL_GetError() << endl;
            SDL_FreeSurface(surface_left);
            return false;
        }
        SDL_FreeSurface(surface_left);
    }

    // Load texture bị hạ gục hướng phải
    if (dead_right_texture == nullptr) {
        SDL_Surface* surface_right = IMG_Load("img//boss_dead_right.png");
        if (surface_right == NULL) {
            cout << "Failed to load boss_dead_right.png: " << IMG_GetError() << endl;
            return false;
        }
        dead_right_texture = SDL_CreateTextureFromSurface(screen, surface_right);
        if (dead_right_texture == NULL) {
            cout << "Failed to create dead_right_texture: " << SDL_GetError() << endl;
            SDL_FreeSurface(surface_right);
            return false;
        }
        SDL_FreeSurface(surface_right);
    }

    return true;
}

void BossObj::Shown(SDL_Renderer* des)
{
    if (is_dead) {
        SDL_Texture* target_texture = (last_direction > 0) ? dead_right_texture : dead_left_texture;
        if (target_texture == NULL) {
            return;
        }

        int screen_x = dead_position.x - map_x;
        int screen_y = dead_position.y - map_y;

        if (screen_x < -DEAD_FRAME_WIDTH || screen_x > SCREEN_WIDTH ||
            screen_y < -DEAD_FRAME_HEIGHT || screen_y > SCREEN_HEIGHT) {
            return;
        }

        if(!is_death_animation_complete) {
            death_animation_timer++;
            if (death_animation_timer >= DEATH_ANIMATION_DELAY) {
                death_animation_timer = 0;
               // Nếu hướng bên phải (last_direction > 0), frame chạy thuận
               // Nếu hướng bên trái (last_direction < 0), frame chạy ngược
               if (last_direction > 0) {
                   dead_frame++;
                   if (dead_frame >= DEAD_FRAME_NUM) {
                       dead_frame = DEAD_FRAME_NUM - 1; // Cố định frame cuối cùng
                       is_death_animation_complete = true;
                   }
               } else {
                   dead_frame--;
                   if (dead_frame < 0) {
                       dead_frame = 0; // Cố định frame đầu tiên
                       is_death_animation_complete = true;
                   }
               }
            }
        }

        SDL_Rect* current_clip = &dead_frame_clip[dead_frame];
        SDL_Rect renderQuad = {screen_x, screen_y, DEAD_FRAME_WIDTH, DEAD_FRAME_HEIGHT};
        SDL_RenderCopy(des, target_texture, current_clip, &renderQuad);
    } else {
        if (p_obj == NULL) return;
        rect.x = x_pos - map_x;
        rect.y = y_pos - map_y;
        if (rect.x < -width_frame || rect.x > SCREEN_WIDTH ||
            rect.y < -height_frame || rect.y > SCREEN_HEIGHT) {
            return;
        }

        int max_frame = is_moving_fast ? FAST_FRAME_NUM : NORMAL_FRAME_NUM;
        frame++;
        if (frame >= max_frame) frame = 0;

        SDL_Rect* current_clip = is_moving_fast ? &fast_frame_clip[frame] : &normal_frame_clip[frame];
        SDL_Rect renderQuad = {rect.x, rect.y, width_frame, height_frame};
        SDL_RenderCopy(des, p_obj, current_clip, &renderQuad);
    }

    if (show_explosion) {
        exp_boss.set_frame(explosion_frame);
        exp_boss.SetRect(explosion_position.x - map_x, explosion_position.y - map_y);
        exp_boss.Shown(des);

        explosion_frame++;
        if (explosion_frame >= NUM_FRAME_EXP) {
            show_explosion = false;
            explosion_frame = 0;
        }
    }
}

void BossObj::set_clips()
{
    // Set clip cho normal movement (8 frame)
    for (int i = 0; i < NORMAL_FRAME_NUM; i++) {
        normal_frame_clip[i].x = i * NORMAL_FRAME_WIDTH;
        normal_frame_clip[i].y = 0;
        normal_frame_clip[i].w = NORMAL_FRAME_WIDTH;
        normal_frame_clip[i].h = NORMAL_FRAME_HEIGHT;
    }

    // Set clip cho fast movement (6 frame)
    for (int i = 0; i < FAST_FRAME_NUM; i++) {
        fast_frame_clip[i].x = i * FAST_FRAME_WIDTH;
        fast_frame_clip[i].y = 0;
        fast_frame_clip[i].w = FAST_FRAME_WIDTH;
        fast_frame_clip[i].h = FAST_FRAME_HEIGHT;
    }

    // Set clip cho death animation (6 frame)
    for (int i = 0; i < DEAD_FRAME_NUM; i++) {
        dead_frame_clip[i].x = i * DEAD_FRAME_WIDTH;
        dead_frame_clip[i].y = 0;
        dead_frame_clip[i].w = DEAD_FRAME_WIDTH;
        dead_frame_clip[i].h = DEAD_FRAME_HEIGHT;
    }
}

SDL_Rect BossObj::Get_Rect_Frame() const
{
    SDL_Rect rect_frame;
    rect_frame.x = x_pos + width_frame * 0.1; // Dịch chuyển để bỏ vùng trong suốt bên trái
    rect_frame.y = y_pos + height_frame * 0.2; // Dịch chuyển để bỏ vùng trong suốt phía trên
    rect_frame.w = width_frame * 0.8; // Giảm 20% chiều rộng
    rect_frame.h = height_frame * 0.6; // Giảm 40% chiều cao
    return rect_frame;
}

void BossObj::MoveNormal(Map& map_data)
{
    if (direction_change_timer > 0) {
        direction_change_timer--;
        return;
    }

    int x1 = (x_pos + x_val) / TILE_SIZE;
    int x2 = (x_pos + x_val + width_frame - 1) / TILE_SIZE;
    int y1 = y_pos / TILE_SIZE;
    int y2 = (y_pos + height_frame - 1) / TILE_SIZE;

    if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y) {
        if (x_val > 0 && (map_data.tile[y1][x2] != 0 || map_data.tile[y2][x2] != 0)) {
            x_pos = x2 * TILE_SIZE - width_frame - 1;
            x_val = -BOSS_NORMAL_SPEED;
            last_direction = -1;
            direction_change_timer = 10;
        }
        else if (x_val < 0 && (map_data.tile[y1][x1] != 0 || map_data.tile[y2][x1] != 0)) {
            x_pos = (x1 + 1) * TILE_SIZE;
            x_val = BOSS_NORMAL_SPEED;
            last_direction = 1;
            direction_change_timer = 10;
        }
    }
    x_pos += x_val;
}

void BossObj::MoveFastToPlayer(Map& map_data)
{
    float distance_to_player = player_x - x_pos;

    if (abs(distance_to_player) < 100) {
        x_val = 0;
        return;
    }

    int direction = (distance_to_player > 0) ? 1 : -1;
    int step = BOSS_FAST_SPEED * direction;

    x_val = step;
    last_direction = direction;

    for (int i = 0; i < abs(step); i++) {
        int temp_x_pos = x_pos + direction;
        int x1 = temp_x_pos / TILE_SIZE;
        int x2 = (temp_x_pos + width_frame - 1) / TILE_SIZE;
        int y1 = y_pos / TILE_SIZE;
        int y2 = (y_pos + height_frame - 1) / TILE_SIZE;

        if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y) {
            if (direction > 0 && (map_data.tile[y1][x2] != 0 || map_data.tile[y2][x2] != 0)) {
                x_pos = x2 * TILE_SIZE - width_frame - 1;
                x_val = 0;
                break;
            }
            else if (direction < 0 && (map_data.tile[y1][x1] != 0 || map_data.tile[y2][x1] != 0)) {
                x_pos = (x1 + 1) * TILE_SIZE;
                x_val = 0;
                break;
            }
            else {
                x_pos += direction;
            }
        } else {
            x_pos += direction;
        }
    }
}

void BossObj::DoPlayer(Map& map_data)
{
    if (is_dead) return; // Stop updating movement when dead

    move_timer++;
    if (!is_moving_fast) {
        if (move_timer >= NORMAL_MOVE_DURATION) {
            is_moving_fast = true;
            move_timer = 0;
            width_frame = FAST_FRAME_WIDTH;
            height_frame = FAST_FRAME_HEIGHT;
            set_clips();
        }
    } else {
        if (move_timer >= FAST_MOVE_DURATION) {
            is_moving_fast = false;
            move_timer = 0;
            width_frame = NORMAL_FRAME_WIDTH;
            height_frame = NORMAL_FRAME_HEIGHT;
            set_clips();
        }
    }

    if (is_moving_fast) {
        MoveFastToPlayer(map_data);
    } else {
        if (x_val == 0) {
            x_val = BOSS_NORMAL_SPEED;
        }
        MoveNormal(map_data);
    }

    y_val += THREAD_GRAVITY_SPEED;
    if (y_val > THREAD_MAX_SPEED_FALL) {
        y_val = THREAD_MAX_SPEED_FALL;
    }

    float temp_x_pos = x_pos + x_val;
    float temp_y_pos = y_pos + y_val;

    // Chỉ kiểm tra va chạm dọc
    int x1 = x_pos / TILE_SIZE;
    int x2 = (x_pos + width_frame - 1) / TILE_SIZE;
    int y1 = temp_y_pos / TILE_SIZE;
    int y2 = (temp_y_pos + height_frame - 1) / TILE_SIZE;

    if (y1 >= 0 && y2 < MAX_MAP_Y) {
        if (y_val > 0 && (map_data.tile[y2][x1] != 0 || map_data.tile[y2][x2] != 0)) {
            temp_y_pos = y2 * TILE_SIZE - height_frame - 1;
            y_val = 0;
        }
    }

    x_pos = temp_x_pos;
    y_pos = temp_y_pos;

    if (x_pos < 0) {
        x_pos = 0;
        x_val = BOSS_NORMAL_SPEED;
    }
    if (x_pos + width_frame > map_data.max_x) {
        x_pos = map_data.max_x - width_frame;
        x_val = -BOSS_NORMAL_SPEED;
    }
    if (y_pos < 0) y_pos = 0;
    if (y_pos + height_frame > map_data.max_y) y_pos = map_data.max_y - height_frame;

    rect.x = x_pos - map_data.start_x;
    rect.y = y_pos - map_data.start_y;
}

void BossObj::HandlePlayerBullet(const std::vector<BulletObj*>& player_bullets, SDL_Renderer* screen, Mix_Chunk* sound_explosion)
{
    if (is_dead) {
        return;
    }

    for (auto bullet : player_bullets) {
        if (bullet->Get_Is_Move()) {
            SDL_Rect bullet_rect = bullet->GetRect();
            SDL_Rect boss_rect = Get_Rect_Frame();

            if (SDL_HasIntersection(&bullet_rect, &boss_rect)) {
                health--;
                bullet->Set_Is_Move(false);

                show_explosion = true;
                explosion_frame = 0;
                explosion_position.x = bullet_rect.x - exp_boss.get_width_frame() * 0.5;
                explosion_position.y = bullet_rect.y - exp_boss.get_height_frame() * 0.5;

                if (explosion_sound != NULL) {
                    int ret = Mix_PlayChannel(-1, explosion_sound, 0);
                    if (ret == -1) {
                        cout << Mix_GetError() << endl;
                    }
                }

                if (health <= 0) {
                    is_dead = true;
                    dead_position.x = x_pos;
                    dead_position.y = y_pos;
                    last_direction = (x_val > 0) ? 1 : -1;
                    // Thiết lập frame ban đầu dựa theo hướng
                    if (last_direction > 0) {
                        // Bên phải, bắt đầu từ frame đầu tiên (chạy thuận)
                        dead_frame = 0;
                    } else {
                        // Bên trái, bắt đầu từ frame cuối cùng (chạy ngược)
                        dead_frame = DEAD_FRAME_NUM - 1;
                    }
                    is_death_animation_complete = false;
                    death_animation_timer = 0;
                    break;
                }
            }
        }
    }
}

void BossObj::ImpMoveType(SDL_Renderer* screen)
{
    if (is_dead) {
        return;
    }

    if (is_moving_fast) {
        if (x_val > 0) {
            LoadImg("img//boss_run_right.png", screen);
            last_direction = 1;
        } else if (x_val < 0) {
            LoadImg("img//boss_run_left.png", screen);
            last_direction = -1;
        } else {
            if (last_direction > 0) {
                LoadImg("img//boss_run_right.png", screen);
            } else {
                LoadImg("img//boss_run_left.png", screen);
            }
        }
    } else {
        if (x_val > 0) {
            LoadImg("img//boss_walk_right.png", screen);
            last_direction = 1;
        } else if (x_val < 0) {
            LoadImg("img//boss_walk_left.png", screen);
            last_direction = -1;
        } else {
            if (last_direction > 0) {
                LoadImg("img//boss_walk_right.png", screen);
            } else {
                LoadImg("img//boss_walk_left.png", screen);
            }
        }
    }
}
