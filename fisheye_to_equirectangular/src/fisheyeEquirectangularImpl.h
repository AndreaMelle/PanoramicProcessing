//
//  fisheyeEquirectangularImpl.h

#ifndef __FISHEYE_EQUIRECTANGULAR_IMPL_H__
#define __FISHEYE_EQUIRECTANGULAR_IMPL_H__

#define _USE_MATH_DEFINES
#include <cmath>
#include <stdio.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#define AUTO_EQUIRECT_W 0
#define AUTO_APERTURE 0
#define AUTO_RADIUS 0

namespace pp
{
	/** @brief Produces the precomputed tables to remap fisheye into equirectangular

	@param mapx destination map for x coordinates, reallocated if needed
	@param mapy destination map for y coordinates, reallocated if needed
	@param fovRadians FOV of the fisheye lens
	@param radius radius in pixel of circle corresponding to fovRadians in your input image
	@param equirectWidth horizontal size of desired destination equirectangular
	if set to AUTO_EQUIRECT_W, optimally computed from radius and fovRadians.
	@param aperture specify FOV crop in radians. Must be <= fovRadians.
	if set to AUTO_APERTURE, set to fovRadians.
	*/
	void fisheye_to_equirectangular_mapping(cv::Mat& mapx,
		cv::Mat& mapy,
		const float& fovRadians,
		const int& radius,
		const int& equirectWidth = AUTO_EQUIRECT_W,
		const float& aperture = AUTO_APERTURE);

	/** @brief Crops arbitrary input images to a fisheye-ready image with specified lens center and radius

	@param src source image.
	@param dst destination image, reallocated if needed.
	@param fovRadians FOV of the fisheye lens.
	@param radius radius in pixel of circle corresponding to the lens FOV in your input image.
	@param center lens center shift from the ideal (0,0).
	*/
	void fisheye_to_equirectangular_crop(const cv::Mat& src,
		cv::Mat& dst,
		const int& radius,
		const cv::Point2i center);

	/** @brief Applies the transformation given the pre-computed mapping

	@param src source image.
	@param dst destination image, reallocated if needed.
	@param mapx source map for x coordinates.
	@param mapy source map for y coordinates.
	@param interpolationMode interpolation method, as in cv::InterpolationFlags.
	*/
	void fisheye_to_equirectangular_image(const cv::Mat& src,
		cv::Mat& dst,
		const cv::Mat& mapx,
		const cv::Mat& mapy,
		const int& interpolationMode = CV_INTER_LINEAR);
    
    template <typename T> int sgn(T val) {
        return (T(0) < val) - (val < T(0));
    }
}

#endif // __FISHEYE_EQUIRECTANGULAR_IMPL_H__
