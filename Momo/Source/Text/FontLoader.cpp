#include "Text\FontLoader.h"

#include "Io\File.h"
#include "Logger.h"
#include "Graphics\SpriteBatch.h"
#include "Graphics\Texture.h"

#include <cstring>
#include <cstdio>


namespace Momo
{
	namespace Text
	{

		using namespace Io;

		FontLoader::FontLoader() :
			mpFileName(NULL),
			mpFont(NULL),
			mBufferUsed(0),
			mFile(NULL),
			mCurrentGlyphCount(0),
			mCurrentKerningCount(0)
		{
		}

		FontLoader::~FontLoader()
		{
		}

		bool FontLoader::Load(const char* pFileName, Font* pFont)
		{
			using namespace Io;

			ASSERT(pFileName != NULL);
			mpFileName = pFileName;

			ASSERT(pFont != NULL);
			mpFont = pFont;

			mCurrentGlyphCount = 0;
			mCurrentKerningCount = 0;

			// Open the file
			mFile = File::Open(mpFileName, Io::File::Mode::Read);
			if (mFile == NULL)
			{
				BREAK_MSG("Unable to load file %s!", mpFileName);
				return false;
			}

			bool shouldContinue;
			do
			{
				shouldContinue = ReadLine();
			} while (shouldContinue);

			File::Close(mFile);

			// Establish which technique the sprite batch should render with
			{
				using namespace Graphics;

				SpriteBatch::TechniqueId id = SpriteBatch::TechniqueId::Invalid;
				if (mpFont->GetInfo()->outlineThickness > 0)
				{
					id = SpriteBatch::TechniqueId::FontOutline;
				}
				else
				{
					id = SpriteBatch::TechniqueId::FontNoOutline;
				}

				mpFont->mTechniqueId = id;
			}

			return LoadTextures();
		}

		void FontLoader::ClearBuffer()
		{
			memset(mpBuffer, 0, sizeof(mpBuffer));
			mBufferUsed = 0;
		}

		bool FontLoader::ParseString(const char* pSource, const char* pName, char* pOutput, size_t outputLen)
		{
			UNUSED(pName); // Unused in Release

			ASSERT_MSG((strlen(pSource) < outputLen),
				"Output buffer not long enough to receive string '%s'!", pName);
			strncpy(pOutput, pSource, outputLen);
			return true;
		}

		bool FontLoader::ParseNumber(const char* pSource, const char* pName, int* pOutput)
		{
			UNUSED(pName); // Unused in Release

			int processed = sscanf(pSource, "%d", pOutput);
			if (processed != 1)
			{
				BREAK_MSG("Invalid number %s parameter '%s' in file %s!", pName, pSource, mpFileName);
				return false;
			}

			return true;
		}

		bool FontLoader::ParseBoolean(const char* pSource, const char* pName, bool* pOutput)
		{
			UNUSED(pName); // Unused in Release

			int value = 0;
			int processed = sscanf(pSource, "%d", &value);
			if (processed != 1)
			{
				BREAK_MSG("Invalid bool %s parameter '%s' in file %s!", pName, pSource, mpFileName);
				return false;
			}
			*pOutput = (value != 0);
			return true;
		}

		bool FontLoader::ParseVector(const char* pSource, const char* pName, u16* pOutput, size_t outputLen)
		{
			UNUSED(pName); // Not used in Release

			const char* kSeparators = ",";

			const size_t kVectorBufferLen = 64;
			char buffer[kVectorBufferLen];
			strncpy(buffer, pSource, kVectorBufferLen);

			char* pCurrent = strtok(buffer, kSeparators);

			int value;

			for (unsigned i = 0; i < outputLen; ++i)
			{
				if (pCurrent == NULL)
				{
					BREAK_MSG("Invalid vetor %s parameter '%s' in file %s!", pName, pSource, mpFileName);
					return false;
				}

				int processed = sscanf(pCurrent, "%d", &value);
				if (processed != 1)
				{
					BREAK_MSG("Invalid vetor %s parameter '%s' in file %s!", pName, pSource, mpFileName);
					return false;
				}

				pOutput[i] = (u16)value;

				pCurrent = strtok(NULL, kSeparators);
			}

			return true;
		}

