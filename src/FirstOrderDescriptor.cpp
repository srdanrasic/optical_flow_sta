//
//  FirstOrderDescriptor.cpp
//  optical_flow_sta
//
//  Created by Srđan Rašić on 5/23/13.
//  Copyright (c) 2013 Srđan Rašić. All rights reserved.
//

#include "FirstOrderDescriptor.h"
#include "helper.h"
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/video.hpp>
#include "opencv2/imgproc/imgproc.hpp"

namespace sta
{
  FirstOrderDescriptor::FirstOrderDescriptor(cv::Size2i grid_size, Kernel &kernel, Integrator &integrator)
  : grid_size_(grid_size), kernel_(kernel), integrator_(integrator)
  {
    reset();
  }
  
  void FirstOrderDescriptor::reset()
  {
    descriptor_ = cv::Mat(grid_size_.width * grid_size_.height * kernel_.getHistogramBinCount(), 1, CV_32F);
    current_time_ = 0;
  }
  
  void FirstOrderDescriptor::update(cv::InputArray _data)
  {
    cv::Mat data = _data.getMat();
    
    int patch_rows = data.rows / grid_size_.height;
    int patch_cols = data.cols / grid_size_.width;
    
    assert(patch_rows > 0 && patch_cols > 0);
    
    for (int row = 0; row < grid_size_.height; row++) {
      for (int col = 0; col < grid_size_.width; col++) {
        
        // extract patch (just working with Mat's header, no actual data is modified/copied here)
        cv::Range row_range(row * patch_rows, (row + 1) * patch_rows);
        cv::Range col_range(col * patch_cols, (col + 1) * patch_cols);
        cv::Mat patch(data, row_range, col_range);
        
        // extract histogram from current patch (just working with Mat's header, no actual data is modified/copied here)
        int index = row * grid_size_.width + col;
        cv::Range histogram_range(cv::Range(index * kernel_.getHistogramBinCount(), (index + 1) * kernel_.getHistogramBinCount()));
        cv::Mat descriptor_part_ = descriptor_.rowRange(histogram_range);
        cv::Mat histogram(kernel_.getHistogramBinCount(), 1, CV_32F);
        
        // calc
        kernel_(patch, histogram);
        integrator_(descriptor_part_, histogram, current_time_);
      }
    }
    
    current_time_++;
    
    draw_histogram("Descriptor", descriptor_); cv::moveWindow("Descriptor", 350, 100);
  }
}
