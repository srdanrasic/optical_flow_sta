//
//  Utilities.cpp
//  optical_flow_sta
//
//  Created by Srđan Rašić on 5/23/13.
//  Copyright (c) 2013 Srđan Rašić. All rights reserved.
//

#include "Utilities.h"
#include <iostream>
#include <opencv2/highgui/highgui.hpp>


void draw_histogram(const char * window_name, cv::Mat histogram, cv::Size grid_size)
{
  int rows = grid_size.height;
  int cols = grid_size.width;
  int bins = histogram.rows / (rows * cols);
  
  cv::Size2i patch_size(80, 80);
  int patch_spacing = 20;
  
  cv::Mat canvas = cv::Mat::zeros((patch_size.width + patch_spacing) * cols + patch_spacing,
                                  (patch_size.height + patch_spacing) * rows + patch_spacing, CV_8UC3);
  double tickness = patch_size.width / bins;
  
  cv::Scalar color(255,0,0);
  cv::RNG rng(324);
  
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      int hidx = (j * rows + i) * bins;
      
      int icolor = (unsigned)rng;
      color = cv::Scalar(icolor & 255, (icolor >> 8) & 255, (icolor >> 16) & 255);
      
      double max;
      cv::minMaxLoc(histogram.rowRange(hidx, hidx + bins), 0, &max);
      
      for (int k = 0; k < bins; k++) {
        cv::Point origin((patch_size.width + patch_spacing) * i + patch_spacing,
                         (patch_size.height + patch_spacing) * j + patch_spacing);
        cv::line(canvas,
                 origin + cv::Point(k * tickness, patch_size.height),
                 origin + cv::Point(k * tickness, patch_size.height - (histogram.at<float>(hidx + k, 0) / max * (float)patch_size.height)),
                 color, tickness/3);
      }
    }
  }
  
  cv::imshow(window_name, canvas);
}