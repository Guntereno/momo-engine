#include "TestView.h"

#include "Game.h"
#include "Graphics/Texture.h"
#include "Text/Alignment.h"
#include "Text/TextWrapper.h"
#include "Ui/ButtonView.h"
#include "Ui/ImageDrawable.h"
#include "Ui/NinePatchDrawable.h"
#include "Ui/TextBoxView.h"

TestView::TestView()
{
	using namespace Momo::Ui;

	Momo::Offset margin;
	margin.Set(15);
	SetMargin(margin);

	mpNinePatchTexture = new Momo::Graphics::Texture();

	mpNinePatch = new Momo::Ui::NinePatchDrawable();
	Momo::Rectangle outer = { 0, 7, 57, 57 };
	Momo::Rectangle inner = { 24, 20, 16, 25 };
	mpNinePatch->SetState
	(
		kStateDefault,
		mpNinePatchTexture,
		outer,
		inner
	);

	for (int i = 0; i < kTextureCount; ++i)
	{
		mpTextures[i] = new Momo::Graphics::Texture();
	}

	for (int i = 0; i < kImageCount; ++i)
	{
		mpImages[i] = new ImageDrawable();
	}

	mpImages[kImageToad]->SetState(kStateDefault, mpTextures[kTextureToad], Momo::Rectangle::Zero());
	mpImages[kImageToad]->SetState(kStatePressed, mpTextures[kTextureToadPressed], Momo::Rectangle::Zero());

	// Create a null drawable for rendering plain coloured quads
	mpNullImage = new ImageDrawable();

	mpNullImage->SetState(kStateDefault, NULL, Momo::Rectangle::Zero());

	struct ViewParams
	{
		Drawable* pImage;
		Momo::Color color;
		u16 flags;
		Momo::Rectangle area;
		Momo::Offset padding;
	};

	mpMushroomButtonView = new ButtonView();
	mpOsdView = new View();

	const int kViewCount = 2;
	Momo::Ui::View* pViews[kViewCount] = { mpMushroomButtonView, mpOsdView };
	ViewParams params[kViewCount] =
	{
		{
			mpImages[kTextureToad],
			Momo::Color::White(),
			View::kFlagTouchRight | View::kFlagTouchTop,
			{0, 0, 128, 128},
			{{0, 0, 0, 0}}
		},
		{
			mpNinePatch,
			Momo::Color::White(),
			View::kFlagTouchRight,
			{0, 0, 250, 200},
			{{10, 10, 10, 10}}
		},
	};

	for (int i = 0; i < kViewCount; ++i)
	{
		pViews[i]->SetBackground(params[i].pImage);
		pViews[i]->SetColor(params[i].color);
		pViews[i]->SetFlags(params[i].flags);
		pViews[i]->SetArea(params[i].area);
		pViews[i]->SetPadding(params[i].padding);
		AddChild(pViews[i]);
	}

	mpTextBox = new TextBoxView(128);
	mpTextBox->SetColor(Momo::Color::White());
	mpTextBox->SetText("This is some test text to test the text. Is it working?");
	mpTextBox->SetFlags(Momo::Text::kHAlignCenter | Momo::Text::kVAlignCenter);
	mpOsdView->AddChild(mpTextBox);
}

TestView::~TestView()
{
	Flush();
}

void TestView::Load()
{
	const char* kNinePatchFileName = "wwf_9patch.tga";
	mpNinePatchTexture->LoadTga(kNinePatchFileName);

	const char* kFileNames[kTextureCount] =
	{
		"toad.tga",
		"toad_pressed.tga"
	};
	for (int i = 0; i < kTextureCount; ++i)
	{
		mpTextures[i]->LoadTga(kFileNames[i]);
	}

	const Momo::Text::Font* pFont = Game::GetInstance()->GetFont(Game::kFontVerdana);
	mpTextBox->SetFont(*pFont);

	mpMushroomButtonView->RegisterCallback(reinterpret_cast<Momo::Ui::ICallback*>(Game::GetInstance()->GetMushroomCallback()));

}

void TestView::Flush()
{
	delete mpTextBox;
	mpTextBox = NULL;

	delete mpMushroomButtonView;
	mpMushroomButtonView = NULL;

	delete mpOsdView;
	mpOsdView = NULL;

	delete mpNullImage;
	mpNullImage = NULL;
}
