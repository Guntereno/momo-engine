#ifndef MOMO_TEXT_FONTLOADER_INCLUDED
#define MOMO_TEXT_FONTLOADER_INCLUDED

#include "Momo.h"

#include "Io\File.h"
#include "Text\Font.h"


namespace Momo
{
namespace Text
{

// Class for loading text format AngelCode bitmap fonts
class FontLoader
{
public:
    FontLoader();
    virtual ~FontLoader();

    bool Load(const char* pFileName, Font* pFont);

private:
    void ClearBuffer();

    bool ParseString(const char* pSource, const char* pName, char* pOutput, size_t outputLen);
    bool ParseNumber(const char* pSource, const char* pName, int* pOutput);
    bool ParseBoolean(const char* pSource, const char* pName, bool* pOutput);
    bool ParseVector(const char* pSource, const char* pName, u16* pOutput, size_t outputLen);

    bool ReadLine();

    bool AddCharToBuffer(char character);

    bool ReadKvp(char*& key, char*& value);

    bool ReadInfo();
    bool ReadCommon();
    bool ReadCount(u32& count);
    bool ReadPage();
    bool ReadChar();
    bool ReadKerning();

    bool LoadTextures();

    const char* mpFileName;

    Font* mpFont;

    static constexpr size_t kBufferLen = 512;
    char mpBuffer[kBufferLen];
    size_t mBufferUsed;

    Io::File::Handle mFile;

    int mCurrentGlyphCount;
    int mCurrentKerningCount;

    static constexpr size_t kFileNameMax = 128;
    char mpTextureFiles[Font::kMaxPages][kFileNameMax];
};

}
}

#endif //MOMO_TEXT_FONTLOADER_INCLUDED
