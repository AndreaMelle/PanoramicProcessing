#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace cv;

/// Global variables
Mat src, dst, viz, vizSmall, dstSmall, crop, temp;
Mat map_x, map_y;
int ind = 0;
float fisheyeAngle = M_PI;
int radius = 1;
int dstWidth;

/// Function Headers
void update_map(const cv::Mat& src, const cv::Mat& dst, cv::Mat& map_x, cv::Mat& map_y);

void on_trackbar( int, void* )
{
    radius = std::max(radius, 1);
    src.copyTo(viz);
    cv::circle(viz, Point(viz.cols / 2, viz.rows / 2), radius, Scalar(0, 0, 255.0), 1);
    cv::circle(viz, Point(viz.cols / 2, viz.rows / 2), 3, Scalar(0, 0, 255.0), -1);
    
    cv::Rect roi(src.cols / 2 - radius, src.rows / 2 - radius, src.cols / 2 + radius, src.rows / 2 + radius);
    src.copyTo(temp);
    crop = temp(roi);
    
    update_map(crop, dst, map_x, map_y);
    remap( crop, dst, map_x, map_y, CV_INTER_LINEAR, BORDER_CONSTANT, Scalar(0,0, 0));
    cv::resize(viz, vizSmall, viz.size() / 2);
    cv::resize(dst, dstSmall, dst.size() / 2);
}

/**
 * @function main
 */
int main( int argc, char** argv )
{
    /// Load the image
    //
    src = imread(argv[1] , 1 );
    src.copyTo(crop);
    src.copyTo(temp);
    
    dstWidth = src.cols * 2;
    cv::Size dstSize = cv::Size(dstWidth, dstWidth / 2); //destination is equirectangular
    
    dst.create( dstSize, src.type() );
    map_x.create( dstSize, CV_32FC1 );
    map_y.create( dstSize, CV_32FC1 );
    
    /// Create window
    namedWindow( "equirectangular", CV_WINDOW_AUTOSIZE );
    namedWindow( "fisheye", CV_WINDOW_AUTOSIZE );
    
    radius = src.rows / 2;
    createTrackbar( "fisheye radius", "fisheye", &radius, src.rows / 2, on_trackbar );
    
    src.copyTo(viz);
    cv::circle(viz, Point(viz.cols / 2, viz.rows / 2), radius, Scalar(0, 0, 255.0), 1);
    cv::circle(viz, Point(viz.cols / 2, viz.rows / 2), 3, Scalar(0, 0, 255.0), -1);
    
    
    
    cv::Rect roi(src.cols / 2 - radius, src.rows / 2 - radius, src.cols / 2 + radius, src.rows / 2 + radius);
    src.copyTo(temp);
    crop = temp(roi);
    
    update_map(crop, dst, map_x, map_y);
    remap( crop, dst, map_x, map_y, CV_INTER_LINEAR, BORDER_CONSTANT, Scalar(0,0, 0) );
    
    cv::resize(viz, vizSmall, viz.size() / 2);
    cv::resize(dst, dstSmall, dst.size() / 2);
    
    //cv::Mat croppedImage = image(myROI);
    
    /// Loop
    while( true )
    {
        /// Each 1 sec. Press ESC to exit the program
        int c = waitKey( 1000 );
        
        if( (char)c == 27 )
        { break; }
        else if ((char)c == 's')
        {
            imwrite("sigma8-sample_full_equi.jpg", dst);
        }
        
        imshow( "equirectangular", dstSmall );
        imshow( "fisheye", vizSmall);
    }
    return 0;
}

/**
 * @function update_map
 * @brief Fill the map_x and map_y matrices with 4 types of mappings
 */
void update_map(const cv::Mat& src, const cv::Mat& dst, cv::Mat& map_x, cv::Mat& map_y)
{
    for( int j = 0; j < dst.rows; j++ )
    {
        for( int i = dst.cols / 4; i < 3 * dst.cols / 4; i++ )
        {
            // i is from 0 to dst.cols
            // need to transform to -2 to +2 for equation
            float x = (((float)i / dst.cols) - 0.5f) * 4.0f;
            float y = (((float)j / dst.rows) - 0.5f) * -2.0f;
            
            float tetha = M_PI * x * 0.5f;
            float phi = M_PI * y * 0.5f;
            
            float px = cosf(phi) * sinf(tetha);
            float py = cosf(phi) * cosf(tetha);
            float pz = sinf(phi);
            
            float tetha_1 = atan2f(pz, px);
            float phi_1 = atan2f(sqrtf(px * px + pz * pz), py);
            
            float r = phi_1 / (fisheyeAngle * 0.5f);
            float u = (1 + r * cosf(tetha_1)) * 0.5f;
            float v = (1 + r * sinf(tetha_1)) * 0.5f;
            
            u *= src.cols;
            v = (1.0f - v) * src.rows;
            
            map_x.at<float>(j,i) = u;
            map_y.at<float>(j,i) = v;
            
            //std::cout << "u " << u << " v " << v << std::endl;
            
        }
    }
    
    
}