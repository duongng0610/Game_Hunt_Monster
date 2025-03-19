#ifndef BASE_OBJ_H_
#define BASE_OBJ_H_

#include "CommonFunc.h"

class BaseObj
{
public:
    BaseObj();
    ~BaseObj();

    void SetRect(const int& x, const int& y) {rect.x = x, rect.y = y;}
    SDL_Rect GetRect() const {return rect;}
    SDL_Texture* GetObj() const {return p_obj;}

    bool LoadImg(string path, SDL_Renderer* screen);
    void Render (SDL_Renderer* des, const SDL_Rect* clip = NULL);
    void Free();

protected:
    SDL_Texture* p_obj;
    SDL_Rect rect;
};
#endif // BASE_OBJ_H_
