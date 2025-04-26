#ifndef BOSS_OBJ_H_
#define BOSS_OBJ_H_

#include "ThreadObj.h"
#include "BaseObj.h"
#include "CommonFunc.h"
#include "BulletObj.h"
#include "MainObj.h"
#include "ExplosionObj.h"

#define NORMAL_FRAME_NUM 8
#define FAST_FRAME_NUM 6
#define DEAD_FRAME_NUM 6
#define NORMAL_FRAME_WIDTH 128
#define NORMAL_FRAME_HEIGHT 169
#define FAST_FRAME_WIDTH 192
#define FAST_FRAME_HEIGHT 169
#define DEAD_FRAME_WIDTH 192
#define DEAD_FRAME_HEIGHT 169
#define BOSS_HEALTH 10
#define FAST_MOVE_DURATION 60
#define NORMAL_MOVE_DURATION 300
#define BOSS_NORMAL_SPEED 2
#define BOSS_FAST_SPEED 4
#define DEATH_ANIMATION_DELAY 10

class BossObj : public ThreadObj
{
public:
    BossObj();
    virtual ~BossObj();

    void MoveNormal(Map& map_data);
    void MoveFastToPlayer(Map& map_data);
    void DoPlayer(Map& map_data) override;
    bool LoadImg(string path, SDL_Renderer* screen) override;
    void set_clips() override;
    void Shown(SDL_Renderer* des) override;
    void HandlePlayerBullet(const std::vector<BulletObj*>& player_bullets, SDL_Renderer* screen, Mix_Chunk* explosion_sound);
    void ImpMoveType(SDL_Renderer* screen) override;
    bool IsDead() const { return is_dead; }
    bool IsDeathAnimationComplete() const { return is_death_animation_complete; }
    void SetPlayerPosition(int x, int y);
    SDL_Rect Get_Rect_Frame() const;
    void SetExplosionSound(Mix_Chunk* sound) { explosion_sound = sound; }

private:
    int health;
    float player_x;
    float player_y;
    SDL_Rect normal_frame_clip[NORMAL_FRAME_NUM];
    SDL_Rect fast_frame_clip[FAST_FRAME_NUM];
    SDL_Rect dead_frame_clip[DEAD_FRAME_NUM];
    int frame;
    int dead_frame;
    bool is_dead;
    bool is_death_animation_complete;
    SDL_Texture* dead_left_texture;
    SDL_Texture* dead_right_texture;
    SDL_Texture* run_texture;
    SDL_Rect player_rect;
    SDL_Texture* texture_;
    SDL_Rect dead_position;
    int move_timer;
    bool is_moving_fast;
    int direction_change_timer;
    int last_direction;
    ExplosionObj exp_boss;
    bool show_explosion;
    SDL_Rect explosion_position;
    int explosion_frame;
    Mix_Chunk* explosion_sound;
    int death_animation_timer;
};

#endif // BOSS_OBJ_H_
