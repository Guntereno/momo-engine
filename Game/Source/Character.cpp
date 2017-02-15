#include "Character.h"

#include "Graphics/SpriteBatch.h"
#include "Graphics/Texture.h"

#include "AssertBreak.h"
#include "GameTime.h"
#include "Logger.h"

#include <cfloat>
#include <cstdlib>
#include <cmath>

using namespace Momo;

const float kFrameTime = 1.0f / 10.0f; // 10 fps

static const Rectangle kFrames[] =
{
	{0, 0, 16, 32},
	{16, 0, 16, 32},
	{32, 0, 16, 32},
	{48, 0, 16, 32},
	{0, 32, 16, 16},
	{16, 32, 16, 16},
	{32, 32, 16, 16},
	{48, 32, 16, 16},
	{0, 48, 16, 16},
	{16, 48, 16, 16},
	{32, 48, 16, 16},
	{48, 48, 16, 16},
};

static const int kFrameCount[Character::kCharacterCount] = { 3, 3, 2 };
static const int kAnimFrames[Character::kCharacterCount][3] =
{
	{ 0, 1, 2 },
	{ 4, 5, 6 },
	{ 8, 9, 10 }
};
static const int kDeadFrames[Character::kCharacterCount] = { 4, 11, 10 };

static const float kSpeeds[Character::kCharacterCount] = { 50.0f, 25.0f, 20.0f };

static int gKillCounts[Character::kCharacterCount] = { 0, };

Momo::Graphics::Texture* Character::mspTexture = NULL;
Momo::Rectangle Character::msBounds;
int Character::msScaleFactor = 1;

void Character::ResetCounters()
{
	for(int i=0; i<Character::kCharacterCount; ++i)
	{
		gKillCounts[i] = 0;
	}
}

int Character::GetKillCount(Type type)
{
	return gKillCounts[type];
}

Character::Character():
	mType(kCharacterMarioBig),
	mFrameTimer(0.0f),
	mFrame(0),
	mDirection(kDirectionRight)
{
	mPosition.Set(0.0f, 0.0f);
	mVelocity.Set(0.0f, 0.0f);
}

void Character::Init()
{
	mState = kStateAlive;

	static const int kRowHeight = 32 * msScaleFactor;
	const int kRowCount = msBounds.height / kRowHeight;

	SetType((Character::Type)(rand() % Character::kCharacterCount));
	Vector2 position;
	position.Set
	(
		(float)(rand() % (msBounds.width - 16)),
		(float)((rand() % kRowCount) * kRowHeight)
	);
	SetPosition(position);
	SetDirection((Character::Direction)(rand() % 2));

	float ratio = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	mFrameTimer = ratio * kFrameTime;

	mFrame = rand() % kFrameCount[mType];
}

void Character::Update(const GameTime& gameTime)
{
	switch(mState)
	{
	case kStateAlive:
		{
			float frameDelta = gameTime.GetDeltaSeconds();
			mFrameTimer += frameDelta;
			while(mFrameTimer > kFrameTime)
			{
				if(++mFrame >= kFrameCount[mType])
					mFrame = 0;

				mFrameTimer -= kFrameTime;
			}

			float distance = kSpeeds[mType] * frameDelta;
			switch(mDirection)
			{
			case kDirectionRight:
				{
					mPosition.x += distance;
					Rectangle bounds = CurrentBounds();
					if(bounds.Right() > msBounds.Right())
					{
						mDirection = kDirectionLeft;
					}
				}
				break;

			case kDirectionLeft:
				{
					mPosition.x -= distance;
					Rectangle bounds = CurrentBounds();
					if(bounds.Left() < msBounds.Left())
					{
						mDirection = kDirectionRight;
					}
				}
				break;

			default:
				ASSERT(false);
			}
		}
		break;

	case kStateDying:
		{
			float frameDelta = gameTime.GetDeltaSeconds();
			mFrameTimer += frameDelta;

			// Small mario falls
			if(mType == kCharacterMarioSmall)
			{
				static const Vector2 kGravity = { 0.0f, -200.0f };
				mVelocity += kGravity * frameDelta;
				mPosition += mVelocity * frameDelta;
			}

			static const float kDeathLength = 2.0f;
			if(mFrameTimer > kDeathLength)
			{
				if(mType == kCharacterMarioBig)
				{
					mType = kCharacterMarioSmall;
					mState = kStateAlive;
				}
				else
				{
					mState = kStateDead;
				}
			}
		}
		break;

	case kStateDead:
		// Do nothing
		break;
	}
}

void Character::Draw(Graphics::SpriteBatch& spriteBatch)
{
	if(mState == kStateDead)
		return;

	ASSERT(mspTexture != NULL);

	unsigned int flags = (mDirection == kDirectionLeft) ? Graphics::SpriteBatch::kFlagFlipX : 0;

	Rectangle source, dest;

	switch(mState)
	{
		case kStateAlive:
		{
			source = CurrentSource();
			dest = CurrentBounds();
		}
		break;

		case kStateDying:
		{
			int deadFrame = kDeadFrames[mType];
			if (mType == kCharacterMarioBig)
			{
				// Big mario flashes between big and small
				static const float kFlashTime = 0.2f;
				if (fmod(mFrameTimer, kFlashTime) < (0.5f * kFlashTime))
				{
					source = kFrames[deadFrame];
					dest.Set(
						(int)mPosition.x,
						(int)mPosition.y,
						source.width * msScaleFactor,
						source.height * msScaleFactor);
				}
				else
				{
					source = CurrentSource();
					dest = CurrentBounds();
				}
			}
			else
			{
				source = kFrames[deadFrame];
				dest = CurrentBounds();
			}
		}
		break;

		case kStateDead:
			// Do nothing
			break;
	}
	spriteBatch.Draw(mspTexture, dest, source, Color::White(), flags);
}

void Character::OnHit()
{
	if(mState == kStateAlive)
	{
		++gKillCounts[mType];

		mState = kStateDying;
		mFrameTimer = 0.0f;

		// Small mario is launched in the air
		if(mType == kCharacterMarioSmall)
		{
			static const Vector2 kInitialVelocity = { 0.0f, 100.0f };
			mVelocity = kInitialVelocity;
		}
	}
}

const Rectangle& Character::CurrentSource()
{
	return kFrames[kAnimFrames[mType][mFrame]];
}

Rectangle Character::CurrentBounds()
{
	const Rectangle& source = CurrentSource();
	Rectangle destination;
	destination.Set(
		(int)mPosition.x,
		(int)mPosition.y,
		source.width * msScaleFactor,
		source.height * msScaleFactor);

	return destination;
}

void Character::Mushroom()
{
	if(mType == kCharacterMarioSmall)
	{
		mType = kCharacterMarioBig;
	}
}