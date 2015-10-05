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
         @param interpolationMode
         */
        void transform(const cv::Mat& src, cv::Mat& dst, const int& interpolationMode = CV_INTER_LINEAR);
        
        /** @brief Samples mapping equation from destination to source
         
         @param fisheyeSide
         @param fisheyeAngle
         @param equirectWidth
         */
        void updateMap(const unsigned int& fisheyeSide, const float& fisheyeAngle, const unsigned int& equirectWidth = AUTO_EQUIRECT_W);
        
    private:
        
        float mFisheyeAngle;
        unsigned int mFisheyeSide;
        unsigned int mEquirectWidth;
        
        cv::Mat mMapX;
        cv::Mat mMapY;
        
    };
    
    /** @brief Helper to crop a fisheye source based on a circle and center
     
     Procudes a new squared image centered in center and of side radius of circle.
     Useful in combination with a GUI or to crop images in barch
     A better model would relates the radius of circle to the focal length of the fisheye lens
     */
    class FisheyeCropUtils
    {
    public:
		FisheyeCropUtils();
		virtual ~FisheyeCropUtils() {}
        
        unsigned int mMinRadius; // 1
        unsigned int mMaxRadius; // will determinte max crop canvas side together with max shift
        unsigned int mMaxShift; //max shift left or right or up or down of original center
        
        void crop(const cv::Mat& src, cv::Mat& dst, const unsigned int& radius, const cv::Point2i& shift);
        
    private:
        
        
        
        cv::Mat mCanvas; //helper canvas for the cropping operation
        
    };
    
    template <typename T> int sgn(T val) {
        return (T(0) < val) - (val < T(0));
    }
}

#endif /* projectionTransform_h */
