#ifndef MOMO_COLOR_INCLUDED
#define MOMO_COLOR_INCLUDED

#include "Momo.h"

namespace Momo
{

	// POD type representing a Colour
	struct Color
	{
		u32 mValue;

		u8 R() const
		{
			return static_cast<u8>(mValue >> kRedShift);
		}

		u8 G() const
		{
			return static_cast<u8>(mValue >> kGreenShift);
		}

		u8 B() const
		{
			return static_cast<u8>(mValue >> kBlueShift);
		}

		
		u8 A() const
		{
			return static_cast<u8>(mValue);
		}

		void SetR(u8 value)
		{
			this->mValue = (this->mValue & ~kRedMask) | ((unsigned int)value << kRedShift);
		}

		void SetG(u8 value)
		{
			this->mValue = (this->mValue & ~kGreenMask) | ((unsigned int)value << kGreenShift);
		}

		void SetB(u8 value)
		{
			this->mValue = (this->mValue & ~kBlueMask) | ((unsigned int)value << kBlueShift);
		}

		void SetA(u8 value)
		{
			this->mValue = (this->mValue & ~kAlphaMask) | ((unsigned int)value << kAlphaShift);
		}

		void Set(const Color& color)
		{
			this->mValue = color.mValue;
		}

		void Set(u8 r, u8 g, u8 b, u8 a = 255)
		{
			this->mValue = static_cast<u32>((r << kRedShift) | (g << kGreenShift) | (b << kBlueShift) | (a << kAlphaShift));
		}

		static Color Lerp(const Color& value1, const Color& value2, float t)
		{
			if (t > 1.0f)
				return value2;
			else if (t < 0.0f)
				return value1;

			u8 r1 = value1.R();
			u8 g1 = value1.G();
			u8 b1 = value1.B();
			u8 a1 = value1.A();

			u8 r2 = value2.R();
			u8 g2 = value2.G();
			u8 b2 = value2.B();
			u8 a2 = value2.A();

			const int kFixedShift = 16;

			u32 fixedT = static_cast<u32>(t * (1 << kFixedShift));

			u32 fixedR = (r1 << kFixedShift) + ((r2 - r1) * fixedT);
			u32 fixedG = (g1 << kFixedShift) + ((g2 - g1) * fixedT);
			u32 fixedB = (b1 << kFixedShift) + ((b2 - b1) * fixedT);
			u32 fixedA = (a1 << kFixedShift) + ((a2 - a1) * fixedT);

			Color result;

			result.SetR(static_cast<u8>(fixedR >> kFixedShift));
			result.SetG(static_cast<u8>(fixedG >> kFixedShift));
			result.SetB(static_cast<u8>(fixedB >> kFixedShift));
			result.SetA(static_cast<u8>(fixedA >> kFixedShift));

			return result;
		}

		static constexpr u32 kRedShift = 0U;
		static constexpr u32 kGreenShift = 8U;
		static constexpr u32 kBlueShift = 16U;
		static constexpr u32 kAlphaShift = 24U;

		static constexpr u32 kRedMask = 0xFFU << kRedShift;
		static constexpr u32 kGreenMask = 0xFFU << kGreenShift;
		static constexpr u32 kBlueMask = 0xFFU << kBlueShift;
		static constexpr u32 kAlphaMask = 0xFFU << kAlphaShift;

		static Color White()
		{
			Color color;
			color.mValue = 0xFFFFFFFFU;
			return color;
		}

		static Color Black()
		{
			Color color;
			color.mValue = 0xFF000000U;
			return color;
		}

		static Color Red()
		{
			Color color;
			color.mValue = 0xFF0000FFU;
			return color;
		}

		static Color Green()
		{
			Color color;
			color.mValue = 0xFF00FF00U;
			return color;
		}

		static Color Blue()
		{
			Color color;
			color.mValue = 0xFFFF0000U;
			return color;
		}
	};

	POD_CHECK(Color);
}

#endif //MOMO_COLOR_INCLUDED
