#define _USE_MATH_DEFINES
#include <cmath>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include "projectionTransform.h"
#include "argtable3.h"

using namespace cv;

/// Global variables
Mat src, dst, viz, dstSmall;

float fisheyeAngle = 180.0f * (float)M_PI  / 180.0f;
int radius = 1;
cv::Point2i center;
int dstWidth;
int guiDownsamples = 2;
int shiftRes = 1;

pp::FisheyeToEquirectangular* gFisheyeToEquirectangular;

/// Function Headers
void update_map(const cv::Mat& src, const cv::Mat& dst, cv::Mat& map_x, cv::Mat& map_y);

void updateViz( int, void* )
{
	cv::resize(src, viz, src.size() / guiDownsamples);
	cv::Point vizCenter = (Point(src.cols / 2, src.rows / 2) + center) / guiDownsamples;
	cv::circle(viz, vizCenter, radius / guiDownsamples, Scalar(0, 0, 255.0), 1);
	cv::circle(viz, vizCenter, 3, Scalar(0, 0, 255.0), -1);
}

struct arg_lit *verb, *help, *version;
struct arg_int *level;
struct arg_file *o, *file;
struct arg_end *end;

/**
 * @function main
 */
int main( int argc, char** argv )
{
	void *argtable[] = {
		help = arg_litn(NULL, "help", 0, 1, "display this help and exit"),
		version = arg_litn(NULL, "version", 0, 1, "display version info and exit"),
		level = arg_intn(NULL, "level", "<n>", 0, 1, "foo value"),
		verb = arg_litn("v", "verbose", 0, 1, "verbose output"),
		o = arg_filen("o", NULL, "myfile", 0, 1, "output file"),
		file = arg_filen(NULL, NULL, "<file>", 1, 100, "input files"),
		end = arg_end(20),
	};


	assert(argc >= 3);
    cv::flip(imread(argv[1], 1 ), src, -1);
    
    //src = imread(argv[1], 1 );
	
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

	gFisheyeToEquirectangular = new pp::FisheyeToEquirectangular();

	gFisheyeToEquirectangular->apply(src, dst, fisheyeAngle, radius, center);
	cv::resize(dst, dstSmall, dst.size() / guiDownsamples);
	updateViz(0, 0);
    
    /// Create window
    namedWindow( "equirectangular", CV_WINDOW_AUTOSIZE );
    namedWindow( "fisheye", CV_WINDOW_AUTOSIZE );
    createTrackbar( "fisheye radius", "fisheye", &radius, src.cols, updateViz );
    
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
			gFisheyeToEquirectangular->apply(src, dst, fisheyeAngle, radius, center);
			cv::resize(dst, dstSmall, dst.size() / guiDownsamples);
		}

        imshow( "equirectangular", dstSmall );
        imshow( "fisheye", viz);
    }
    
    std::cout << "Center: " << center.x << " , " << center.y<<std::endl;
    std::cout << "Radius: " << radius << std::endl;
    
	src.release();
	dst.release();
	viz.release();
	dstSmall.release();

	delete gFisheyeToEquirectangular;

    return 0;
}
