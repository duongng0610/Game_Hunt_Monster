#include "Survial.h"

Survial::Survial()
{
    num = 0;
}

Survial::~Survial()
{

}

void Survial::AddPos(const int& xp)
{
    pos_list.push_back(xp);
}

void Survial::Init(SDL_Renderer* screen)
{
    LoadImg("img//survival.png", screen);
    num = 3;
    if(pos_list.size() > 0) {
        pos_list.clear();
    }
    AddPos(20);
    AddPos(60);
    AddPos(100);
}

void Survial::Shown(SDL_Renderer* screen)
{
    for(int i = 0; i < pos_list.size(); i++) {
        rect.x = pos_list[i];
        rect.y = 0;
        Render(screen);
    }
}

void Survial::Decrease()
{
    num--;
    pos_list.pop_back();
}
