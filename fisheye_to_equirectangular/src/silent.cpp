#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include "projectionTransform.h"
#include <iomanip>

using namespace cv;

pp::FisheyeToEquirectangular gFisheyeToEquirectangular;
pp::FisheyeCropUtils gFisheyeCropUtils;

int main( int argc, char** argv )
{
    assert(argc == 7);
    
    std::string filename = argv[1];
    
    cv::Point2i center(atoi(argv[2]), atoi(argv[3]));
    int radius = atoi(argv[4]);
    float fisheyeAngle =  atof(argv[5]) * M_PI  / 180.0f;
    
    std::string outdir = argv[6];
    
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
    
    std::stringstream ss;
    ss << outdir << "equi_" << argv[1];
    imwrite(ss.str(), dst);
    
    std::cout << ss.str() << std::endl;
    
    return 0;
}