		bool FontLoader::ReadLine()
		{
			ClearBuffer();

			// Read the tag from the start
			char currentChar;
			do
			{
				size_t read = File::Read(mFile, &currentChar, 1);

				if ((read == 0) || (currentChar == '\n'))
				{
					if (mBufferUsed > 0)
					{
						BREAK_MSG("End of line found within kvp in font %s!", mpFileName);
					}
					return false;
				}

				if (!AddCharToBuffer(currentChar))
				{
					return false;
				}
			} while (currentChar != ' ');
			mpBuffer[mBufferUsed - 1] = '\0';

			bool result = false;
			if (strncmp(mpBuffer, "info", mBufferUsed) == 0)
			{
				result = ReadInfo();
			}
			else if (strncmp(mpBuffer, "common", mBufferUsed) == 0)
			{
				result = ReadCommon();
			}
			else if (strncmp(mpBuffer, "page", mBufferUsed) == 0)
			{
				result = ReadPage();
			}
			else if (strncmp(mpBuffer, "chars", mBufferUsed) == 0)
			{
				u32 count;
				result = ReadCount(count);
				if (result)
				{
					mpFont->mGlyphCount = count;
					mpFont->mpGlyphs = new Font::Glyph[count];
				}
			}
			else if (strncmp(mpBuffer, "char", mBufferUsed) == 0)
			{
				result = ReadChar();
			}
			else if (strncmp(mpBuffer, "kernings", mBufferUsed) == 0)
			{
				u32 count;
				result = ReadCount(count);
				if (result)
				{
					mpFont->mKerningCount = count;
					mpFont->mpKernings = new Font::Kerning[count];
				}
			}
			else if (strncmp(mpBuffer, "kerning", mBufferUsed) == 0)
			{
				result = ReadKerning();
			}
			else
			{
				BREAK_MSG("Invalid marker %s found in font %s!", mpBuffer, mpFileName);
				return false;
			}

			if (!result)
			{
				return false;
			}

			return true;
		}

		bool FontLoader::AddCharToBuffer(char character)
		{
			if (mBufferUsed < kBufferLen)
			{
				mpBuffer[mBufferUsed++] = character;
			}
			else
			{
				BREAK_MSG("Exceeded work buffer whilst loading font %s!", mpFileName);
				return false;
			}

			return true;
		}

		bool FontLoader::ReadKvp(char*& key, char*& value)
		{
			key = value = NULL;
			ClearBuffer();

			// Ignore carriage return characters
			const char kCarriageReturn = 13;

			// Read the key
			char currentChar;
			do
			{
				size_t read = File::Read(mFile, &currentChar, 1);

				if ((read == 0) || (currentChar == '\n'))
				{
					if (mBufferUsed > 0)
					{
						BREAK_MSG("End of line found within key in font %s!", mpFileName);
					}
					return false;
				}

				if ((currentChar == ' ') || (currentChar == kCarriageReturn))
				{
					// skip
				}
				else if (!AddCharToBuffer(currentChar))
				{
					return false;
				}
			} while (currentChar != '=');
			mpBuffer[mBufferUsed - 1] = '\0';
			key = mpBuffer;
			size_t read;

			// Read the value
			bool quoted = false;
			value = mpBuffer + mBufferUsed;
			do
			{
				read = File::Read(mFile, &currentChar, 1);

				if (read > 0)
				{
					if (currentChar == '"')
					{
						quoted = !quoted;
					}
					else if (((currentChar == ' ') && (!quoted)) || (currentChar == '\n') || (currentChar == kCarriageReturn))
					{
						// skip
					}
					else if (!AddCharToBuffer(currentChar))
					{
						return false;
					}
				}
			} while ((read > 0) && (currentChar != '\n') && !(!quoted && (currentChar == ' ')));

			AddCharToBuffer('\0');

			return ((read != 0) && (currentChar != '\n'));
		}

