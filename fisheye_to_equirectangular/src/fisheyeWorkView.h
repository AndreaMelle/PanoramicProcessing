#ifndef __FISHEYE_WORK_VIEW_H__
#define __FISHEYE_WORK_VIEW_H__

#include "FisheyeEquirectangularDll.h"
#include "ContollerSubject.h"
#include "fisheyeEquirectangularController.h"


namespace pp
{
	/** @brief Display a fisheye image and info from a controller
	*/
	class FisheyeWorkView : public ControllerObserver, public std::enable_shared_from_this<FisheyeWorkView>
	{
	public:
		FisheyeWorkView();
		virtual ~FisheyeWorkView();

		FISHEYEEQUIRECTANGULARDLL_API void bind(std::shared_ptr<FisheyeEquirectangularController> inFisheyeController);
		void unbind(std::shared_ptr<FisheyeEquirectangularController> inFisheyeController);

		virtual void onControllerSourceUpdate(std::weak_ptr<ControllerSubject> sender) override;
		virtual void onControllerParamUpdate(std::weak_ptr<ControllerSubject> sender, const std::string& paramName, const int& paramValue) override;
		virtual void onControllerParamUpdate(std::weak_ptr<ControllerSubject> sender, const std::string& paramName, const cv::Point2i& paramValue) override;

		FISHEYEEQUIRECTANGULARDLL_API void draw();


	private:

		void update();

		std::weak_ptr<FisheyeEquirectangularController> mFisheyeController;

		int mGuiDownsample;
		cv::Mat mViz;

		cv::Scalar mRadiusColor;
		cv::Scalar mCenterColor;
	};
}

#endif //__FISHEYE_WORK_VIEW_H__