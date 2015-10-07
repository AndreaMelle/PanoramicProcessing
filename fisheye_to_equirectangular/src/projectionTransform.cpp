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

FisheyeToEquirectangular::FisheyeToEquirectangular()
    : paramFovRadians(0)
    , paramRadius(0)
    , paramCenter(cv::Point2i(0,0))
    , paramEquirectWidth(-1)
    , paramAperture(-1.0f)
    , paramDownsample(-1)
    , paramInterpolationMode(-1)
    , paramSrcSize(cv::Size(0,0))
{
    
}

void FisheyeToEquirectangular::apply(const cv::Mat& src,
	cv::Mat& dst,
	const float& fovRadians,
	const int& radius,
	const cv::Point2i center,
	const int& equirectWidth,
	const int& aperture,
	const int& downsample,
	const int& interpolationMode)
{
	assert(fovRadians > 0);
    
    if(src.size() != paramSrcSize || radius != paramRadius || center != paramCenter)
    {
        cv::Rect fisheyeRect(0,0, 2 * radius, 2 * radius);
        cv::Point2i centerToCorner(radius - src.cols / 2, radius - src.rows / 2);
        cv::Rect srcRect(-center + centerToCorner, src.size());
        mSrcTo = fisheyeRect & srcRect;
        mWorkSrc.create(2 * radius, 2 * radius, src.type());
        mSrcFrom = cv::Rect(mSrcTo + center - centerToCorner);
    }
    
    mWorkSrc.setTo(cv::Scalar(0,0,0));
    src(mSrcFrom).copyTo(mWorkSrc(mSrcTo));
    
    cv::Mat mSrcFromSmall;
    cv::resize(mWorkSrc, mSrcFromSmall, mWorkSrc.size() / 4);
    imshow("mSrcFrom", mSrcFromSmall);
    
    if (equirectWidth != paramEquirectWidth || fovRadians != paramFovRadians || radius != paramRadius)
    {
        if (equirectWidth == AUTO_EQUIRECT_W)
        {
            // 2 radius : mFovAngle = w : 2 PI
            mEquirectSize.width = (int)((4.0f * M_PI * (float)radius) / fovRadians);
            mEquirectSize.width = (mEquirectSize.width / 2) * 2;
        }
        else
        {
            mEquirectSize.width = equirectWidth;
        }
        
        mEquirectSize.height = mEquirectSize.width / 2;
    }
    
    if(aperture != paramAperture || fovRadians != paramFovRadians)
    {
        if(aperture == AUTO_APERTURE)
        {
            mWorkAperture = fovRadians;
        }
        else
        {
            mWorkAperture = std::min<float>(std::max<float>(1.0f, aperture), fovRadians);
        }
    }
    
    if (equirectWidth != paramEquirectWidth)
    {
        mMapX.create(mEquirectSize, CV_32FC1);
        mMapY.create(mEquirectSize, CV_32FC1);
        dst.create(mEquirectSize, src.type());
    }
    
    if(equirectWidth != paramEquirectWidth || fovRadians != paramFovRadians || radius != paramRadius || aperture != paramAperture)
    {
        mMapX.setTo(0);
        mMapY.setTo(0);
        
        for (int j = 0; j < mEquirectSize.height; j++)
        {
            for (int i = 0; i < mEquirectSize.width; i++)
            {
                // i is from 0 to dst.cols
                // need to transform to -2 to +2 for equation
                float x = (((float)i / mEquirectSize.width) - 0.5f) * 4.0f;
                float y = (((float)j / mEquirectSize.height) - 0.5f) * -2.0f;
                
                float tetha = (float)M_PI * x * 0.5f;
                
                if (fabsf(tetha) >= (mWorkAperture * 0.5f))
                    continue;
                
                float phi = (float)M_PI * y * 0.5f;
                
                float px = cosf(phi) * sinf(tetha);
                float py = cosf(phi) * cosf(tetha);
                float pz = sinf(phi);
                
                float tetha_1 = atan2f(pz, px);
                float phi_1 = atan2f(sqrtf(px * px + pz * pz), py);
                
                float r = phi_1 / (fovRadians * 0.5f);
                float u = (1 + r * cosf(tetha_1)) * 0.5f;
                float v = (1 + r * sinf(tetha_1)) * 0.5f;
                
                u *= 2 * radius;
                v = (1.0f - v) * radius * 2;
                
                mMapX.at<float>(j, i) = u;
                mMapY.at<float>(j, i) = v;
            }
        }
    }

	
    dst.setTo(cv::Scalar(0,0,0));
	cv::remap(mWorkSrc, dst, mMapX, mMapY, interpolationMode, cv::BORDER_CONSTANT, cv::Scalar(0, 0, 0));
    
    paramSrcSize = src.size();
    paramFovRadians = fovRadians;
    paramRadius = radius;
    paramCenter = center;
    paramEquirectWidth = equirectWidth;
    paramAperture = aperture;
    paramDownsample = downsample;
    paramInterpolationMode = interpolationMode;
}