		bool FontLoader::ReadInfo()
		{
			bool result;
			int value;
			bool boolValue;

			Font::Info& info = mpFont->mInfo;
			info.flags = 0;

			bool shouldContinue;
			do
			{
				char* pKey = NULL;
				char* pValue = NULL;
				shouldContinue = ReadKvp(pKey, pValue);

				if ((pKey != NULL) && (pValue != NULL))
				{
					if (strcmp(pKey, "face") == 0)
					{
						result = ParseString(pValue, pKey, info.pName, Font::Info::kStrLen);
						if (!result)
							return false;
					}
					else if (strcmp(pKey, "size") == 0)
					{
						result = ParseNumber(pValue, pKey, &value);
						if (!result)
							return false;
						info.size = (u16)value;
					}
					else if (strcmp(pKey, "bold") == 0)
					{
						result = ParseBoolean(pValue, pKey, &boolValue);
						if (!result)
							return false;

						if (boolValue)
							info.flags |= info.kFlagBold;
					}
					else if (strcmp(pKey, "italic") == 0)
					{
						result = ParseBoolean(pValue, pKey, &boolValue);
						if (!result)
							return false;

						if (boolValue)
							info.flags |= info.kFlagItalic;
					}
					else if (strcmp(pKey, "charset") == 0)
					{
						result = ParseString(pValue, pKey, info.pCharset, Font::Info::kStrLen);
						if (!result)
							return false;
					}
					else if (strcmp(pKey, "unicode") == 0)
					{
						result = ParseBoolean(pValue, pKey, &boolValue);
						if (!result)
							return false;

						if (boolValue)
							info.flags |= info.kFlagUnicode;
					}
					else if (strcmp(pKey, "stretchH") == 0)
					{
						result = ParseNumber(pValue, pKey, &value);
						if (!result)
							return false;
						info.stretchH = (u16)value;
					}
					else if (strcmp(pKey, "smooth") == 0)
					{
						result = ParseBoolean(pValue, pKey, &boolValue);
						if (!result)
							return false;

						if (boolValue)
							info.flags |= info.kFlagUnicode;
					}
					else if (strcmp(pKey, "aa") == 0)
					{
						result = ParseNumber(pValue, pKey, &value);
						if (!result)
							return false;
						info.antiAliasing = (u16)value;
					}
					else if (strcmp(pKey, "padding") == 0)
					{
						result = ParseVector(pValue, pKey, info.padding, info.kPaddingCount);
						if (!result)
							return false;
					}
					else if (strcmp(pKey, "spacing") == 0)
					{
						result = ParseVector(pValue, pKey, info.spacing, info.kSpacingCount);
						if (!result)
							return false;
					}
					else if (strcmp(pKey, "outline") == 0)
					{
						result = ParseNumber(pValue, pKey, &value);
						if (!result)
							return false;
						info.outlineThickness = (u16)value;
					}
					else
					{
						BREAK_MSG("Invalid key %s found in font file %s!", pKey, mpFileName);
						return false;
					}
				}
			} while (shouldContinue);

			return true;
		}

		bool FontLoader::ReadCommon()
		{
			Font::Common& common = mpFont->mCommon;

			bool shouldContinue;
			do
			{
				char* pKey = NULL;
				char* pValue = NULL;
				shouldContinue = ReadKvp(pKey, pValue);
				bool result;
				int value;

				if ((pKey != NULL) && (pValue != NULL))
				{
					if (strcmp(pKey, "lineHeight") == 0)
					{
						result = ParseNumber(pValue, pKey, &value);
						if (!result)
							return false;
						common.lineHeight = (u16)value;
					}
					else if (strcmp(pKey, "base") == 0)
					{
						result = ParseNumber(pValue, pKey, &value);
						if (!result)
							return false;
						common.base = (s16)value;
					}
					else if (strcmp(pKey, "scaleW") == 0)
					{
						result = ParseNumber(pValue, pKey, &value);
						if (!result)
							return false;
						common.scaleW = (u16)value;
					}
					else if (strcmp(pKey, "scaleH") == 0)
					{
						result = ParseNumber(pValue, pKey, &value);
						if (!result)
							return false;
						common.scaleH = (u16)value;
					}
					else if (strcmp(pKey, "pages") == 0)
					{
						result = ParseNumber(pValue, pKey, &value);
						if (!result)
							return false;
						common.pages = (u16)value;
					}
					else if (strcmp(pKey, "packed") == 0)
					{
						result = ParseBoolean(pValue, pKey, &common.packed);
						if (!result)
							return false;
					}
					else if (strcmp(pKey, "alphaChnl") == 0)
					{
						result = ParseNumber(pValue, pKey, &value);
						if (!result)
							return false;
						common.channelContents[Font::Common::kChannelAlpha] = (u8)value;
					}
					else if (strcmp(pKey, "redChnl") == 0)
					{
						result = ParseNumber(pValue, pKey, &value);
						if (!result)
							return false;
						common.channelContents[Font::Common::kChannelRed] = (u8)value;
					}
					else if (strcmp(pKey, "greenChnl") == 0)
					{
						result = ParseNumber(pValue, pKey, &value);
						if (!result)
							return false;
						common.channelContents[Font::Common::kChannelGreen] = (u8)value;
					}
					else if (strcmp(pKey, "blueChnl") == 0)
					{
						result = ParseNumber(pValue, pKey, &value);
						if (!result)
							return false;
						common.channelContents[Font::Common::kChannelBlue] = (u8)value;
					}
					else
					{
						BREAK_MSG("Invalid key %s found in font file %s!", pKey, mpFileName);
						return false;
					}
				}
			} while (shouldContinue);

			// Allocate the array of pages
			ASSERT(common.pages > 0);
			mpFont->mPageCount = common.pages;
			mpFont->mpPages = new Text::Font::Page[common.pages];

			return true;
		}

