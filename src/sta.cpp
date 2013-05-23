//
//  sta.cpp
//  optical_flow_sta
//
//  Created by Srđan Rašić on 5/23/13.
//  Copyright (c) 2013 Srđan Rašić. All rights reserved.
//

#include "sta.h"
#include "helper.h"
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/video.hpp>
#include "opencv2/imgproc/imgproc.hpp"


STA1Descriptor::STA1Descriptor(cv::Size2i grid_size, STAKernel &kernel) : grid_size_(grid_size), kernel_(kernel)
{
  descriptor_ = cv::Mat(1, grid_size_.width * grid_size_.height * kernel_.getHistogramBinCount(), CV_32F);
}

void STA1Descriptor::update(cv::InputArray _data)
{
  cv::Mat data = _data.getMat();
  cv::Mat descriptor_step = cv::Mat(descriptor_.rows, descriptor_.cols, CV_32F); // fix: type
  
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
      cv::Range histogram_range(cv::Range(row * col * kernel_.getHistogramBinCount(), (row * col + 1) * kernel_.getHistogramBinCount()));
      cv::Mat histogram(descriptor_step, cv::Range(0, 1), histogram_range);
      
      // calc
      kernel_(patch, histogram);
      
      char name[10]; sprintf(name, "%2dx%2d", row, col);
      draw_histogram(name, histogram);
    }
  }
  
  // TODO
}


void STAIntensityKernel::operator()(cv::InputArray &_patch, cv::InputOutputArray &histogram)
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
