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

void FisheyeToEquirectangular::transform(const cv::Mat& src,
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
	mFovAngle = fovRadians;

	mMaxEvenSrcDim = (std::max(src.cols, src.rows) / 2) * 2;
	mMinRadius = 1;
	mMaxRadius = mMaxEvenSrcDim / 2;
	mMaxShift = mMaxRadius;

	mWorkRadius = std::max<unsigned int>(std::min<unsigned int>(radius, mMaxRadius), mMinRadius);
	cv::Point2i shiftSign(pp::sgn<int>(center.x), pp::sgn<int>(center.y));
	cv::Point2i shiftAbs(abs(center.x), abs(center.y));
	shiftAbs.x = std::max<unsigned int>(std::min<unsigned int>(shiftAbs.x, mMaxShift), 0);
	shiftAbs.y = std::max<unsigned int>(std::min<unsigned int>(shiftAbs.y, mMaxShift), 0);
	mWorkShift = cv::Point2i(shiftSign.x * shiftAbs.x, shiftSign.y * shiftAbs.y);
	int canvasHalfSide = mMaxShift + mMaxRadius;
	int canvasOrigin = canvasHalfSide - mMaxEvenSrcDim / 2;
	int left = mWorkShift.x - mWorkRadius + canvasHalfSide;
	int top = mWorkShift.y - mWorkRadius + canvasHalfSide;
	
	if (equirectWidth == AUTO_EQUIRECT_W)
	{
		// 2 radius : mFovAngle = w : 2 PI
		mEquirectWidth = (int)((4.0f * M_PI * (float)mWorkRadius) / mFovAngle);
		mEquirectWidth = (mEquirectWidth / 2) * 2;
	}
	else
	{
		mEquirectWidth = equirectWidth;
	}

	int equirectHeight = mEquirectWidth / 2;

	mMapX.create(equirectHeight, mEquirectWidth, CV_32FC1);
	mMapY.create(mMapX.size(), CV_32FC1);
	mMapX.setTo(0);
	mMapY.setTo(0);

	for (int j = 0; j < equirectHeight; j++)
	{
		for (int i = 0; i < mEquirectWidth; i++)
		{
			// i is from 0 to dst.cols
			// need to transform to -2 to +2 for equation
			float x = (((float)i / mEquirectWidth) - 0.5f) * 4.0f;
			float y = (((float)j / equirectHeight) - 0.5f) * -2.0f;

			float tetha = (float)M_PI * x * 0.5f;

			if (fabsf(tetha) >= (mFovAngle * 0.5f))
				continue;

			float phi = (float)M_PI * y * 0.5f;

			float px = cosf(phi) * sinf(tetha);
			float py = cosf(phi) * cosf(tetha);
			float pz = sinf(phi);

			float tetha_1 = atan2f(pz, px);
			float phi_1 = atan2f(sqrtf(px * px + pz * pz), py);

			float r = phi_1 / (mFovAngle * 0.5f);
			float u = (1 + r * cosf(tetha_1)) * 0.5f;
			float v = (1 + r * sinf(tetha_1)) * 0.5f;

			u *= 2 * mWorkRadius;
			v = (1.0f - v) * mWorkRadius * 2;

			mMapX.at<float>(j, i) = u;
			mMapY.at<float>(j, i) = v;
		}
	}


	////

	mWorkSrc.create(mMaxEvenSrcDim, mMaxEvenSrcDim, src.type());
	mWorkSrc.setTo(cv::Scalar(0, 0, 0));
	src.copyTo(mWorkSrc(cv::Rect((mMaxEvenSrcDim - src.cols) / 2, (mMaxEvenSrcDim - src.rows) / 2, src.cols, src.rows)));

	mCanvas.create(2 * canvasHalfSide, 2 * canvasHalfSide, mWorkSrc.type());
	mCanvas.setTo(cv::Scalar(0, 0, 0));
	mWorkSrc.copyTo(mCanvas(cv::Rect(canvasOrigin, canvasOrigin, mWorkSrc.cols, mWorkSrc.rows)));
	mWorkSrc.create(mWorkRadius * 2, mWorkRadius * 2, mCanvas.type());
	mCanvas(cv::Rect(left, top, 2 * mWorkRadius, 2 * mWorkRadius)).copyTo(mWorkSrc);

	dst.create(equirectHeight, mEquirectWidth, src.type());
	cv::remap(mWorkSrc, dst, mMapX, mMapY, interpolationMode, cv::BORDER_CONSTANT, cv::Scalar(0, 0, 0));
}

void FisheyeToEquirectangular::updateConstraints(const cv::Mat& src)
{
	mMaxEvenSrcDim = (std::max(src.cols, src.rows) / 2) * 2;
	mMinRadius = 1;
	mMaxRadius = mMaxEvenSrcDim / 2;
	mMaxShift = mMaxRadius;
}

int FisheyeToEquirectangular::getMaxRadius(const cv::Mat& src)
{
	updateConstraints(src);
	return mMaxRadius;
}
int FisheyeToEquirectangular::getMinRadius(const cv::Mat& src)
{
	updateConstraints(src);
	return mMinRadius;
}

int FisheyeToEquirectangular::getMaxShift(const cv::Mat& src)
{
	updateConstraints(src);
	return mMaxShift;
}

