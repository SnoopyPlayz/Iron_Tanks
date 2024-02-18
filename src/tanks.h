#include "map.h"
#include <raylib.h>

void tankInit();
void tankUpdate();
void addTanksFromMap(PlacedTex *placedMap, int placedMapSize);
void tankRestartGame();
int addTank(int x, int y, int type);
extern Sound boomSound;
