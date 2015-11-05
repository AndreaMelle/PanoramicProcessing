// FisheyeEquirectangularDll.h

#ifndef __FISHEYE_EQUIRECTANGULAR_DLL_H__
#define __FISHEYE_EQUIRECTANGULAR_DLL_H__

#ifdef FISHEYEEQUIRECTANGULARDLL_EXPORTS
#define FISHEYEEQUIRECTANGULARDLL_API __declspec(dllexport) 
#else
#define FISHEYEEQUIRECTANGULARDLL_API __declspec(dllimport) 
#endif

namespace pp
{
	class FisheyeEquirectangularController;
	class FisheyeWorkView;

	class FisheyeEquirectangular
	{
	public:

		static FISHEYEEQUIRECTANGULARDLL_API FisheyeEquirectangularController* CreateController();
		static FISHEYEEQUIRECTANGULARDLL_API void DestroyController(FisheyeEquirectangularController* controller);

		static FISHEYEEQUIRECTANGULARDLL_API FisheyeWorkView* CreateWorkView();
		static FISHEYEEQUIRECTANGULARDLL_API void DestroyWorkView(FisheyeWorkView* view);

	};
}

#endif //__FISHEYE_EQUIRECTANGULAR_DLL_H__

