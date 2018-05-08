#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "gameObject.h"
#include <vector>
#include <cstdlib>
#include <sstream>
#include <fstream>
#include <ios>
#include <map>
#include <algorithm>

bool collisionDetector(SDL_Rect* asteroids, SDL_Rect* ship);
std::vector<SDL_Rect> splitTexture();
Score loadScore(SDL_Renderer* renderer, int currentScore);
gameObject loadShip(SDL_Renderer* renderer);
Projectiles loadProjectile(gameObject &ship, SDL_Renderer* renderer);
SDL_Texture* loadBackground(SDL_Renderer* renderer);
std::vector<gameObject> loadAsteroids(SDL_Renderer* renderer, int num);
SDL_Texture* gameScreen(SDL_Renderer* renderer, std::string text, SDL_Color color);
void writeToLeaderboard(std::string name, int points);
std::vector<SDL_Texture*> getLeaders(SDL_Renderer* renderer);

#endif //GAMELOGIC_H
