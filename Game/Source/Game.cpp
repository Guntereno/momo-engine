#include "Game.h"

#include "MushroomCallback.h"
#include "Tests/Tests.h"

#include "AssertBreak.h"
#include "Color.h"
#include "GameTime.h"
#include "Logger.h"
#include "Matrix.h"
#include "Graphics/SpriteBatch.h"
#include "Graphics/Targa.h"
#include "Graphics/Technique.h"
#include "Graphics/Utils.h"
#include "Io/File.h"
#include "Memory/MemoryTests.h"
#include "Text/C99Printf.h"
#include "Ui/View.h"

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <cstdio>
#include <cstdlib>
#include <cstddef>
#include <cstring>
#include <cmath>

using namespace Momo;

Game* Game::mspInstance = NULL;


static constexpr char* kFontNames[Game::kFontCount] =
{
    "fonts/verdana_outline_nopack.fnt",
};

Game::Game() :
    GameBase(),
    mCurrentFont(0)
{
    ASSERT(mspInstance == NULL);

    mpViewContainer->AddChild(&mTestView);

    mpMushroomCallback = new MushroomCallback(this);

    mspInstance = this;
}

Game::~Game()
{
    mspInstance = NULL;

    delete mpMushroomCallback;

    mTestView.Flush();
}

void Game::DoTests()
{
#ifdef MOMO_DEBUG
    Tests::Do();
#endif
}

void Game::LoadGraphics()
{
    mTexture.LoadTga("mario.tga");
    Character::SetTexture(mTexture);

    for (int i = 0; i < kFontCount; ++i)
    {
        mFonts[i].Load(kFontNames[i]);
    }

    mTestView.Load();
}

void Game::Init3dCamera()
{
    Matrix view;
    view.SetTranslation(0.0f, 0.0f, 0.0f);
    mCamera3d.SetView(view);
    mCamera3d.SetProjectionOrtho(mViewport, -1.0f, 1.0f);
}

void Game::LoadMesh()
{
    const size_t kNumVerts = 4;
    const size_t kNumIndices = 6;
    const float kSize = 100.0f;

    auto pVerts = new Momo::Graphics::Mesh::Vertex[kNumVerts];
    pVerts[0] =
    {
        Color::Red(),
        { -kSize, -kSize, 0.0f, 1.0f },
        { 0.0f, 1.0f }
    };
    pVerts[1] =
    {
        Color::Green(),
        { -kSize, kSize, 0.0f, 1.0f },
        { 0.0f, 0.0f }
    };
    pVerts[2] =
    {
        Color::Blue(),
        { kSize, kSize, 0.0f, 1.0f },
        { 1.0f, 0.0f }
    };
    pVerts[3] =
    {
        Color::White(),
        { kSize, -kSize, 0.0f, 1.0f },
        { 1.0f, 1.0f }
    };

    auto pIndices = new GLushort[kNumIndices];
    pIndices[0] = 0;
    pIndices[1] = 2;
    pIndices[2] = 1;
    pIndices[3] = 0;
    pIndices[4] = 3;
    pIndices[5] = 2;

    mMesh.Load(
        pVerts,
        kNumVerts,
        pIndices,
        kNumIndices
    );

    mMeshTechnique.Load("shaders/vpMesh.vp", "shaders/fpMesh.fp");

    mMeshRenderer.Load(mMeshTechnique, mMesh, &mTexture);
}

void Game::Init()
{
    GameBase::Init();

    DoTests();

    LoadGraphics();
    Init3dCamera();
    LoadMesh();
}

void Game::Resize(unsigned int width, unsigned int height)
{
    GameBase::Resize(width, height);

    static constexpr int kScaleFactor = 3;
    Character::SetBounds(mViewport);
    Character::SetScaleFactor(kScaleFactor);

    // Setup the characters
    for (int i = 0; i < kNumCharacters; ++i)
    {
        mCharacters[i].Init();
    }

    Character::ResetCounters();

    mMeshPos.mX = mViewport.mWidth * 0.5f;
    mMeshPos.mY = mViewport.mHeight * 0.5f;
    mMeshPos.mZ = 0.0f;
}

void Game::Update()
{
    GameBase::Update();

    for (int i = 0; i < kNumCharacters; ++i)
    {
        mCharacters[i].Update(mGameTime);
    }
}

void Game::Draw()
{
    //LOGI("Game::Render() begin");

    GL_CHECK(glClearColor(0.322f, 0.545f, 0.651f, 1.0f))
        GL_CHECK(glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT))

        glDisable(GL_CULL_FACE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);

    mSpriteBatch.Begin();

    RenderCharacters();
    RenderOsd();

    mpViewContainer->Draw(mSpriteBatch);

    mSpriteBatch.End(mCamera);

    RenderMesh();

    // Unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Game::DrawDebug()
{
    mLineBatch.Begin();

    mpViewContainer->DrawDebug(mLineBatch);

    mLineBatch.End(mCamera);
}

void Game::Destroy()
{
    GameBase::Destroy();
}

void Game::Pause()
{
    GameBase::Pause();
}

void Game::UnPause()
{
    GameBase::UnPause();
}

bool Game::HandleTouchEvent(const Momo::Input::Event& event)
{
    switch (event.type)
    {
        case Momo::Input::Event::Type::Down:
        case Momo::Input::Event::Type::Move:
        case Momo::Input::Event::Type::Up:
        {
            // Search backwards so front characters are hit
            for (int i = (kNumCharacters - 1); i >= 0; --i)
            {
                Character& character = mCharacters[i];
                if (character.IsAlive() && character.CurrentBounds().Contains(event.pos))
                {
                    mCharacters[i].OnHit();
                    break;
                }

                mMeshPos.mX = (float)event.pos.mX;
                mMeshPos.mY = (float)event.pos.mY;
            }
            return true;
        }
        break;

        case Momo::Input::Event::Type::None:
            // Do nothing
            break;
    }

    return false;
}

void Game::RenderCharacters()
{
    for (int i = 0; i < kNumCharacters; ++i)
    {
        mCharacters[i].Draw(mSpriteBatch);
    }
}

void Game::RenderOsd()
{
    const size_t kBufferLen = 64;
    char buffer[kBufferLen];

    Point pos = { 8, 8 };
    const Momo::Text::Font& font = mFonts[kFontVerdana];

    const Momo::Color kMarioColor = { 0xFF1800c8 };
    snprintf(buffer, kBufferLen, "Mario: %d\0", Character::GetKillCount(Character::kCharacterMarioSmall));
    mSpriteBatch.DrawString(font, buffer, strlen(buffer), pos, kMarioColor);

    pos.mY += font.GetCommon()->lineHeight;

    const Momo::Color kGoombaColor = { 0xFF3070c8 };
    snprintf(buffer, kBufferLen, "Goomba: %d\0", Character::GetKillCount(Character::kCharacterMushroom));
    mSpriteBatch.DrawString(font, buffer, strlen(buffer), pos, kGoombaColor);
}

void Game::RenderMesh()
{
    Momo::Matrix world = Momo::Matrix::Translation(mMeshPos);
    mMeshRenderer.Draw(mCamera, world);
}

const Momo::Text::Font* Game::GetFont(FontId fontId)
{
    if ((fontId >= 0) && (fontId < kFontCount))
    {
        return &mFonts[fontId];
    }

    BREAK_MSG("Invald font id %d!", fontId);
    return NULL;
}
