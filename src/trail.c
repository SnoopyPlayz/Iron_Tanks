#include <stdio.h>
#include <stdlib.h>
#include "player.h"
#include "raylib.h"
#include "utils.h"

int addTrail(int x, int y, int rot, int *lastX, int *lastY);
void removeTrail(int b);

typedef struct Trail{
	float x;
	float y;
	int rot;
} Trail;
Trail * trail = NULL;
int trailAmount = 0;
int trailRemovedAmount = 0;
Texture2D trailTex;

void tankTrailInit(){
	trailTex = LoadTexture("/res/tanks/trail.png");
}

int trailAmountMax = 40;
void tankTrailUpdate(){
	if (GetFPS() < 59 && trailAmountMax > 20) {
		trailAmountMax-=10;
	}else if(trailAmountMax < 1000) {
		trailAmountMax++;
	}

	if (trailAmount > trailAmountMax) {
		removeTrail(trailRemovedAmount);
		trailRemovedAmount++;
		if (trailRemovedAmount > trailAmountMax - 1) {
			trailRemovedAmount = 0;
		}
	}

	for (int i = 0; i < trailAmount; i++) {
		DrawTextureWithRot(trailTex, trail[i].x, trail[i].y, trail[i].rot, WHITE);
	}
}

int addTrail(int x, int y, int rot, int *lastX, int *lastY){
	if (abs(*lastX - x) > 10 || abs(*lastY - y) > 10) {
		*lastX = x;
		*lastY = y;
	}else {
		return 0;
	}

	trailAmount++;

	if (trail == NULL) {
		trail = malloc(trailAmount * sizeof(Trail));
	}else{
		trail = realloc(trail, trailAmount * sizeof(Trail));
	}
	if (trail == NULL) {
		printf("trail malloc or realloc error \n");	
		return 0;
	}
	trail[trailAmount - 1] = (Trail){x, y, rot};

	return trailAmount;
}

void removeTrail(int b){
	trailAmount--;
	trail[b] = trail[trailAmount];
	trail = realloc(trail, trailAmount * sizeof(Trail));
}

void trailRestartGame(){
	trailAmount = 0;
	free(trail);
	trail = NULL;
}
