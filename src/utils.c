#include "map.h"
#include "raylib.h"
#include "math.h"
#include "bullet.h"
#include "tanks.h"
#include "trail.h"
#include "camera.h"
#include "player.h"
#include "index.h"
#include <stdio.h>
#include <stdlib.h>
#include <emscripten.h>

void DrawTextureWithRot(Texture tex, int x, int y, int rot, Color col){
	int texX = tex.width;
	int texY = tex.height;

	struct Rectangle posAndSize= (Rectangle){x, y, texX, texY};
	struct Rectangle texSize = (Rectangle){0, 0, texX, texY};

	DrawTexturePro(tex, texSize, posAndSize, (Vector2){(float)texX / 2, (float)texY / 2}, rot, col);
}

float pointsToDeg(float aX, float aY, float bX, float bY){
	double dx = aX - bX;
	double dy = aY - bY;

	double angleRad = atan2(dy, dx);
	// deg
	return angleRad * (180.0 / M_PI) + 90;
}

double toThera(double a){
	return a * M_PI / 180.;
}

// INIT INT TO 0
int mouseX = 0;
int mouseY = 0;
int lastMouseX = 0;
int lastMouseY = 0;
int mouseXOffset = 0;
int mouseYOffset = 0;
Texture2D cursor;

// Y: 306 X: 564
void updateMouse(bool InMenu){
	if (cursor.width == 0) {
		cursor = LoadTexture("/res/cursor.png");
	}

	mouseX = GetMouseX();
	mouseY = GetMouseY();

	//DrawTexture(cursor, mouseX, mouseY, WHITE);
}

EMSCRIPTEN_KEEPALIVE
void funcJS(char * func){
	emscripten_run_script(func);
}
void clearGameData(){
	playerRestartGame();
	trailRestartGame();
	tankRestartGame();
	bulletRestartGame();
	mapRestartGame();
	cameraRestartGame();
}

Sound winSound;
void stopGame(){
	PauseMusicStream(gameMusic);
	
	PlaySound(boomSound);

	gameState = dead;
	funcJS("stopGamePlay()");
	clearGameData();
}


void winLevel(){
	currentLevel++;
	char storeLevel[80];
	sprintf(storeLevel, "storeData(%d)", currentLevel);
	funcJS(storeLevel);
	
	PlaySound(winSound);

	if (doesLevelExist(currentLevel)) {
		gameState = win;
		funcJS("happyTime()");
		currentLevel = 1;
	}else{
		gameState = nextLevel;
	}
	funcJS("stopGamePlay()");
	clearGameData();
}

void initUtils(){
	winSound = LoadSound("/res/Audio/levelnext_comp.ogg");
	boomSound = LoadSound("/res/Audio/boom_comp.ogg");
}
