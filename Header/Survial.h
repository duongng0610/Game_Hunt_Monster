#ifndef SURVIAL_H_
#define SURVIVAL_H_

#include "CommonFunc.h"
#include "BaseObj.h"

class Survial : public BaseObj
{
public:
    Survial();
    ~Survial();

    void SetNum(const int& _num) {num = _num;}
    void AddPos(const int& xPos);
    void Shown(SDL_Renderer* screen);
    void Init(SDL_Renderer* screen);

    void Decrease();
private:
    int num;
    vector<int> pos_list;
};
#endif // SURVIAL_H_
