#ifndef GAME_TESTSCREEN_INCLUDED
#define GAME_TESTSCREEN_INCLUDED

#include "Momo.h"

#include "Ui/View.h"


class TestView : public Momo::Ui::View
{
public:
	TestView();
	~TestView();

	void Load();
	void Flush();

private:
	Momo::Ui::View* mpOsdView;
	Momo::Ui::ButtonView* mpMushroomButtonView;
	Momo::Ui::ImageDrawable* mpNullImage;

	enum TextureId
	{
		kTextureToad,
		kTextureToadPressed,

		kTextureCount
	};
	Momo::Graphics::Texture* mpTextures[kTextureCount];


	enum ImageId
	{
		kImageToad,

		kImageCount
	};
	Momo::Ui::ImageDrawable* mpImages[kImageCount];

	Momo::Graphics::Texture* mpNinePatchTexture;
	Momo::Ui::NinePatchDrawable* mpNinePatch;
	Momo::Ui::TextBoxView* mpTextBox;
};

#endif //GAME_TESTSCREEN_INCLUDED
