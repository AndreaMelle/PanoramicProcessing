#define _USE_MATH_DEFINES
#include <cmath>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include "projectionTransform.h"

using namespace cv;

/// Global variables
Mat src, dst, viz, vizSmall, dstSmall;

float fisheyeAngle = 180.0f * (float)M_PI  / 180.0f;
int radius = 1;
cv::Point2i center;
int dstWidth;
int guiDownsamples = 2;
int shiftRes = 1;

pp::FisheyeToEquirectangular gFisheyeToEquirectangular;

/// Function Headers
void update_map(const cv::Mat& src, const cv::Mat& dst, cv::Mat& map_x, cv::Mat& map_y);

void updateViz( int, void* )
{
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
    cv::flip(imread(argv[1], 1 ), src, -1);
    
	fisheyeAngle = (float)atof(argv[2]) * (float)M_PI / 180.0f;

	if (argc >= 4)
		guiDownsamples = atoi(argv[3]);
	if (argc >= 5)
		shiftRes = atoi(argv[4]);

	radius = (std::min(src.cols, src.rows) / 2) * 2;
    center = cv::Point2i(0,0);
    
	if (argc >= 6)
		center.x = atoi(argv[5]);

	if (argc >= 7)
		center.y = atoi(argv[6]);

	if (argc >= 8)
		radius = atoi(argv[7]);

	gFisheyeToEquirectangular.transform(src, dst, fisheyeAngle, radius, center);
	updateViz(0, 0);
    
    /// Create window
    namedWindow( "equirectangular", CV_WINDOW_AUTOSIZE );
    namedWindow( "fisheye", CV_WINDOW_AUTOSIZE );
    createTrackbar( "fisheye radius", "fisheye", &radius, gFisheyeToEquirectangular.getMaxRadius(src), updateViz );
    
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
			updateViz(0, 0);
		}
        
        else if ((char)c == 's')
        {
			center += cv::Point2i(0, shiftRes);
			updateViz(0, 0);
		}
        
        else if ((char)c == 'a')
        {
			center += cv::Point2i(-shiftRes, 0);
			updateViz(0, 0);
		}
        
        else if ((char)c == 'd')
        {
			center += cv::Point2i(shiftRes, 0);
			updateViz(0, 0);

        }
        
        else if ((char)c == 'r')
        {
            center = cv::Point2i(0,0);
            radius = src.rows / 2;
			updateViz(0, 0);
        }

		else if ((char)c == 'g')
		{
			gFisheyeToEquirectangular.transform(src, dst, fisheyeAngle, radius, center);
			updateViz(0, 0);
		}

        imshow( "equirectangular", dstSmall );
        imshow( "fisheye", vizSmall);
    }
    
    std::cout << "Center: " << center.x << " , " << center.y<<std::endl;
    std::cout << "Radius: " << radius << std::endl;
    
    return 0;
}
