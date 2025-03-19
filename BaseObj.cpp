#include  "BaseObj.h"

BaseObj::BaseObj()
{
    p_obj = NULL;
    rect.x = 0;
    rect.y = 0;
    rect.w = 0;
    rect.h = 0;
}

BaseObj::~BaseObj()
{
   Free();
}

bool BaseObj::LoadImg(string path, SDL_Renderer* screen)
{
    SDL_Texture* new_texture = NULL;

    SDL_Surface* load_surface = IMG_Load(path.c_str());
    if(load_surface != NULL) {
        SDL_SetColorKey(load_surface, SDL_TRUE, SDL_MapRGB(load_surface-> format, COLOR_Key_R, COLOR_Key_G, COLOR_Key_B));
        new_texture = SDL_CreateTextureFromSurface(screen, load_surface);

        if(new_texture != NULL) {
            rect.w = load_surface -> w;
            rect.h = load_surface -> h;
        }

        SDL_FreeSurface(load_surface);
    }

    p_obj = new_texture;

    return p_obj != NULL;
}

void BaseObj::Render(SDL_Renderer* des, const SDL_Rect* clip)
{
    SDL_Rect renderquad = {rect.x, rect.y, rect.w, rect.h};

    SDL_RenderCopy(des, p_obj, clip, &renderquad);
}

void BaseObj::Free()
{
    if(p_obj != NULL) {
        SDL_DestroyTexture(p_obj);
        p_obj = NULL;
        rect.w = 0;
        rect.h = 0;
    }
}
