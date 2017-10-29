#ifndef MOMO_COLOR_INCLUDED
#define MOMO_COLOR_INCLUDED

#include "Momo.h"


namespace Momo
{

	// POD type representing a Colour
	struct Color
	{
		unsigned int value;

		unsigned char R() const
		{
			return value >> kRedShift;
		}

		unsigned char G() const
		{
			return value >> kGreenShift;
		}

		unsigned char B() const
		{
			return value >> kBlueShift;
		}

		unsigned char A() const
		{
			return value;
		}

		void SetR(unsigned char value)
		{
			this->value = (this->value & ~kRedMask) | ((unsigned int)value << kRedShift);
		}

		void SetG(unsigned char value)
		{
			this->value = (this->value & ~kGreenMask) | ((unsigned int)value << kGreenShift);
		}

		void SetB(unsigned char value)
		{
			this->value = (this->value & ~kBlueMask) | ((unsigned int)value << kBlueShift);
		}

		void SetA(unsigned char value)
		{
			this->value = (this->value & ~kAlphaMask) | ((unsigned int)value << kAlphaShift);
		}

		void Set(const Color& color)
		{
			this->value = color.value;
		}

		void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255)
		{
			this->value = (r << kRedShift) | (g << kGreenShift) | (b << kBlueShift) | (a << kAlphaShift);
		}

		static Color Lerp(const Color& value1, const Color& value2, float t)
		{
			if (t > 1.0f)
				return value2;
			else if (t < 0.0f)
				return value1;

			unsigned char r1 = value1.R();
			unsigned char g1 = value1.G();
			unsigned char b1 = value1.B();
			unsigned char a1 = value1.A();

			unsigned char r2 = value2.R();
			unsigned char g2 = value2.G();
			unsigned char b2 = value2.B();
			unsigned char a2 = value2.A();

			const int kFixedShift = 16;

			unsigned int fixedT = (int)(t * (1 << kFixedShift));

			int fixedR = (r1 << kFixedShift) + ((r2 - r1) * fixedT);
			int fixedG = (g1 << kFixedShift) + ((g2 - g1) * fixedT);
			int fixedB = (b1 << kFixedShift) + ((b2 - b1) * fixedT);
			int fixedA = (a1 << kFixedShift) + ((a2 - a1) * fixedT);

			Color result;

			result.SetR((unsigned char)(fixedR >> kFixedShift));
			result.SetG((unsigned char)(fixedG >> kFixedShift));
			result.SetB((unsigned char)(fixedB >> kFixedShift));
			result.SetA((unsigned char)(fixedA >> kFixedShift));

			return result;
		}

		static const int kRedShift = 0;
		static const int kGreenShift = 8;
		static const int kBlueShift = 16;
		static const int kAlphaShift = 24;

		static const int kRedMask = 0xFF << kRedShift;
		static const int kGreenMask = 0xFF << kGreenShift;
		static const int kBlueMask = 0xFF << kBlueShift;
		static const int kAlphaMask = 0xFF << kAlphaShift;

		static Color White()
		{
			Color color;
			color.value = 0xFFFFFFFF;
			return color;
		}

		static Color Black()
		{
			Color color;
			color.value = 0xFF000000;
			return color;
		}

		static Color Red()
		{
			Color color;
			color.value = 0xFF0000FF;
			return color;
		}

		static Color Green()
		{
			Color color;
			color.value = 0xFF00FF00;
			return color;
		}

		static Color Blue()
		{
			Color color;
			color.value = 0xFFFF0000;
			return color;
		}
	};

}

#endif //MOMO_COLOR_INCLUDED
