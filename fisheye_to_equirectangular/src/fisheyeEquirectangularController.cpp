//
//  fisheyeEquirectangularController.cpp

#include "fisheyeEquirectangularController.h"
#include "fisheyeEquirectangularImpl.h"
#include <assert.h>

using namespace pp;

FisheyeEquirectangularController::FisheyeEquirectangularController()
    : paramFovRadians(0)
    , paramRadius(0)
    , paramEquirectWidth(-1)
    , paramAperture(-1.0f)
	, mParamDirtyFlag(true)
	, paramCenter(cv::Point2i(0, 0))
	, paramDownsample(1)
	, paramInterpolationMode(CV_INTER_LINEAR)
{
    
}

FisheyeEquirectangularController::~FisheyeEquirectangularController()
{
	mMapX.release();
	mMapY.release();
	mWorkSrc.release();
	mSrc.release();
	mDst.release();
}

void FisheyeEquirectangularController::open(const char* filename, CVFlipCode flip)
{
	if (flip != FLIP_NONE)
	{
		cv::flip(cv::imread(filename, 1), mSrc, flip);
	}
	else
	{
		cv::imread(filename, 1);
	}
}

void FisheyeEquirectangularController::apply()
{
	fisheye_to_equirectangular_crop(mSrc, mWorkSrc, paramRadius, paramCenter);

    if (mParamDirtyFlag)
    {
		mParamDirtyFlag = false;
		fisheye_to_equirectangular_mapping(mMapX, mMapY, paramFovRadians, paramRadius, paramEquirectWidth, paramAperture);
    }
    
	fisheye_to_equirectangular_image(mWorkSrc, mDst, mMapX, mMapY, paramInterpolationMode);
}

void FisheyeEquirectangularController::setParamFovRadians(const float& inFovRadians)
{
	paramFovRadians = inFovRadians;
	mParamDirtyFlag = true;
}

void FisheyeEquirectangularController::setParamFovDegrees(const float& inFovDegrees)
{
	paramFovRadians = inFovDegrees * (float)M_PI / 180.0f;
	mParamDirtyFlag = true;
}

void FisheyeEquirectangularController::setParamRadius(const int& inRadius)
{
	paramRadius = inRadius;
	mParamDirtyFlag = true;
}

void FisheyeEquirectangularController::setParamEquirectWidth(const int& inEquirectWidth)
{
	paramEquirectWidth = inEquirectWidth;
	mParamDirtyFlag = true;
}

void FisheyeEquirectangularController::setParamAperture(const float& inAperture)
{
	paramAperture = inAperture;
	mParamDirtyFlag = true;
}

void FisheyeEquirectangularController::setParamCenter(const cv::Point2f& inCenter)
{
	paramCenter = inCenter;
}