		bool FontLoader::ReadCount(u32& count)
		{
			int value = -1;

			bool shouldContinue;
			do
			{
				char* pKey = NULL;
				char* pValue = NULL;
				shouldContinue = ReadKvp(pKey, pValue);
				bool result;

				if ((pKey != NULL) && (pValue != NULL))
				{
					if (strcmp(pKey, "count") == 0)
					{
						result = ParseNumber(pValue, pKey, &value);
						if (!result)
							return false;
					}
				}
			} while (shouldContinue);

			if (value < 0)
				return false;

			count = (u32)value;

			return true;
		}

		bool FontLoader::ReadPage()
		{
			int id = -1;
			char fileName[kFileNameMax];
			memset(fileName, 0, sizeof(fileName));

			bool shouldContinue;
			do
			{
				char* pKey = NULL;
				char* pValue = NULL;
				shouldContinue = ReadKvp(pKey, pValue);
				bool result;

				if ((pKey != NULL) && (pValue != NULL))
				{
					if (strcmp(pKey, "id") == 0)
					{
						result = ParseNumber(pValue, pKey, &id);
						if (!result)
							return false;
					}
					else if (strcmp(pKey, "file") == 0)
					{
						result = ParseString(pValue, pKey, fileName, kFileNameMax);
						if (!result)
							return false;
					}
					else
					{
						BREAK_MSG("Invalid key %s found in font file %s!", pKey, mpFileName);
						return false;
					}
				}
			} while (shouldContinue);

			bool valid = ((id >= 0) && (id < (int)mpFont->mPageCount) && (fileName[0] != '\0'));
			if (valid)
			{
				Font::Page& page = mpFont->mpPages[id];
				page.id = (u16)id;

				strncpy(mpTextureFiles[id], fileName, kFileNameMax);
			}

			return true;
		}

		bool FontLoader::ReadChar()
		{
			Font::Glyph glyph;
			memset(&glyph, 0, sizeof(Font::Glyph));

			bool shouldContinue;
			do
			{
				char* pKey = NULL;
				char* pValue = NULL;
				shouldContinue = ReadKvp(pKey, pValue);
				bool result;
				int value;

				if ((pKey != NULL) && (pValue != NULL))
				{
					if (strcmp(pKey, "id") == 0)
					{
						result = ParseNumber(pValue, pKey, &value);
						if (!result)
							return false;
						glyph.id = (u16)value;
					}
					else if (strcmp(pKey, "x") == 0)
					{
						result = ParseNumber(pValue, pKey, &glyph.source.mX);
						if (!result)
							return false;
					}
					else if (strcmp(pKey, "y") == 0)
					{
						result = ParseNumber(pValue, pKey, &glyph.source.mY);
						if (!result)
							return false;
					}
					else if (strcmp(pKey, "width") == 0)
					{
						result = ParseNumber(pValue, pKey, &glyph.source.mWidth);
						if (!result)
							return false;
					}
					else if (strcmp(pKey, "height") == 0)
					{
						result = ParseNumber(pValue, pKey, &glyph.source.mHeight);
						if (!result)
							return false;
					}
					else if (strcmp(pKey, "xoffset") == 0)
					{
						result = ParseNumber(pValue, pKey, &value);
						if (!result)
							return false;
						glyph.xOffset = (s16)value;
					}
					else if (strcmp(pKey, "yoffset") == 0)
					{
						result = ParseNumber(pValue, pKey, &value);
						if (!result)
							return false;
						glyph.yOffset = (s16)value;
					}
					else if (strcmp(pKey, "xadvance") == 0)
					{
						result = ParseNumber(pValue, pKey, &value);
						if (!result)
							return false;
						glyph.xAdvance = (s16)value;
					}
					else if (strcmp(pKey, "page") == 0)
					{
						result = ParseNumber(pValue, pKey, &value);
						if (!result)
							return false;
						glyph.page = (u16)value;
					}
					else if (strcmp(pKey, "chnl") == 0)
					{
						result = ParseNumber(pValue, pKey, &value);
						if (!result)
							return false;

						enum Channel
						{
							kChannelBlue = 0x1,
							kChannelGreen = 0x2,
							kChannelRed = 0x4,
							kChannelAlpha = 0x8,
							kChannelAll = kChannelBlue | kChannelGreen | kChannelRed | kChannelAlpha
						};

						memset(&(glyph.channel), 0, sizeof(Vector4));
						switch (value)
						{
						case kChannelRed:
							glyph.channel.mX = 1.0f;
							break;

						case kChannelGreen:
							glyph.channel.mY = 1.0f;
							break;

						case kChannelBlue:
							glyph.channel.mZ = 1.0f;
							break;

						case kChannelAlpha:
							glyph.channel.mW = 1.0f;
							break;

						case kChannelAll:
							// Nothing
							break;

						default:
							BREAK_MSG("Invalid chnl value %d found in font %s!", value, mpFileName);
							return false;
						}
					}
				}
			} while (shouldContinue);

			if (mCurrentGlyphCount >= (int)mpFont->mGlyphCount)
			{
				BREAK_MSG("Invalid glyph count in font %s!", mpFileName);
				return false;
			}

			ASSERT(mpFont->mpGlyphs != NULL);

			mpFont->mpGlyphs[mCurrentGlyphCount++] = glyph;

			return true;
		}

