#include "player.h"
#include "utils.h"
#include <math.h>
#include <stdio.h>
#include <raylib.h>
#include <stdlib.h>

Camera2D camera = (Camera2D){0};
const int cameraX = -550;
const int cameraY = -350;

void cameraInit(){
	//camera.target = (Vector2){3000, 3000};
	camera.target = (Vector2){cameraX + playerX, cameraY + playerY};
	camera.offset = (Vector2){.0f, .0f};
	camera.zoom = 0.85f;
}

void cameraMovement(Camera2D * camera){
	const float cameraSpeed = 3.5;
	//camera->zoom -= GetMouseWheelMove() * 0.05;
	
	// stutter fix
	if (abs((int)camera->target.x - playerX - cameraX) < 4 && abs((int)camera->target.y - playerY - cameraY) < 4)
		return;
	

	float rot = pointsToDeg(camera->target.x, camera->target.y, playerX + cameraX, playerY + cameraY);
	camera->target.x -= sin(toThera(rot)) * cameraSpeed;
	camera->target.y += cos(toThera(rot)) * cameraSpeed;
	
	/*
	if (camera->target.x + 3 < cameraX + playerX) {
		camera->target.x += cameraSpeed;
	}else if (camera->target.x - 3 > cameraX + playerX) {
		camera->target.x -= cameraSpeed;
	}

	if (camera->target.y + 3 < cameraY + playerY) {
		camera->target.y += cameraSpeed;
	}else if (camera->target.y - 3 > cameraY + playerY) {
		camera->target.y -= cameraSpeed;
	}*/
}

void cameraRestartGame(){
	cameraInit();
}
