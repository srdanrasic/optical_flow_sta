//
//  OpticalFlowKernel.cpp
//  optical_flow_sta
//
//  Created by Srđan Rašić on 5/24/13.
//

#include "OpticalFlowKernel.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/video.hpp>
#include "opencv2/imgproc/imgproc.hpp"

namespace sta
{
  void OpticalFlowKernel::operator()(cv::InputArray &_patch, cv::InputOutputArray &_histogram)
  {
    cv::Mat patch = _patch.getMat();
    assert(patch.channels() == 2 && patch.type() == CV_32FC2 && "Need optical flow data.");
    
    cv::Mat xy[2];
    cv::split(patch, xy);
    
    cv::Mat mag, angle;
    cv::cartToPolar(xy[0], xy[1], mag, angle);
    
    int nbins = getHistogramBinCount();
    
    cv::Mat histogram = _histogram.getMatRef();
    
    float angle_scale = nbins / (CV_PI * 2);
    
    for (int i = 0; i < angle.rows; i++) {
      for (int j = 0; j < angle.cols; j++) {
        float m = mag.at<float>(i, j);
        float a = angle.at<float>(i, j) * angle_scale - 0.5f;
        int hidx = floor(a);
        
        if( hidx < 0 )
          hidx += nbins;
        else if (hidx >= nbins)
          hidx -= nbins;
        
        histogram.at<float>(hidx, 0) += m;
      }
    }
  }
}