		bool FontLoader::ReadKerning()
		{
			Font::Kerning kerning;
			memset(&kerning, 0, sizeof(Font::Kerning));

			bool shouldContinue;
			do
			{
				char* pKey = NULL;
				char* pValue = NULL;
				shouldContinue = ReadKvp(pKey, pValue);
				int value;
				bool result;

				if ((pKey != NULL) && (pValue != NULL))
				{
					if (strcmp(pKey, "first") == 0)
					{
						result = ParseNumber(pValue, pKey, &value);
						if (!result)
							return false;
						kerning.first = (u16)value;
					}
					else if (strcmp(pKey, "second") == 0)
					{
						result = ParseNumber(pValue, pKey, &value);
						if (!result)
							return false;
						kerning.second = (u16)value;
					}
					else if (strcmp(pKey, "amount") == 0)
					{
						result = ParseNumber(pValue, pKey, &value);
						if (!result)
							return false;
						kerning.amount = (s16)value;
					}
				}
			} while (shouldContinue);

			if (mCurrentKerningCount >= (int)mpFont->mKerningCount)
			{
				BREAK_MSG("Invalid glyph count in font %s!", mpFileName);
				return false;
			}

			ASSERT(mpFont->mpKernings != NULL);

			mpFont->mpKernings[mCurrentKerningCount++] = kerning;

			return true;
		}

		bool FontLoader::LoadTextures()
		{
			const size_t kPathBufferLen = 256;

			// Find last instance of a slash
			size_t fileNameLen = strlen(mpFileName);
			int slashPos = -1;
			for (int i = (int)fileNameLen - 1; i >= 0; --i)
			{
				char curChar = mpFileName[i];
				switch (curChar)
				{
				case '\\':
				case '/':
					slashPos = i;
					break;
				}

				if (slashPos != -1)
					break;
			}

			// Create the folder part of the buffer
			char pFolderBuffer[kPathBufferLen];
			memset(pFolderBuffer, 0, sizeof(pFolderBuffer));
			if (slashPos >= 0)
			{
				strncpy(pFolderBuffer, mpFileName, (size_t)(slashPos + 1));
			}

			char pFilenameBuffer[kPathBufferLen];
			ASSERT(mpFont != NULL);
			for (unsigned i = 0; i < mpFont->mPageCount; ++i)
			{
				strncpy(pFilenameBuffer, pFolderBuffer, sizeof(pFilenameBuffer));
				strncat(pFilenameBuffer, mpTextureFiles[i], sizeof(pFilenameBuffer));

				Font::Page& page = mpFont->mpPages[i];
				page.pTexture = new Graphics::Texture();
				if (page.pTexture != NULL)
				{
					bool loaded = page.pTexture->LoadTga(pFilenameBuffer);
					if (!loaded)
						return false;
				}
			}

			return true;
		}

	}
}