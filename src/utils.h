#pragma once
#include "raylib.h"

void DrawTextureWithRot(Texture tex, int x, int y, int rot, Color col);
float pointsToDeg(float aX, float aY, float bX, float bY);
double toThera(double a);
//bool checkAABBCol(int aX, int aY, int aWidth, int aHeight, int bX, int bY, int bWidth, int bHeight);
void updateMouse(bool InMenu);
void winLevel();
void stopGame();
void funcJS(char * func);
void initUtils();

extern int mouseX;
extern int mouseY;
