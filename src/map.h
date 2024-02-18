#include <stdlib.h>
#include "raylib.h"
#include <stdio.h>
#pragma once

#define u16 int16_t 
#define s16 int16_t
#define u8 int8_t

typedef struct PlacedTex{
	u16 x;
	u16 y;
	u8 tex;
	s16 rot;
} PlacedTex;

void mapInit();
void mapRender();
int checkMapColision(int x, int y, int width, int height);
int doesLevelExist(int level);
void mapRestartGame();
extern int currentLevel;
extern Texture2D stone;
