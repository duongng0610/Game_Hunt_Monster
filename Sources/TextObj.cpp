#include "TextObj.h"

TextObj::TextObj()
{
    text_color.r = 255;
    text_color.g = 255;
    text_color.b = 255;

    texture = NULL;
}

TextObj::~TextObj()
{

}

bool TextObj::LoadFromRenderText(TTF_Font* font, SDL_Renderer* screen)
{
    SDL_Surface* text_surface = TTF_RenderText_Solid(font, str_val.c_str(), text_color);
    if(text_surface)
    {
        texture = SDL_CreateTextureFromSurface(screen, text_surface);
        width = text_surface ->w;
        height = text_surface ->h;

        SDL_FreeSurface(text_surface);
    }
    return (texture != NULL);
}

void TextObj::Free()
{
    if(texture != NULL) {
        SDL_DestroyTexture(texture);
        texture = NULL;
    }
}

void TextObj::SetColor(Uint8 red, Uint8 green, Uint8 blue)
{
    text_color.r = red;
    text_color.g = green;
    text_color.b = blue;
}

void TextObj::SetColor(int type)
{
    if(type == RED) {
        SDL_Color color = {255, 0, 0};
        text_color = color;

    }else if(type = WHITE) {
        SDL_Color color = {255, 255, 255};
        text_color = color;

    }else if(type == BLACK) {
        SDL_Color color = {0, 0, 0};
        text_color = color;
    }
}

void TextObj::RenderText(SDL_Renderer* screen, int xp, int yp, SDL_Rect* clip,
                         double angle, SDL_Point* center, SDL_RendererFlip flip) {
      SDL_Rect renderQuad = {xp, yp, width, height};
      if(clip != NULL) {
         renderQuad.w = clip ->w;
         renderQuad.h = clip ->h;
      }

      SDL_RenderCopyEx(screen, texture, clip, &renderQuad, angle, center, flip);
}
