#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include <windows.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

using namespace std;

static SDL_Window* g_window = NULL;
static SDL_Renderer* g_screen = NULL;
static SDL_Event g_event;

// Screen
const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 640;
const int SCREEN_BPP = 32;

const int COLOR_Key_R = 167;
const int COLOR_Key_G = 175;
const int COLOR_Key_B = 180;

const int RENDER_DRAW_COLOR = 0xff;

const int TILE_SIZE = 64;
const int MAX_MAP_X = 400;
const int MAX_MAP_Y = 10;

struct Map {
    int start_x;
    int start_y;

    int max_x;
    int max_y;

    int tile[MAX_MAP_X][MAX_MAP_Y];
    char* file_name;
};

