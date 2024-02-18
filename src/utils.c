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

int mouseX;
int mouseY;
int lastMouseX;
int lastMouseY;
int mouseXOffset;
int mouseYOffset;
Texture2D cursor;

// Y: 306 X: 564
void updateMouse(bool InMenu){
	if (cursor.width == 0) {
		cursor = LoadTexture("/res/cursor.png");
	}

	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		DisableCursor();
	}

	// if window already focused dont change mouse pos
	
	if (GetMouseY() != 307 && GetMouseX() != 564){
		mouseX = GetMouseX() + camera.target.x + mouseXOffset;
		mouseY = GetMouseY() + camera.target.y + mouseYOffset;
		lastMouseX = GetMouseX();
		lastMouseY = GetMouseY();
	}else{
		mouseX = lastMouseX + camera.target.x + mouseXOffset;
		mouseY = lastMouseY + camera.target.y + mouseXOffset;
	}

	DrawTexture(cursor, mouseX - 20, mouseY - 20, WHITE);

	if (!InMenu){
		mouseXOffset = 0;
		mouseYOffset = 0;
		return;
	}
	
	if (mouseX < 0)
		mouseXOffset += 10;
	
	if (mouseX > 1128)
		mouseXOffset -= 100;
	
	if (mouseY > 1128)
		mouseYOffset -= 100;
	
	if(mouseY < 100)
		mouseYOffset += 10;
}
/*
bool checkAABBCol(int aX, int aY, int aWidth, int aHeight, int bX, int bY, int bWidth, int bHeight) {
    // no overlap x-axis
    if ((aX + aWidth) < bX || aX > (bX + bWidth))
        return false;

    // no overlap y-axis
    if ((aY + aHeight) < bY || aY > (bY + bHeight))
        return false;

    return true;
}
*/

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
	printf("current levlel: %d \n", currentLevel);
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
