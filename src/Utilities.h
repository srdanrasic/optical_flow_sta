//
//  Utilities.h
//  optical_flow_sta
//
//  Created by Srđan Rašić on 5/23/13.
//

#ifndef __optical_flow_sta__utilities__
#define __optical_flow_sta__utilities__

#include <opencv2/core/core.hpp>

void draw_descriptor(const char * window_name, cv::Mat ** descriptor, int rows, int cols, cv::Mat image = cv::Mat());

#endif /* defined(__optical_flow_sta__helper__) */