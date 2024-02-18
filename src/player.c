#include "raylib.h"
#include "camera.h"
#include "index.h"
#include "utils.h"
#include "trail.h"
#include "map.h"
#include "utils.h"
#include "bullet.h"
#include <math.h>
#include <stdio.h>

int bodyAnimation();
void renderPlayer();
void playerBulletUpdate();
void playerKeyInput(int *playerVelX, int *playerVelY);
void playerCheckColision(int *playerVelX, int *playerVelY);

Texture2D player;
Texture2D playerHead;

int playerX = 100;
int playerY = 100;
double headRot = 0;
int bodyRot = 0;
int playerCenter = 32;

void playerInit(){
	player = LoadTexture("/res/player2.png");
	playerHead = LoadTexture("/res/player.png");
}

int lastTrailX, lastTrailY;
void playerUpdate(){
	int playerVelX = 0;
	int playerVelY = 0;

	playerKeyInput(&playerVelX, &playerVelY);
	playerCheckColision(&playerVelX, &playerVelY);
	playerBulletUpdate();
	// tank trail
	addTrail(playerX, playerY, bodyRot, &lastTrailX, &lastTrailY);

	if(checkBulletColision(playerX - playerCenter, playerY - playerCenter, player.width, player.width, true))
		stopGame();
	
	renderPlayer();
}

void renderPlayer(){
	bodyAnimation();
	//body render
	DrawTextureWithRot(player, playerX + 5, playerY + 5, bodyRot, GRAY);
	DrawTextureWithRot(player, playerX, playerY, bodyRot, WHITE);

	//head pos
	float posX = playerX - camera.target.x;
	float posY = playerY - camera.target.y;
	//head rot
	
	headRot = pointsToDeg(mouseX - camera.target.x, mouseY - camera.target.y, posX, posY);

	//head render
	DrawTextureWithRot(playerHead, playerX + 4, playerY + 4, headRot, GRAY);
	DrawTextureWithRot(playerHead, playerX, playerY, headRot, WHITE);
}

void playerCheckColision(int *playerVelX, int *playerVelY){
	int posX = playerX - playerCenter + 7;
	int posY = playerY - playerCenter;

	if (!checkMapColision(posX + *playerVelX, posY, player.width - 18, player.height - 5))
		playerX += *playerVelX;
	
	if (!checkMapColision(posX, posY + *playerVelY, player.width - 18, player.height - 5))
		playerY += *playerVelY;
}

void playerKeyInput(int *playerVelX, int *playerVelY){
	const int playerSpeed = 4;
	if (IsKeyDown(KEY_A))
		*playerVelX -= playerSpeed;

	if (IsKeyDown(KEY_W))
		*playerVelY -= playerSpeed;

	if (IsKeyDown(KEY_D))
		*playerVelX += playerSpeed;

	if (IsKeyDown(KEY_S))
		*playerVelY += playerSpeed;
}

int playerBulletsAmount = 0;
void playerBulletUpdate(){
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && playerBulletsAmount < 3) {
		addBullet(playerX, playerY, headRot, true);
		playerBulletsAmount++;
	}
}

int bodyAnimation(){
	float horizontalRot = 90 * IsKeyDown(KEY_A) + -90 * IsKeyDown(KEY_D);
	float verticalRot = 0.5 * IsKeyDown(KEY_W) + -0.5 * IsKeyDown(KEY_S) + 1;

	float currentRot = horizontalRot * verticalRot;
/*	if (IsKeyDown(KEY_W)) {
		if (IsKeyDown(KEY_A)) currentRot = -45;
		if (IsKeyDown(KEY_D)) currentRot = 45;
	} else if (IsKeyDown(KEY_S)) {
		if (IsKeyDown(KEY_A)) currentRot = 45;
		if (IsKeyDown(KEY_D)) currentRot = -45;
	}else {
		if(IsKeyDown(KEY_A)) currentRot = 90;
		if(IsKeyDown(KEY_D)) currentRot = 90;
	}*/

	//animation
	if (bodyRot > currentRot) {
		bodyRot-=5;
	}else if (bodyRot < currentRot) {
		bodyRot+=5;
	}
	
	return bodyRot;
}

int checkPlayerColision(int x, int y, int width, int height){
	const int pWidth = player.width;
	const int pHeight = 64;
	
	if(x < playerX + pWidth && x + width > playerX && y < playerY + pHeight && y + height > playerY)
		return 1;

	return 0;
}

void playerRestartGame(){
	playerBulletsAmount = 0;
	playerX = 100;
	playerY = 100;
}
