#pragma once
typedef struct Bullet{
	int x;
	int y;
	int rot;
} Bullet;

void bulletInit();
void bulletRender();
void removeBullet(int b);
int addBullet(int x, int y, int rot, bool player);
void bulletRestartGame();

int checkBulletColision(int x, int y, int pWidth, int pHeight, bool playerCol);
