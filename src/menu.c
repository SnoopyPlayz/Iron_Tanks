#include <stdio.h>
#include "map.h"
#include "raylib.h"
#include "utils.h"
#include "index.h"
#include <emscripten.h>

bool button(int x, int y, int width, int height){
	if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) 
		return false;

	// no overlap x-axis
	if ((x + width) < mouseX || x > mouseX)
		return false;

	// no overlap y-axis
	if ((y + height) < mouseY || y > mouseY)
		return false;

	return true;
}

int timeSinceBanner = 0;
Texture2D tank;
Texture2D logo;

void menuInit() {
	tank = LoadTexture("/res/game_Icon.png");
	logo = LoadTexture("/res/Play_CG_Purple.png");
}

void menuUpdate() {
	ClearBackground(RAYWHITE);

	DrawText("Iron Tanks", 100, 50, 100, BLACK);

	if (currentLevel > 1) {
		DrawText("continue", 100, 150, 60, BLACK);
	}else{
		DrawText("Play", 100, 150, 60, BLACK);
	}

	if (button(100, 150, 260, 60)){
		gameState = game;
		funcJS("startGamePlay()");
	}


	DrawText("Controls", 100, 250, 60, BLACK);
	if (button(100, 250, 260, 60))
		gameState = controls;
	

	DrawTexture(tank, 500, 140, WHITE);
	DrawTexture(logo, 100, 340, WHITE);
	updateMouse(true);
}

void controlsUpdate() {
	ClearBackground(RAYWHITE);

	DrawText("Controls", 100, 50, 100, BLACK);

	DrawText("WASD to move tank", 100, 150, 40, BLACK);
	DrawText("mouse to point and kill other tanks", 100, 200, 40, BLACK);
	DrawText("kill all enemy tanks to win!", 100, 250, 40, BLACK);

	DrawText("Back to Menu", 100, 350, 60, BLACK);
	if (button(100, 350, 460, 60))
		gameState = menu;

	updateMouse(true);
}
void startGame(){
	gameState = game;
	//timeSinceBanner = 0;
	funcJS("clearAD()");
	funcJS("startGamePlay()");
	ResumeMusicStream(gameMusic);
}

void deadUpdate() {
	ClearBackground(RAYWHITE);
	DrawRectangle(60, 98, 410, 405, BLACK);
	DrawTextureEx(stone, (Vector2){80, 110}, 0, 5.9, GRAY);

	DrawText("Dead", 100, 100, 90, RED);

	char level[50];
	sprintf(level, "level: %d", currentLevel);
	DrawText(level, 100, 170, 50, RED);

	DrawText("Play Again", 100, 270, 60, WHITE);
	if (button(100, 270, 300, 60)){
		startGame();
		return;
	}
	timeSinceBanner++;

	if (timeSinceBanner < 2) {
		funcJS("displayAD()");
	}
	if (timeSinceBanner > 3600) {
		timeSinceBanner = 0;
	}

	updateMouse(true);
}

void nextLevelUpdate(){
	ClearBackground(RAYWHITE);
	DrawRectangle(60, 80, 420, 420, BLACK);
	DrawTextureEx(stone, (Vector2){80, 100}, 0, 6, GRAY);

	DrawText("YOU WIN!", 100, 100, 80, GREEN);

	char level[20];
	sprintf(level, "level: %d", currentLevel);
	DrawText(level, 100, 170, 50, GREEN);

	DrawText("NEXT LEVEL", 80, 270, 60, WHITE);
	if (button(100, 270, 300, 60)){
		startGame();
		return;
	}
	timeSinceBanner++;

	if (timeSinceBanner < 2) {
		funcJS("displayAD()");
	}
	if (timeSinceBanner > 3600) {
		timeSinceBanner = 0;
	}

	updateMouse(true);
}

void winGame(){
	ClearBackground(RAYWHITE);
	DrawRectangle(60, 180, 440, 350, BLACK);
	DrawRectangle(80, 200, 400, 300, GRAY);
	DrawTextureEx(stone, (Vector2){80, 200}, 0, 6, GRAY);

	DrawText("YOU WON THE GAME!", 100, 200, 30, GREEN);

	DrawText("PLAY AGAIN", 100, 370, 60, WHITE);
	if (button(100, 370, 300, 60)){
		currentLevel = 1;
		char storeLevel[80];
		sprintf(storeLevel, "storeData(%d)", currentLevel);
		funcJS(storeLevel);
		startGame();
		return;
	}
	timeSinceBanner++;

	if (timeSinceBanner < 2) {
		funcJS("displayAD()");
	}
	if (timeSinceBanner > 3600) {
		timeSinceBanner = 0;
	}

	updateMouse(true);
}
