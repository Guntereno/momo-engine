#ifndef MOMO_FORWARDDECLARES_INCLUDED
#define MOMO_FORWARDDECLARES_INCLUDED


// Forward declares for all engine classes
namespace Momo
{
	namespace Graphics
	{
		class Camera;
		class LineBatch;
		class Mesh;
		class Program;
		class SpriteBatch;
		class Targa;
		class Technique;
		class Texture;
		class Utils;
	}

	namespace Input
	{
		struct Event;
		class InputQueue;
		class TouchHandler;
	}

	namespace Io
	{
		class File;
		class FileInMemory;
	}

	namespace Memory
	{
		class DefaultHeap;
		class Heap;
		class StackHeap;
	}

	namespace Text
	{
		class Font;
		class FontLoader;
		class TextWrapper;
	}

	namespace Ui
	{
		class ButtonView;
		class Drawable;
		class ImageDrawable;
		class NinePatchDrawable;
		class TextBoxView;
		class View;
	}

	namespace Util
	{
		class State;
		class StateMachine;
	}

	class ApplicationTimer;
	struct Color;
	class GameBase;
	class GameTime;
	class IdString;
	struct Matrix;
	struct Offset;
	struct Point;
	struct Rectangle;
	struct Vector2;
	struct Vector3;
	struct Vector4;
}

#endif //MOMO_FORWARDDECLARES_INCLUDED
