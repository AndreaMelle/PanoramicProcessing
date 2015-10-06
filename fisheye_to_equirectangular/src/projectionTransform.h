//
//  projectionTransform.h
//  fisheye_to_equirectangular
//
//  Created by Andrea Melle on 05/10/2015.
//  Copyright © 2015 Andrea Melle. All rights reserved.
//

#ifndef projectionTransform_h
#define projectionTransform_h

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
    /** @brief Models the Fisheye to Equirectangular remapping
     */
    class FisheyeToEquirectangular
    {
    public:
		FisheyeToEquirectangular() {}
		virtual ~FisheyeToEquirectangular() {}
        
        /** @brief Transforms fisheye image into equirectangular, reallocating arrays if needed
         
         @param src
		 @param dst
		 @param center As shift from (0,0) center
		 @param aperture Radius relative to FOV radius. TODO: option to specify it as angle
		 @param interpolationMode
         */
        void transform(const cv::Mat& src,
			cv::Mat& dst,
			const float& fovRadians,
			const int& radius = AUTO_RADIUS,
			const cv::Point2i center = cv::Point2i(0,0),
			const int& equirectWidth = AUTO_EQUIRECT_W,
			const int& aperture = AUTO_APERTURE,
			const int& downsample = 1,
			const int& interpolationMode = CV_INTER_LINEAR);

		int getMaxRadius(const cv::Mat& src);
		int getMinRadius(const cv::Mat& src);
		int getMaxShift(const cv::Mat& src);
        
    private:
        
		void updateConstraints(const cv::Mat& src);

		float mFovAngle;
        int mFisheyeSide;
        int mEquirectWidth;
		int mMinRadius; // 1
		int mMaxRadius; // will determinte max crop canvas side together with max shift
		int mMaxShift; //max shift left or right or up or down of original center

		// working src paramteres - can change over the course of transformations
		int mMaxEvenSrcDim;
		int mWorkRadius;
		cv::Point2i mWorkShift;
        
        cv::Mat mMapX;
        cv::Mat mMapY;
		cv::Mat mWorkSrc;
		cv::Mat mCanvas; //helper canvas for the cropping operation

    };
    
    template <typename T> int sgn(T val) {
        return (T(0) < val) - (val < T(0));
    }
}

#endif /* projectionTransform_h */
