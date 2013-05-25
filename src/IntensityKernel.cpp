//
//  IntensityKernel.cpp
//  optical_flow_sta
//
//  Created by Srđan Rašić on 5/24/13.
//

#include "IntensityKernel.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/video.hpp>
#include "opencv2/imgproc/imgproc.hpp"

namespace sta
{
  void IntensityKernel::operator()(cv::InputArray &_patch, cv::InputOutputArray &histogram)
  {
    cv::Mat patch = _patch.getMat();
    assert(patch.channels() == 1 && patch.type() == CV_8UC1 && "Need greyscale image of 1 chennel");
    
    cv::Mat images[] = { patch };
    int channels[] = { 0 };
    int histSize[] = { getHistogramBinCount() };
    float ch0_ranges[] = { 0, 255 };
    const float * ranges[] = { ch0_ranges };
    
    cv::calcHist(images, 1, channels, cv::Mat(), histogram, 1, histSize, ranges);
  }
}
