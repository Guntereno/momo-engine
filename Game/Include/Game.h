#ifndef GAME_GAME_INCLUDED
#define GAME_GAME_INCLUDED

#include "GameBase.h"

#include "Momo.h"

#include "Character.h"
#include "Graphics/Texture.h"
#include "Point.h"
#include "Text/Font.h"
#include "TestView.h"

class Game: public Momo::GameBase
{
public:
	enum FontId
	{
		kFontVerdana,

		kFontCount
	};

	static Game* GetInstance() { return mspInstance; }

	Game();
	virtual ~Game();

	virtual void Init();
	virtual void Resize(unsigned int width, unsigned int height);
	virtual void Update();
	virtual void Draw();
	virtual void DrawDebug();
	virtual void Destroy();
	virtual void Pause();
	virtual void UnPause();
	virtual bool HandleTouchEvent(const Momo::Input::Event& event);

	const Momo::Text::Font* GetFont(FontId fontId);

	class MushroomCallback;
	MushroomCallback* GetMushroomCallback() { return mpMushroomCallback; }

private:
	static const int kNumCharacters = 512;

	static Game* mspInstance;

	MushroomCallback* mpMushroomCallback;

	void DoTests();
	void LoadGraphics();

	void RenderCharacters();
	void RenderOsd();

	Momo::Graphics::Texture mTexture;
	Character mCharacters[kNumCharacters];
	Momo::Text::Font mFonts[kFontCount];
	int mCurrentFont;

	TestView mTestView;
};

#endif // GAME_GAME_INCLUDED
