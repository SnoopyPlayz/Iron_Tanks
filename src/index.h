#include <raylib.h>

enum GameState{
	menu,
	dead,
	game,
	nextLevel,
	win,
	controls
};
extern enum GameState gameState;
extern Music gameMusic;
