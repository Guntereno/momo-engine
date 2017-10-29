#ifndef MOMO_UI_ICALLBACK_INCLUDED
#define MOMO_UI_ICALLBACK_INCLUDED


namespace Momo
{
	namespace Ui
	{

		class ICallback
		{
		public:
			virtual void Call(void* pSender) = 0;
		};

	}
}

#endif
