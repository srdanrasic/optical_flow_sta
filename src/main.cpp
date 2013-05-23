//
//  main.cpp
//  optical_flow_sta
//
//  Created by Srđan Rašić on 5/23/13.
//  Copyright (c) 2013 Srđan Rašić. All rights reserved.
//

#include <iostream>
#include "sta.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/video.hpp>
#include "opencv2/imgproc/imgproc.hpp"


int main(int, char**)
{
  cv::VideoCapture cap("/Users/srdan/Downloads/running/person01_running_d3_uncomp.avi");
  cv::Mat frame;
  cv::namedWindow("Display window", CV_WINDOW_AUTOSIZE);
  
  STAIntensityKernel kernel(23);
  STA1Descriptor descriptor(cv::Size2i(2,2), kernel);
  
  while (cap.isOpened()) {
    cap >> frame;

    if (frame.empty()) {
      break;
    }
    
    cv::cvtColor(frame, frame, CV_BGR2GRAY);
    cv::GaussianBlur(frame, frame, cv::Size2i(7,7), 1.5);
    descriptor.update(frame);
    imshow("Display window", frame);
    
    if(cv::waitKey(30) >= 0) {
      break;
    }
  }
  
  return 0;
}