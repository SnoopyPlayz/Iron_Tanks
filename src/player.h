#pragma once

extern int playerX;
extern int playerY;
extern int playerBulletsAmount;
void playerInit();
void playerUpdate();
int checkPlayerColision(int x, int y, int width, int height);
void playerRestartGame();
