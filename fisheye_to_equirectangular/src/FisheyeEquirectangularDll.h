// FisheyeEquirectangularDll.h

#ifdef FISHEYEEQUIRECTANGULARDLL_EXPORTS
#define FISHEYEEQUIRECTANGULARDLL_API __declspec(dllexport) 
#else
#define FISHEYEEQUIRECTANGULARDLL_API __declspec(dllimport) 
#endif

namespace pp
{
	class FisheyeEquirectangularController;

	class FisheyeEquirectangular
	{
	public:

		static FISHEYEEQUIRECTANGULARDLL_API FisheyeEquirectangularController* CreateController();
		static FISHEYEEQUIRECTANGULARDLL_API void DestroyController(FisheyeEquirectangularController* controller);
	};
}

