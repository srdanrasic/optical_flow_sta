//
//  main.cpp
//  optical_flow_sta
//
//  Created by Srđan Rašić on 5/23/13.
//

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/video.hpp>
#include "opencv2/imgproc/imgproc.hpp"

#include "FirstOrderDescriptor.h"
#include "SecondOrderDescriptor.h"
#include "IntensityKernel.h"
#include "OpticalFlowKernel.h"
#include "MeanIntegrator.h"
#include "Utilities.h"


int main(int, char**)
{
  cv::VideoCapture cap("/Users/srdan/Downloads/running/person02_running_d3_uncomp.avi");
  cv::Mat prev, next, flow;
  
  int number_of_bins = 5;
  int patch_rows = 2;
  int patch_cols = 2;
  
  sta::OpticalFlowKernel kernel(number_of_bins);
  sta::MeanIntegrator integrator;
  sta::FirstOrderDescriptor sta1_descriptor(patch_rows, patch_cols, kernel, integrator, true);
  sta::SecondOrderDescriptor sta2_descriptor(sta1_descriptor, 5);
  
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
    sta2_descriptor.update(flow);
    
    draw_descriptor("Descriptor 1", sta1_descriptor.getDescriptor(), patch_rows, patch_cols, false, next);
    draw_descriptor("Descriptor 2", sta2_descriptor.getDescriptor(), patch_rows * patch_cols * number_of_bins, false);

    
    prev = next;
    next.release();
    
    if(cv::waitKey(0) >= 0) {
      //break;
    }
  }
  
  return 0;
}