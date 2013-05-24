//
//  main.cpp
//  optical_flow_sta
//
//  Created by Srđan Rašić on 5/23/13.
//  Copyright (c) 2013 Srđan Rašić. All rights reserved.
//

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/video.hpp>
#include "opencv2/imgproc/imgproc.hpp"

#include "FirstOrderDescriptor.h"
#include "IntensityKernel.h"
#include "OpticalFlowKernel.h"
#include "MeanIntegrator.h"
#include "Utilities.h"


int main(int, char**)
{
  cv::VideoCapture cap("/Users/srdan/Downloads/running/person02_running_d3_uncomp.avi");
  cv::Mat prev, next, flow;
  
  cv::namedWindow("Descriptor", CV_WINDOW_AUTOSIZE);
  cv::moveWindow("Descriptor", 100, 100);
  cv::namedWindow("Display window", CV_WINDOW_AUTOSIZE);
  cv::moveWindow("Display window", 500, 100);
  
  int number_of_bins = 8;
  cv::Size2i grid_size(5,5);
  
  sta::OpticalFlowKernel kernel(number_of_bins);
  sta::MeanIntegrator integrator;
  sta::FirstOrderDescriptor sta1_descriptor(grid_size, kernel, integrator, true);
  
  if (!cap.isOpened()) {
    return 0;
  }
  
  cap >> prev;
  cv::cvtColor(prev, prev, CV_BGR2GRAY);
  
  while (cap.isOpened()) {
    cap >> next;

    if (next.empty()) {
      break;
    }
    
    cv::cvtColor(next, next, CV_BGR2GRAY);
    cv::calcOpticalFlowFarneback(prev, next, flow, 0.5, 1, 7, 1, 5, 1.1, 0);
    sta1_descriptor.update(flow);
    
    cv::imshow("Display window", next);
    
    draw_histogram("Descriptor", sta1_descriptor.getDescriptor(), grid_size);
    
    prev = next;
    next.release();
    
    if(cv::waitKey(1) >= 0) {
      break;
    }
  }
  
  return 0;
}