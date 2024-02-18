#include <stdlib.h>
#include <stdio.h>
#include <raylib.h>
#include "player.h"
#include "index.h"
#include "tanks.h"
#include "utils.h"

#define u16 int16_t 
#define s16 int16_t
#define u8 int8_t

PlacedTex * map = 0;
int mapSize = 0;
int currentLevel = 1;

int doesLevelExist(int level){
	char location[80];
	sprintf(location, "/res/map/%d", level);
        FILE * mapFile = fopen(location,"r");
	
        if (!mapFile)
		return -1;

	fclose(mapFile);
	return 0;
}

int loadMap(char * filePath, PlacedTex **placedMap, int *placedMapSize){
        FILE * mapFile = fopen(filePath,"rb");
	
        if (!mapFile){
		printf("NO MAP FILE %s \n",filePath);
		return -1;
	}

        PlacedTex readTex;
	*placedMap = malloc(sizeof(PlacedTex));
	*placedMapSize = 0;

	while (fread(&readTex, sizeof(PlacedTex), 1, mapFile)) {
		(*placedMapSize)++;
		*placedMap = realloc(*placedMap, *placedMapSize * sizeof(PlacedTex));

		(*placedMap)[(*placedMapSize) - 1] = readTex;
	}
	fclose(mapFile);
	return 0;
}


void addEntitiesFromMap(PlacedTex *placedMap, int placedMapSize){
	for (int i = 0; i < placedMapSize; i++) {
		if(placedMap[i].tex >= 3){
			addTank(placedMap[i].x + 32, placedMap[i].y + 32, placedMap[i].tex);
		}
		if(placedMap[i].tex == 2){
			playerX = placedMap[i].x + 32;
			playerY = placedMap[i].y + 32;
		}
	}
}

Texture2D grass;
Texture2D stone;
Texture2D sand;
Texture2D sandStone;
void mapInit(){
	grass = LoadTexture("/res/mapTex/grass.png");
	stone = LoadTexture("/res/mapTex/stone.png");
	sand = LoadTexture("/res/mapTex/sand.png");
	sandStone = LoadTexture("/res/mapTex/sandstone.png");

	char location[80];
	sprintf(location, "/res/map/%d", currentLevel);
	loadMap(location, &map, &mapSize);
	addEntitiesFromMap(map, mapSize);
}

void mapRestartGame(){
	char location[80];
	sprintf(location, "/res/map/%d", currentLevel);
	loadMap(location, &map, &mapSize);
	addEntitiesFromMap(map, mapSize);
}

void mapRender(){
	for (int i = 0; i < mapSize; i++) {
		if (map[i].tex && currentLevel % 2){
			DrawTexture(grass, map[i].x, map[i].y, WHITE);
		}else if (map[i].tex){
			DrawTexture(sand, map[i].x, map[i].y, WHITE);
		}
	}
	for (int i = 0; i < mapSize; i++) {
		if (!map[i].tex && currentLevel % 2){
			DrawTexture(stone, map[i].x + 7, map[i].y + 7, GRAY);
		}else if (!map[i].tex){
			DrawTexture(sandStone, map[i].x + 7, map[i].y + 7, GRAY);
		}
	}
	for (int i = 0; i < mapSize; i++) {
		if (!map[i].tex && currentLevel % 2){
			DrawTexture(stone, map[i].x, map[i].y, WHITE);
		}else if (!map[i].tex){
			DrawTexture(sandStone, map[i].x, map[i].y, WHITE);
		}
	}
}

int checkMapColision(int x, int y, int width, int height){
	const int mapWidth = 64;
	const int mapHeight = 64;

	for (int i = 0; i < mapSize; i++) { // AABB colision
		if (map[i].tex)
			continue;

		if(x < map[i].x + mapWidth && x + width > map[i].x && y < map[i].y + mapHeight && y + height > map[i].y)
			return 1;
	}
	return 0;
}
