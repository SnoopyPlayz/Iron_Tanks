#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include "utils.h"
#include "map.h"
#include "player.h"

Texture2D bulletTex;
Sound colSound;

typedef struct Bullet{
	float x;
	float y;
	int rot;
	bool bounced;
	bool playerBullet;
	float velX;
	float velY;
} Bullet;
Bullet * bullets = NULL;
int bulletsAmount = 0;

void bulletInit(){
	bulletTex = LoadTexture("/res/bullet.png");
	colSound = LoadSound("/res/Audio/odbicie_comp.ogg");
}

void moveBullet(int b, int speed){
	bullets[b].x += bullets[b].velX * speed;
	bullets[b].y -= bullets[b].velY * speed;
}

int addBullet(int x, int y, int rot, bool player){
	bulletsAmount++;
	if (bullets == NULL) {
		bullets = malloc(bulletsAmount * sizeof(Bullet));
	}else{
		bullets = realloc(bullets, bulletsAmount * sizeof(Bullet));
	}

	if (bullets == NULL) {
		printf("NULL POINTER  \n");	
		bullets = realloc(bullets, bulletsAmount * sizeof(Bullet));
	}

	bullets[bulletsAmount - 1] = (Bullet){x, y, rot, 0, player, sin(toThera(rot)), cos(toThera(rot))};
	moveBullet(bulletsAmount - 1, 65);

	return bulletsAmount;
}

void removeBullet(int b){
	if (bullets[b].playerBullet)
		playerBulletsAmount--;	

	bulletsAmount--;
	bullets[b] = bullets[bulletsAmount];
	bullets = realloc(bullets, bulletsAmount * sizeof(Bullet));
}

int checkBulletColision(int x, int y, int pWidth, int pHeight, bool playerCol);
int checkAllBulletColisions(int x, int y, int pWidth, int pHeight, int bullet);

void bulletColision(Bullet * bullet, int i){
	if (checkMapColision(bullet->x - 10, bullet->y - 10, 15, 1)) {
		if (!bullet->bounced) {
			bullet->bounced = true;
			bullet->rot = -bullet->rot;
			bullet->velX = -bullet->velX;
			PlaySound(colSound);
			return;
		}
		removeBullet(i);
		return;
	}
	else if (checkMapColision(bullet->x - 10, bullet->y - 15, 15, 15)) {
		if (!bullet->bounced) {
			bullet->bounced = true;
			bullet->velY = -bullet->velY;
			bullet->rot = -bullet->rot + 180;
			PlaySound(colSound);
			return;
		}
		removeBullet(i);
		return;
	}

	int colision = checkAllBulletColisions(bullet->x - 10, bullet->y - 10, 15, 15, i);
	if (colision) {
		int bulletX = bullet->x;
		int bulletY = bullet->y;
		removeBullet(i);

		// array changed get new pos
		int newElementPos = checkAllBulletColisions(bulletX - 10, bulletY - 10, 15, 15, -1);

		if (newElementPos)
			removeBullet(newElementPos - 1);
		return;
	}
}

int checkAllBulletColisions(int x, int y, int pWidth, int pHeight, int bullet){
	const int bulletSize = 15;

	for (int i = 0; i < bulletsAmount; i++) { // AABB colision
		Bullet b = bullets[i];
		b.x -= 10;
		b.y -= 15;
		//if (playerCol && b.playerBullet)
		//	continue;
		if (i == bullet)
			continue;
		
		if(x < b.x + bulletSize && x + pWidth > b.x && y < b.y + bulletSize && y + pHeight > b.y)
			return i + 1;
	}
	return 0;
}

int checkBulletColision(int x, int y, int pWidth, int pHeight, bool playerCol){
	const int bulletSize = 15;

	for (int i = 0; i < bulletsAmount; i++) { // AABB colision
		Bullet b = bullets[i];
		b.x -= 10;
		b.y -= 15;
		//if (playerCol && b.playerBullet)
		//	continue;
		
		if(x < b.x + bulletSize && x + pWidth > b.x && y < b.y + bulletSize && y + pHeight > b.y)
			return i + 1;
	}
	return 0;
}

void bulletRender(){
	for (int i = 0; i < bulletsAmount; i++) {
		Bullet * bullet = &bullets[i];
		// Debug  DrawRectangle(bullets[i].x - 10, bullets[i].y - 10, 15, 15, BLACK);
		moveBullet(i, 6);
		bulletColision(bullet, i);
	}
	for (int i = 0; i < bulletsAmount; i++) {
		Bullet * bullet = &bullets[i];
		DrawTextureWithRot(bulletTex, bullet->x + 3, bullet->y + 3, bullet->rot, GRAY);
		DrawTextureWithRot(bulletTex, bullet->x, bullet->y, bullet->rot, WHITE);
	}
}

void bulletRestartGame(){
	bulletsAmount = 0;
	free(bullets);
	bullets = NULL;
}
