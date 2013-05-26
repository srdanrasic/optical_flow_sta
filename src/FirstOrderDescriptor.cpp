//
//  FirstOrderDescriptor.cpp
//  optical_flow_sta
//
//  Created by Srđan Rašić on 5/23/13.
//

#include "FirstOrderDescriptor.h"
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/video/video.hpp>
#include "opencv2/imgproc/imgproc.hpp"

namespace sta
{
  FirstOrderDescriptor::FirstOrderDescriptor(int rows, int cols, Kernel &kernel, Integrator &integrator, bool normalize)
  : rows_(rows), cols_(cols), kernel_(kernel), integrator_(integrator), normalize_(normalize)
  {
    reset();
  }
  
  void FirstOrderDescriptor::reset()
  {
    descriptor_ = new cv::Mat*[rows_];
    for (int i = 0; i < rows_; i++) {
      descriptor_[i] = new cv::Mat[cols_];
      for (int j = 0; j < cols_; j++) {
        descriptor_[i][j] = cv::Mat::zeros(kernel_.getHistogramBinCount(), 1, CV_32F);
      }
    }
    
    current_time_ = 0;
  }
  
  bool FirstOrderDescriptor::update(cv::InputArray _data)
  {
    cv::Mat data = _data.getMat();
    
    int patch_rows = data.rows / rows_;
    int patch_cols = data.cols / cols_;
    
    if(patch_rows <= 0 || patch_cols <= 0) {
      std::cerr << "Warning: Patch to small to process. Skipping frame." << std::endl;
      return false;
    }
    
    for (int row = 0; row < rows_; row++) {
      for (int col = 0; col < cols_; col++) {
        
        // extract patch (just working with Mat's header, no actual data is modified/copied here)
        cv::Range row_range(row * patch_rows, (row + 1) * patch_rows);
        cv::Range col_range(col * patch_cols, (col + 1) * patch_cols);
        cv::Mat patch(data, row_range, col_range);
        
        cv::Mat histogram = cv::Mat::zeros(kernel_.getHistogramBinCount(), 1, CV_32F);
        
        // calculate histogram
        kernel_(patch, histogram);
        
        // normalize histogram
        if (normalize_) {
          cv::normalize(histogram, histogram);
        }
        
        // get descriptor part that represents histogram for current patch
        cv::Mat descriptor_part_ = descriptor_[row][col];
        
        // integrate
        integrator_(descriptor_part_, histogram, current_time_);
      }
    }
    
    current_time_++;
    return true;
  }
}
