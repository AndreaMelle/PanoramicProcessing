#define _USE_MATH_DEFINES
#include <cmath>
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include "projectionTransform.h"

using namespace cv;

int main( int argc, char** argv )
{
	pp::FisheyeToEquirectangular gFisheyeToEquirectangular;
	pp::FisheyeCropUtils gFisheyeCropUtils;

    assert(argc == 7);
    
    std::string filename = argv[1];


    
    cv::Point2i center(atoi(argv[2]), atoi(argv[3]));
    int radius = atoi(argv[4]);
    float fisheyeAngle =  (float)atof(argv[5]) * M_PI  / 180.0f;
    
    std::string outfile = argv[6];
    
    Mat src, dst, crop;
    int dstWidth;
    
    src = imread(filename, 1 );

	//make square!
	int maxSide = std::max(src.cols, src.rows);
	maxSide = (maxSide % 2 == 0) ? maxSide : (maxSide + 1);

	cv::Mat temp;
	src.copyTo(temp);
	src.create(maxSide, maxSide, src.type());
	src.setTo(cv::Scalar(0, 0, 0));
	temp.copyTo(src(cv::Rect((maxSide - temp.cols) / 2, (maxSide - temp.rows) / 2, temp.cols, temp.rows)));

    
    //cout << setfill('0') << setw(5) << 25;
   
    
    gFisheyeCropUtils.mMaxShift = radius / 2;
    gFisheyeCropUtils.mMaxRadius = radius + gFisheyeCropUtils.mMaxShift;
    
    gFisheyeCropUtils.crop(src, crop, radius, center);
    gFisheyeToEquirectangular.updateMap(crop.rows, fisheyeAngle);
    gFisheyeToEquirectangular.transform(crop, dst);
    
	std::vector<int> compression_params;
	compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
	compression_params.push_back(100);

    imwrite(outfile, dst, compression_params);
    
    
    return 0;
}
