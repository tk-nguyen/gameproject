#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "SDL_utils.h"

const int SIZE = 75;
const int PROJECTILE_SIZE = 25;
const int SCORE_SIZE = 100;
//Tạo kiểu "gameObject" dùng cho tàu vũ trụ, thiên thạch và đạn
class gameObject
{
private:
    int x, y, w, h;
    SDL_Texture* object;
    SDL_Rect hitbox;
public:
    gameObject()
    {
        x = 0; y = 0; w = SIZE; h = SIZE;
        hitbox.w = SIZE; hitbox.h = SIZE;
        object = nullptr;
    }
    ~gameObject() {};
    void loadObject(std::string file, SDL_Renderer* renderer) { object = loadTexture(file.c_str(), renderer); }
    void changePos(int _x, int _y)
    {
        x = _x;
        y = _y;
    }
    void changeSize(int _w, int _h)
    {
        w = _w;
        h = _h;
    }
    void changeHitbox(int _x, int _y)
    {
        hitbox.x = _x;
        hitbox.y = _y;
    }
    int returnX() { return x; }
    int returnY() { return y; }
    int returnW() { return w; }
    int returnH() { return h; }
    SDL_Texture* returnTex() { return object; }
    SDL_Rect* returnHitbox() { return &hitbox; }
};

//Tạo kiểu Projectiles dùng cho đạn
class Projectiles
{
private:
    int x, y, w, h;
    int speed;
    SDL_Texture* object;
    SDL_Rect hitbox;
public:
    Projectiles()
    {
        x = 0; y = 0; w = PROJECTILE_SIZE; h = PROJECTILE_SIZE;
        speed = 0;
        hitbox.w = PROJECTILE_SIZE; hitbox.h = PROJECTILE_SIZE;
        object = nullptr;
    }
    ~Projectiles() {};
    void loadObject(std::string file, SDL_Renderer* renderer) { object = loadTexture(file.c_str(), renderer); }
    void changePos(int _x, int _y)
    {
        x = _x;
        y = _y;
    }
    void changeSize(int _w, int _h)
    {
        w = _w;
        h = _h;
    }
    void changeHitbox(int _x, int _y)
    {
        hitbox.x = _x;
        hitbox.y = _y;
    }
    void changeSpeed(int _s) { speed = _s; }
    int returnX() { return x; }
    int returnY() { return y; }
    int returnW() { return w; }
    int returnH() { return h; }
    int returnSpeed() { return speed; }
    SDL_Texture* returnTex() { return object; }
    SDL_Rect* returnHitbox() { return &hitbox; }
};

//Tạo kiểu Score dùng cho điểm
class Score
{
private:
    int score;
    TTF_Font* font;
    SDL_Color color;
    SDL_Texture* renderedText;
public:
    Score()
    {
        score = 0;
        font = nullptr; renderedText = nullptr;
        color = { 255, 255, 255 };
    }
    void loadFont(std::string file, int size) { font = TTF_OpenFont(file.c_str(), 26); }
    void loadTexture(TTF_Font* font ,std::string text, SDL_Color textColor, SDL_Renderer* renderer) { renderedText = loadText(font, text.c_str(), textColor, renderer); }
    void changeScore(int _s) { score = _s; }
    int returnScore() { return score; }
    TTF_Font* returnFont() { return font; }
    SDL_Color returnColor() { return color; }
    SDL_Texture* returnTex() { return renderedText; }
};
#endif // GAMEOBJECT_H
