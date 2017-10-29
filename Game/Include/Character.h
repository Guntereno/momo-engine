#ifndef GAME_CHARACTER_INCLUDED
#define GAME_CHARACTER_INCLUDED

#include "Common.h"
#include "Rectangle.h"
#include "Vector.h"

#include <cstddef>

class Character
{
public:
	enum Type
	{
		kCharacterMarioBig,
		kCharacterMarioSmall,
		kCharacterMushroom,

		kCharacterCount
	};

	enum Direction
	{
		kDirectionLeft,
		kDirectionRight
	};

	enum State
	{
		kStateAlive,
		kStateDying,
		kStateDead
	};

	static void SetTexture(Momo::Graphics::Texture& value) { mspTexture = &value; }
	static void SetBounds(const Momo::Rectangle& value) { msBounds = value; }
	static void SetScaleFactor(int value) { msScaleFactor = value; }
	static void ResetCounters();
	static int GetKillCount(Type type);

	Character();

	void Init();

	void Update(const Momo::GameTime& gameTime);

	void Draw(Momo::Graphics::SpriteBatch& spriteBatch);

	void SetType(Type value)
	{
		mType = value;
		mFrame = 0;
	}

	void SetPosition(Momo::Vector2 value) { mPosition = value; }
	void SetDirection(Direction value) { mDirection = value; }

	void OnHit();

	const Momo::Rectangle& CurrentSource();
	Momo::Rectangle CurrentBounds();
	bool IsAlive() { return mState == kStateAlive; }

	void Mushroom();

private:
	DISALLOW_COPY_AND_ASSIGN(Character);

	static Momo::Graphics::Texture* mspTexture;
	static Momo::Rectangle msBounds;
	static int msScaleFactor;

	Type mType;
	State mState;
	float mFrameTimer;
	int mFrame;
	Momo::Vector2 mPosition;
	Momo::Vector2 mVelocity;
	Direction mDirection;
};

#endif // GAME_CHARACTER_INCLUDED
