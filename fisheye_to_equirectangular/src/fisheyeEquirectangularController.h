//
//  fisheyeEquirectangularController.h

#ifndef __FISHEYE_EQUIRECTANGULAR_CONTROLLER_H__
#define __FISHEYE_EQUIRECTANGULAR_CONTROLLER_H__

#define _USE_MATH_DEFINES
#include <cmath>
#include <stdio.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "ContollerSubject.h"

#define AUTO_EQUIRECT_W 0
#define AUTO_APERTURE 0
#define AUTO_RADIUS 0

namespace pp
{
	typedef enum CVFlipCode
	{
		FLIP_BOTH = -1,
		FLIP_X = 0,
		FLIP_Y = 1,
		FLIP_NONE = 2,

	} CVFlipCode;

    class FisheyeEquirectangularController : public ControllerSubject
    {
    public:
		FisheyeEquirectangularController();
		virtual ~FisheyeEquirectangularController();

		void open(const char* filename, CVFlipCode flip = CVFlipCode::FLIP_NONE);
		const cv::Mat& get() const { return mDst; }

		void setParamFovRadians(const float& inFovRadians);
		void setParamFovDegrees(const float& inFovDegrees);
		void setParamRadius(const int& inRadius);
		void setParamEquirectWidth(const int& inEquirectWidth);
		void setParamAperture(const float& inAperture);
		void setParamCenter(const cv::Point2i& inCenter);
        
        void apply();
        
    private:
        
        float paramFovRadians;
        int paramRadius;
        int paramEquirectWidth;
        float paramAperture;
		cv::Point2i paramCenter;
		int paramDownsample; //TODO: No setter yet
		int paramInterpolationMode; //TODO: No setter yet

		bool mParamDirtyFlag;

		bool mAutoApplyOnChange;
        
        cv::Mat mMapX;
        cv::Mat mMapY;
		cv::Mat mWorkSrc;

		cv::Mat mSrc;
		cv::Mat mDst;
    };

	
}

#endif //__FISHEYE_EQUIRECTANGULAR_CONTROLLER_H__
