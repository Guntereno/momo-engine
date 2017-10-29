#ifndef GAME_MUSHROOMCALLBACK_INCLUDED
#define GAME_MUSHROOMCALLBACK_INCLUDED

#include "Momo.h"
#include "Ui/ICallback.h"

#include "Game.h"


class Game::MushroomCallback : public Momo::Ui::ICallback
{
public:
	MushroomCallback() {}
	virtual ~MushroomCallback() {}

	MushroomCallback(Game* pGame);
	void Call(void* pSender);

private:
	DISALLOW_COPY_AND_ASSIGN(MushroomCallback);

	Game* mpGame;
};

#endif //GAME_MUSHROOMCALLBACK_INCLUDED
