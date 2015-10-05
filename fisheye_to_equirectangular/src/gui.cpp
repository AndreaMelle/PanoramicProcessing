#define _USE_MATH_DEFINES
#include <cmath>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include "projectionTransform.h"

using namespace cv;

/// Global variables
Mat src, dst, viz, vizSmall, dstSmall, crop;

float fisheyeAngle = 180.0f * (float)M_PI  / 180.0f;
int radius = 1;
cv::Point2i center;
int dstWidth;
int guiDownsamples = 4;
int shiftRes = 1;

pp::FisheyeToEquirectangular gFisheyeToEquirectangular;
pp::FisheyeCropUtils gFisheyeCropUtils;

/// Function Headers
void update_map(const cv::Mat& src, const cv::Mat& dst, cv::Mat& map_x, cv::Mat& map_y);

void recompute( int, void* )
{
    gFisheyeCropUtils.crop(src, crop, radius, center);
    gFisheyeToEquirectangular.updateMap(crop.rows, fisheyeAngle);
    gFisheyeToEquirectangular.transform(crop, dst);
    
    src.copyTo(viz);
    cv::circle(viz, Point(viz.cols / 2, viz.rows / 2) + center, radius, Scalar(0, 0, 255.0), 1);
    cv::circle(viz, Point(viz.cols / 2, viz.rows / 2) + center, 5, Scalar(0, 0, 255.0), -1);
	cv::resize(viz, vizSmall, viz.size() / guiDownsamples);
	cv::resize(dst, dstSmall, dst.size() / guiDownsamples);
}

/**
 * @function main
 */
int main( int argc, char** argv )
{
	assert(argc >= 3);
    src = imread(argv[1], 1 );
	fisheyeAngle = (float)atof(argv[2]) * (float)M_PI / 180.0f;

	if (argc >= 4)
		guiDownsamples = atoi(argv[3]);
	if (argc >= 5)
		shiftRes = atoi(argv[4]);

    dstWidth = src.cols * 2;
    radius = src.rows / 2;
    center = cv::Point2i(0,0);
    
    gFisheyeCropUtils.mMaxShift = radius / 2;
    gFisheyeCropUtils.mMaxRadius = radius + gFisheyeCropUtils.mMaxShift;

	shiftRes = std::min<unsigned int>(shiftRes, gFisheyeCropUtils.mMaxShift);
    
	if (argc >= 6)
		center.x = atoi(argv[5]);

	if (argc >= 7)
		center.y = atoi(argv[6]);

	if (argc >= 8)
		radius = atoi(argv[7]);

    recompute(0, 0);
    
    
    /// Create window
    namedWindow( "equirectangular", CV_WINDOW_AUTOSIZE );
    namedWindow( "fisheye", CV_WINDOW_AUTOSIZE );
    createTrackbar( "fisheye radius", "fisheye", &radius, gFisheyeCropUtils.mMaxRadius, recompute );
    
    /// Loop
    while( true )
    {
        /// Each 1 sec. Press ESC to exit the program
        int c = waitKey( 1000 );
        
        if( (char)c == 27 )
        { break; }
        else if ((char)c == 'o')
        {
            std::stringstream ss;
            ss << "equi_" << argv[1];
            imwrite(ss.str(), dst);
        }
        
        else if ((char)c == 'w')
        {
			center += cv::Point2i(0, -shiftRes);
            recompute(0, 0);
        }
        
        else if ((char)c == 's')
        {
			center += cv::Point2i(0, shiftRes);
            recompute(0, 0);
        }
        
        else if ((char)c == 'a')
        {
			center += cv::Point2i(-shiftRes, 0);
            recompute(0, 0);
        }
        
        else if ((char)c == 'd')
        {
			center += cv::Point2i(shiftRes, 0);
            recompute(0, 0);
        }
        
        else if ((char)c == 'r')
        {
            center = cv::Point2i(0,0);
            radius = src.rows / 2;
            recompute(0, 0);
        }
        
        imshow( "equirectangular", dstSmall );
        imshow( "fisheye", vizSmall);
    }
    
    std::cout << "Center: " << center.x << " , " << center.y<<std::endl;
    std::cout << "Radius: " << radius << std::endl;
    
    return 0;
}
