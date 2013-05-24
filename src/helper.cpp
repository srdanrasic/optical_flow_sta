//
//  helper.cpp
//  optical_flow_sta
//
//  Created by Srđan Rašić on 5/23/13.
//  Copyright (c) 2013 Srđan Rašić. All rights reserved.
//

#include "helper.h"
#include <iostream>
#include <opencv2/highgui/highgui.hpp>


void draw_histogram(const char * window_name, cv::Mat histogram)
{
  cv::Mat canvas = cv::Mat::zeros(125, 300, CV_8U);
  double tickness = canvas.cols / (double)histogram.rows;
  
  double max;
  cv::minMaxLoc(histogram, 0, &max);
  
  for (int j = 0, rows = canvas.rows; j < histogram.rows; j++) {
    line(canvas, cv::Point(j * tickness, rows),
         cv::Point(j * tickness, rows - (histogram.at<float>(j,0) * (float)rows) / max),
         cv::Scalar(200, 200, 200),
         tickness
         );
  }
  
  cv::imshow(window_name, canvas);
  //cv::resize(canvas, canvas, cv::Size(125,100), 0, 0, CV_INTER_NN);
}