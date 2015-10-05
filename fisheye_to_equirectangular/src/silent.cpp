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
    
    //cout << setfill('0') << setw(5) << 25;
    
    dstWidth = src.cols * 2;
    radius = src.rows / 2;
    center = cv::Point2i(0,0);
    
    gFisheyeCropUtils.mMaxShift = radius / 2;
    gFisheyeCropUtils.mMaxRadius = radius + gFisheyeCropUtils.mMaxShift;
    
    gFisheyeCropUtils.crop(src, crop, radius, center);
    gFisheyeToEquirectangular.updateMap(crop.rows, fisheyeAngle, dstWidth);
    gFisheyeToEquirectangular.transform(crop, dst);
    

    imwrite(outfile, dst);
    
    
    return 0;
}
