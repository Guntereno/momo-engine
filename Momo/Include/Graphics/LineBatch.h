#ifndef MOMO_GRAPHICS_LINEBATCH_INCLUDED
#define MOMO_GRAPHICS_LINEBATCH_INCLUDED

#include "Momo.h"

#include "Color.h"
#include "Matrix.h"
#include "Rectangle.h"
#include "Vector.h"

#include "Graphics/Texture.h"


namespace Momo
{
namespace Graphics
{

class LineBatch
{
public:
    LineBatch();

    void Load();

    void Begin();

    void Draw(const Point& from, const Point& to, const Color& color);
    void Draw(const Point& from, const Point& to, const Color& colorFrom, const Color& colorTo);
    void DrawRectangle(const Rectangle& rect, const Color& color);

    void End(const Camera& camera);

private:
    DISALLOW_COPY_AND_ASSIGN(LineBatch);

    static constexpr size_t kVertsPerLine = 2;
    static constexpr size_t kIndicesPerLine = 2;

    static constexpr size_t kLineMax = 8192;
    static constexpr size_t kVertexMax = kLineMax * kVertsPerLine;
    static constexpr size_t kIndexMax = kLineMax * kIndicesPerLine;

    struct Vertex
    {
        static constexpr int kFloatsPerPosition = 2;
        static constexpr int kFloatsPerChannel = 4;
        static constexpr int kBytesPerColor = 4;

        Color color;
        Vector2 position;
    };

    bool LoadTechnique();

    void DrawInternal(const Point& from, const Point& to, const Color& colorFrom, const Color& colorTo);

    GLuint mVertexBufferHandle;
    GLuint mIndexBufferHandle;

    Vertex mVertexData[kVertexMax];

    bool mInBeginEndBlock;
    size_t mLineCount;

    const Camera* mpCamera;
};

}
}

#endif //MOMO_GRAPHICS_LINEBATCH_INCLUDED
