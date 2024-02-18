#include "camera.h"
#include "raylib.h"
#include "utils.h"
#include "player.h"
#include "utils.h"
#include "menu.h"
#include "trail.h"
#include "bullet.h"
#include "tanks.h"
#include "map.h"
#include <emscripten/emscripten.h>
#include <stdio.h>
#define PLATFORM_WEB

void gameUpdate();
void menuUpdate();
void gameInit();
void update();
enum GameState{
	menu,
	dead,
	game,
	nextLevel,
	win,
	controls
};
enum GameState gameState = menu;


int main() {
	funcJS("startLoadingGame()");
	InitWindow(1128, 615, "Iron Tanks");

	DisableCursor();

	gameInit();
	SetTargetFPS(60);

	menuInit();
	funcJS("stopLoadingGame()");
	funcJS("stopGamePlay()");

	emscripten_set_main_loop(update, 0, 1);

	CloseWindow();

	return 0;
}

void update(){
	BeginDrawing();
	ClearBackground(GRAY);

	switch (gameState) {
		case game:
			gameUpdate();
			break;
		case menu:
			menuUpdate();
			break;
		case dead:
			deadUpdate();
			break;
		case nextLevel:
			nextLevelUpdate();
			break;
		case win:
			winGame();
			break;
		case controls:
			controlsUpdate();
			break;
	}
	EndDrawing();
}

Music gameMusic;
void gameInit(){
	int loadingLevel = EM_ASM_INT({ return loadData();});

	if (loadingLevel) {
		currentLevel = loadingLevel;
	}

	InitAudioDevice();

	playerInit();
	mapInit();
	cameraInit();
	bulletInit();
	tankInit();
	tankTrailInit();
	initUtils();

	gameMusic = LoadMusicStream("/res/Audio/game_comp.ogg");
	PlayMusicStream(gameMusic);
}

void gameUpdate() {
	BeginMode2D(camera);
	UpdateMusicStream(gameMusic);

	mapRender();
	tankTrailUpdate();
	bulletRender();
	tankUpdate();
	playerUpdate();
	cameraMovement(&camera);
	updateMouse(false);

	//printf("fps: %d \n",GetFPS());
	EndMode2D();

	char level[80];
	sprintf(level, "level: %d", currentLevel);
	DrawText(level, 40, 180, 40, BLACK);
}
