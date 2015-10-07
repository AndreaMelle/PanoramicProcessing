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
        FisheyeToEquirectangular();
		virtual ~FisheyeToEquirectangular() {}
        
        /** @brief Transforms fisheye image into equirectangular, reallocating arrays if needed
         
         @param src
		 @param dst
		 @param center As shift from (0,0) center
		 @param aperture Radius relative to FOV radius. TODO: option to specify it as angle
		 @param interpolationMode
         */
        void apply(const cv::Mat& src,
			cv::Mat& dst,
			const float& fovRadians,
			const int& radius,
			const cv::Point2i center = cv::Point2i(0,0),
			const int& equirectWidth = AUTO_EQUIRECT_W,
			const int& aperture = AUTO_APERTURE,
			const int& downsample = 1,
			const int& interpolationMode = CV_INTER_LINEAR);
        
    private:
        
        float paramFovRadians;
        int paramRadius;
        cv::Point2i paramCenter;
        int paramEquirectWidth;
        int paramAperture;
        int paramDownsample;
        int paramInterpolationMode;
        cv::Size paramSrcSize;
        
        //internals
        cv::Rect mSrcFrom;
        cv::Rect mSrcTo;
        cv::Size mEquirectSize;
        float mWorkAperture;
        
        
        cv::Mat mMapX;
        cv::Mat mMapY;
		cv::Mat mWorkSrc;

    };
    
    template <typename T> int sgn(T val) {
        return (T(0) < val) - (val < T(0));
    }
}

#endif /* projectionTransform_h */
