#include "MushroomCallback.h"

#include "Character.h"

Game::MushroomCallback::MushroomCallback(Game* pGame)
{
	mpGame = pGame;
}

void Game::MushroomCallback::Call(void* pSender)
{
	for (int i = 0; i < Game::kNumCharacters; ++i)
	{
		Character& character = mpGame->mCharacters[i];
		character.Mushroom();
	}
}
