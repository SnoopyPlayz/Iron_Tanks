#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "player.h"
#include "trail.h"
#include "map.h"
#include "raylib.h"
#include "utils.h"
#include "bullet.h"

typedef struct Tank{
	float x;
	float y;
	int rot;
	int lastTrackX;
	int lastTrackY;
	int lastPX; // last known player pos
	int lastPY;
	int tankType;
	bool playerSeen;
	int bulletTime;
	int explosionFrame;
} Tank;

enum Tiles{
	stoneTile,
	grassTile,
	playerTile,
	blackTankTile,
	greenTankTile,
	redTankTile,
	whiteTankTile,
	purpuleTankTile,
};

int addTank(int x, int y, int type);
void tankBotUpdate(int i);
void tankPlayerRay(Tank * t);

Tank * tanks = NULL;
int tanksAmount = 0;

#define tankTexAmount 6
Texture2D tanksBody[tankTexAmount + 1];
Texture2D tanksHead[tankTexAmount + 1];

char * tankTextures[tankTexAmount] = {"Black","Green","Red","White","Purpule"};

Sound boomSound;
void tankInit(){
	for (int i = 0; i < tankTexAmount; i++) {
		char location[50];
		sprintf(location, "/res/tanks/body%s.png", tankTextures[i]); 
		tanksBody[i] = LoadTexture(location);
		sprintf(location, "/res/tanks/head%s.png", tankTextures[i]); 
		tanksHead[i] = LoadTexture(location);
	}

	boomSound = LoadSound("/res/Audio/boom_comp.ogg");
}


int addTank(int x, int y, int type){
	tanksAmount++;
	if (tanks == NULL) {
		tanks = malloc(tanksAmount * sizeof(Tank));
	}else{
		tanks = realloc(tanks, tanksAmount * sizeof(Tank));
	}
	tanks[tanksAmount - 1] = (Tank){x, y, 0, x, y, x, y, type, false, 50};

	return tanksAmount;
}

void removeTank(int b){
	tanksAmount--;
	tanks[b] = tanks[tanksAmount];
	tanks = realloc(tanks, tanksAmount * sizeof(Tank));
}

void tankExplosion(Tank * t, int i);
void tankUpdate(){
	if (tanksAmount == 0) {
		winLevel();
	}

	for (int i = 0; i < tanksAmount; i++) {
		// player alive
		if (tanks[i].explosionFrame){
			tankExplosion(&tanks[i], i);
			continue;
		}

		tankBotUpdate(i);

		Texture2D tankBodyTex = tanksBody[tanks[i].tankType - 3];
		Texture2D tankHeadTex = tanksHead[tanks[i].tankType - 3];
		
		DrawTextureWithRot(tankBodyTex, tanks[i].x + 5, tanks[i].y + 5, tanks[i].rot, GRAY);
		DrawTextureWithRot(tankBodyTex, tanks[i].x, tanks[i].y, tanks[i].rot, WHITE);

		DrawTextureWithRot(tankHeadTex, tanks[i].x + 4, tanks[i].y + 4, tanks[i].rot, GRAY);
		DrawTextureWithRot(tankHeadTex, tanks[i].x, tanks[i].y, tanks[i].rot, WHITE);
	}
}

Texture2D explosions[6];
void tankExplosion(Tank * t, int i){
	t->explosionFrame++;
	int explosion = t->explosionFrame / 10;
	explosion++;

	if (explosion > 5){
		removeTank(i);
		return;
	}

	if (explosions[explosion].width == 0) {
		char location[40];
		sprintf(location, "/res/smoke/%d.png" , explosion);
		explosions[explosion] = LoadTexture(location);
	};

	DrawTexture(explosions[explosion], t->x - 32, t->y - 32, WHITE);
}

void tankBotUpdate(int i){
	Tank * t = &tanks[i];
	const int tankCenter = 32;
	float tankSpeed = 1;

	if (t->tankType == redTankTile)
		tankSpeed = 3;

	if (t->tankType == purpuleTankTile)
		tankSpeed = -1;

	if (t->tankType == greenTankTile)
		tankSpeed = 0.3;

	if (t->explosionFrame){
		tankExplosion(t, i);
		return;
	}
	

	tankPlayerRay(t);

	t->rot = pointsToDeg(t->lastPX, t->lastPY, t->x, t->y);

	float posLeft = t->x - tankCenter + 7;//top left
	float posTop = t->y - tankCenter;//top left
	float velX = sin(toThera(t->rot)) * tankSpeed;
	float velY = -cos(toThera(t->rot)) * tankSpeed;

	if (!checkMapColision(posLeft + velX, posTop, 53, 60)){
		t->x += velX;
	}else {
		if (t->playerSeen) {
			t->y += velY * tankSpeed;
		}
	}
	if (!checkMapColision(posLeft, posTop + velY, 53, 60)){
		t->y += velY;
	}else {
		if (t->playerSeen) {
			t->x += velX * tankSpeed;
		}
	}

	addTrail(t->x, t->y, t->rot, &t->lastTrackX, &t->lastTrackY);


	if (checkBulletColision(t->x - 30, t->y - 30, 60, 60, false)) {
		removeBullet(checkBulletColision(t->x - 30, t->y - 30, 60, 60, false) - 1);
		t->explosionFrame = 1;

		PlaySound(boomSound);
	}
}

void tankRestartGame(){
	tanksAmount = 0;
	free(tanks);
	tanks = NULL;
}

void tankPlayerRay(Tank * t){
	int bulletShootTime = 60;

	if (t->tankType == redTankTile)
		bulletShootTime = 10;

	if (t->tankType == greenTankTile)
		bulletShootTime = 100;

	const int tankViewDistance = 10;
	
	// Ray 
	for (int i = 0; i < tankViewDistance; i++) {
		float rot = pointsToDeg(playerX, playerY, t->x, t->y);
		float x = t->x + sin(toThera(rot)) * i * 60;
		float y = t->y + -cos(toThera(rot)) * i * 60;
		//DrawRectangle(x, y, 7, 7, BLACK);

		// see wall 
		if(checkMapColision(x, y, 1, 1)){
			break;
		}
		// see player
		if (checkPlayerColision(x, y, 5, 5)) {
			t->lastPX = playerX;
			t->lastPY = playerY;
			t->playerSeen = true;
			t->bulletTime++;

			if (t->bulletTime > bulletShootTime) {
				if (t->tankType == whiteTankTile) {
					addBullet(t->x, t->y, t->rot - 25, false);
					addBullet(t->x, t->y, t->rot + 25, false);
				}
				addBullet(t->x, t->y, t->rot, false);
				t->bulletTime = 0;
			}
			break;
		}
	}

	// no player last seen
	if (abs(t->lastPX - (int)t->x) < 3 && abs(t->lastPY - (int)t->y) < 3) {
		t->playerSeen = false;
		int randPosY = (rand() % 3 - 1) * 64;
		int randPosX = (rand() % 3 - 1) * 64;
		if (!checkMapColision(t->lastPX + randPosX, t->lastPY + randPosY, 64, 64)) {
			t->lastPX += randPosX;
			t->lastPY += randPosY;
		}
	}
}

