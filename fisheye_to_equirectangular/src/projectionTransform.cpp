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
    , paramEquirectWidth(-1)
    , paramAperture(-1.0f)
{
    
}

FisheyeToEquirectangular::~FisheyeToEquirectangular()
{
	mMapX.release();
	mMapY.release();
	mWorkSrc.release();
}


void FisheyeToEquirectangular::apply(const cv::Mat& src,
	cv::Mat& dst,
	const float& fovRadians,
	const int& radius,
	const cv::Point2i center,
	const int& equirectWidth,
	const float& aperture,
	const int& downsample,
	const int& interpolationMode)
{
	fisheye_to_equirectangular_crop(src, mWorkSrc, radius, center);

    if (equirectWidth != paramEquirectWidth
		|| fovRadians != paramFovRadians
		|| radius != paramRadius
		|| aperture != paramAperture)
    {
		fisheye_to_equirectangular_mapping(mMapX, mMapY, fovRadians, radius, equirectWidth, aperture);
    }
    
	fisheye_to_equirectangular_image(mWorkSrc, dst, mMapX, mMapY, interpolationMode);
    
    paramFovRadians = fovRadians;
    paramRadius = radius;
    paramEquirectWidth = equirectWidth;
    paramAperture = aperture;
}

void pp::fisheye_to_equirectangular_mapping(cv::Mat& mapx,
	cv::Mat& mapy,
	const float& fovRadians,
	const int& radius,
	const int& equirectWidth,
	const float& aperture)
{
	assert(fovRadians > 0);
	assert(radius > 0);

	cv::Size equirectSize;
	float effectiveAperture;

	if (equirectWidth == AUTO_EQUIRECT_W)
	{
		equirectSize.width = (int)((4.0f * M_PI * (float)radius) / fovRadians);
		equirectSize.width = (equirectSize.width / 2) * 2;
	}
	else
	{
		equirectSize.width = equirectWidth;
	}

	if (aperture == AUTO_APERTURE)
		effectiveAperture = fovRadians;
	else
		effectiveAperture = std::min<float>(std::max<float>(1.0f, aperture), fovRadians);

	equirectSize.height = equirectSize.width / 2;

	mapx.create(equirectSize, CV_32FC1);
	mapy.create(equirectSize, CV_32FC1);
	mapx.setTo(0);
	mapy.setTo(0);

	float x, y, tetha, phi, px, py, pz, tetha_1, phi_1, r, u, v;

	for (register int j = 0; j < equirectSize.height; j++)
	{
		for (register int i = 0; i < equirectSize.width; i++)
		{
			x = (((float)i / equirectSize.width) - 0.5f) * 4.0f;
			y = (((float)j / equirectSize.height) - 0.5f) * -2.0f;

			tetha = (float)M_PI * x * 0.5f;

			if (fabsf(tetha) >= (effectiveAperture * 0.5f))
				continue;

			phi = (float)M_PI * y * 0.5f;

			px = cosf(phi) * sinf(tetha);
			py = cosf(phi) * cosf(tetha);
			pz = sinf(phi);

			tetha_1 = atan2f(pz, px);
			phi_1 = atan2f(sqrtf(px * px + pz * pz), py);

			r = phi_1 / (fovRadians * 0.5f);
			u = (1 + r * cosf(tetha_1)) * 0.5f;
			v = (1 + r * sinf(tetha_1)) * 0.5f;

			mapx.at<float>(j, i) = 2 * u * radius;
			mapy.at<float>(j, i) = (1.0f - v) * radius * 2;
		}
	}
}

void pp::fisheye_to_equirectangular_crop(const cv::Mat& src,
	cv::Mat& dst,
	const int& radius,
	const cv::Point2i center)
{
	assert(radius > 0);

	int diameter = 2 * radius;
	cv::Point2i centerTL = -center + cv::Point2i(radius - src.cols / 2, radius - src.rows / 2);
	cv::Rect srcTo = cv::Rect(0, 0, diameter, diameter) & cv::Rect(centerTL, src.size());
	cv::Rect srcFrom = cv::Rect(srcTo - centerTL);

	dst.create(diameter, diameter, src.type());
	dst.setTo(cv::Scalar(0, 0, 0));
	src(srcFrom).copyTo(dst(srcTo));
}

void pp::fisheye_to_equirectangular_image(const cv::Mat& src,
	cv::Mat& dst,
	const cv::Mat& mapx,
	const cv::Mat& mapy,
	const int& interpolationMode)
{
	assert(src.cols == src.rows);
	assert(mapx.size() == mapy.size());
	assert(mapx.cols == mapx.rows * 2);

	dst.create(mapx.size(), src.type());
	dst.setTo(cv::Scalar(0, 0, 0));
	cv::remap(src, dst, mapx, mapy, interpolationMode, cv::BORDER_CONSTANT, cv::Scalar(0, 0, 0));
}


