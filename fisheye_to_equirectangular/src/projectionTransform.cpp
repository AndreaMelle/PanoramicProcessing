//
//  projectionTransform.cpp
//  fisheye_to_equirectangular
//
//  Created by Andrea Melle on 05/10/2015.
//  Copyright © 2015 Andrea Melle. All rights reserved.
//

#include "projectionTransform.h"
#include <assert.h>


using namespace pp;

void FisheyeToEquirectangular::transform(const cv::Mat& src, cv::Mat& dst, const int& interpolationMode)
{
    cv::Size equirectSize(mEquirectWidth, mEquirectWidth / 2);
    dst.create(equirectSize, src.type());
    
    // fisheye must be squared
    assert(src.rows == src.cols);
    
    if(src.rows != mFisheyeSide)
    {
        updateMap(src.rows, mFisheyeAngle, mEquirectWidth);
    }
    
    cv::remap(src, dst, mMapX, mMapY, interpolationMode, cv::BORDER_CONSTANT, cv::Scalar(0,0,0) );
}

void FisheyeToEquirectangular::updateMap(const unsigned int& fisheyeSide,
                                         const float& fisheyeAngle,
                                         const unsigned int& equirectWidth)
{
    assert(fisheyeAngle > 0);
    assert(fisheyeSide > 0);
    
    mFisheyeAngle = fisheyeAngle;
    mFisheyeSide = fisheyeSide;
    
    if(equirectWidth == AUTO_EQUIRECT_W)
    {
        mEquirectWidth = 2 * fisheyeSide;
    }
    else
    {
        mEquirectWidth = equirectWidth;
    }
    
    mEquirectWidth = (mEquirectWidth % 2 == 0) ? mEquirectWidth : (mEquirectWidth + 1);
    
    cv::Size equirectSize(mEquirectWidth, mEquirectWidth / 2);
    
    mMapX.create( equirectSize, CV_32FC1 );
    mMapY.create( equirectSize, CV_32FC1 );
    
    
    for( int j = 0; j < equirectSize.height; j++ )
    {
        for( int i = 0; i < equirectSize.width; i++ )
        {
            // i is from 0 to dst.cols
            // need to transform to -2 to +2 for equation
            float x = (((float)i / equirectSize.width) - 0.5f) * 4.0f;
            float y = (((float)j / equirectSize.height) - 0.5f) * -2.0f;
            
            float tetha = M_PI * x * 0.5f;

			if (fabsf(tetha) >= (mFisheyeAngle * 0.5f))
				continue;

            float phi = M_PI * y * 0.5f;
            
            float px = cosf(phi) * sinf(tetha);
            float py = cosf(phi) * cosf(tetha);
            float pz = sinf(phi);
            
            float tetha_1 = atan2f(pz, px);
            float phi_1 = atan2f(sqrtf(px * px + pz * pz), py);
            
            float r = phi_1 / (mFisheyeAngle * 0.5f);
            float u = (1 + r * cosf(tetha_1)) * 0.5f;
            float v = (1 + r * sinf(tetha_1)) * 0.5f;
            
            u *= fisheyeSide;
            v = (1.0f - v) * fisheyeSide;
            
            mMapX.at<float>(j,i) = u;
            mMapY.at<float>(j,i) = v;
        }
    }
}

FisheyeCropUtils::FisheyeCropUtils()
    : mMaxRadius(0)
    , mMaxShift(0)
	, mMinRadius(1)
{
    
}


void FisheyeCropUtils::crop(const cv::Mat& src, cv::Mat& dst, const unsigned int& radius, const cv::Point2i& shift)
{
    assert(src.cols == src.rows);
    
    if(mMaxRadius <= 0)
    {
        mMaxRadius = src.cols / 2;
    }
    
    mMinRadius = std::max<unsigned int>(mMinRadius, 1);
    
    mMaxShift = std::max<unsigned int>(mMaxShift, 0);
    
    int workRadius = std::max<unsigned int>(std::min<unsigned int>(radius, mMaxRadius), mMinRadius);
    
    cv::Point2i shiftSign(pp::sgn<int>(shift.x), pp::sgn<int>(shift.y));
    cv::Point2i shiftAbs(abs(shift.x), abs(shift.y));
    
    shiftAbs.x = std::max<unsigned int>(std::min<unsigned int>(shiftAbs.x, mMaxShift), 0);
    shiftAbs.y = std::max<unsigned int>(std::min<unsigned int>(shiftAbs.y, mMaxShift), 0);
                                        
    cv::Point2i workShift(shiftSign.x * shiftAbs.x, shiftSign.y * shiftAbs.y);
    
    int canvasHalfSide = mMaxShift + mMaxRadius;
    
    mCanvas.create(2 * canvasHalfSide, 2 * canvasHalfSide, src.type());
    
    int left = canvasHalfSide - src.rows / 2;
    src.copyTo(mCanvas(cv::Rect(left, left, src.cols, src.rows)));
    
    dst.create(workRadius * 2, workRadius * 2, src.type());
    
    left = workShift.x - workRadius + canvasHalfSide;
    int top = workShift.y - workRadius + canvasHalfSide;
    
    mCanvas(cv::Rect(left, top, 2 * workRadius, 2 * workRadius)).copyTo(dst);
    
}





