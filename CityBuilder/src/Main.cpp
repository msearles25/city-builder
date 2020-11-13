#include "Engine/Game.h"
#include "States/GameStateStart.h"

int main()
{
	Game game;

	game.pushState(new GameStateStart(&game));
	game.gameLoop();

	return 0;
}