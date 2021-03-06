//
//  Utilities.cpp
//  optical_flow_sta
//
//  Created by Srđan Rašić on 5/23/13.
//

#include "Utilities.h"
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

void draw_descriptor(const char * window_name, cv::Mat * _descriptor, int rows, int cols, int bins, bool normalize)
{
  cv::Mat ** descriptor = new cv::Mat*[rows];
  
  for (int row = 0; row < rows; row++) {
    descriptor[row] = new cv::Mat[cols * bins];
    for (int col = 0; col < cols; col++) {
      for (int bin = 0; bin < bins; bin++) {
        descriptor[row][col*bins+bin] = _descriptor[(row*cols+col)*bins+bin];
      }
    }
  }  

  draw_descriptor(window_name, descriptor, rows, cols * bins, normalize);
}

void draw_descriptor(const char * window_name, cv::Mat ** descriptor, int rows, int cols, bool normalize, cv::Mat image, cv::Rect rect)
{
  int bins = descriptor[0][0].rows;
  
  int patch_rows = 80;
  int patch_cols = 80;
  int patch_spacing = 20;
  
  cv::Mat canvas;
  cv::Size size = cv::Size((patch_cols + patch_spacing) * cols,
                           (patch_rows + patch_spacing) * rows);
  
  if (image.empty()) {
    canvas  = cv::Mat::zeros(size, CV_8UC3);
  } else {
    //double rs = size.height / image.rows, cs = size.width / image.cols;
    cv::rectangle(image, rect, cv::Scalar(0,255,0));
    cv::resize(image, canvas, size);
    cv::cvtColor(canvas, canvas, CV_GRAY2BGR);
  }

  double tickness = patch_cols / bins;
  
  cv::Scalar color(255,0,0);
  cv::RNG rng(324);
  
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      
      int icolor = (unsigned)rng;
      color = cv::Scalar(icolor & 255, (icolor >> 8) & 255, (icolor >> 16) & 255);
      
      double max = 1;
      if (normalize) {
        cv::minMaxLoc(descriptor[i][j], 0, &max);
      }
      
      cv::Point origin((patch_rows + patch_spacing) * j + patch_spacing/2,
                       (patch_cols + patch_spacing) * i + patch_spacing/2);
      
      cv::rectangle(canvas,
                    origin - cv::Point(patch_spacing/2, patch_spacing/2),
                    origin + cv::Point(patch_rows, patch_cols) + cv::Point(patch_spacing/2, patch_spacing/2),
                    cv::Scalar(200, 200, 200));
      
      for (int k = 0; k < bins; k++) {
        cv::line(canvas,
                 origin + cv::Point(k * tickness, patch_rows),
                 origin + cv::Point(k * tickness, patch_rows - (descriptor[i][j].at<float>(k, 0) / max * (float)patch_rows)),
                 color, tickness/3);
      }
    }
  }
  
  cv::imshow(window_name, canvas);
}