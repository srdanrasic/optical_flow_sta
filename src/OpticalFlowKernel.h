//
//  OpticalFlowKernel.h
//  optical_flow_sta
//
//  Created by Srđan Rašić on 5/24/13.
//  Copyright (c) 2013 Srđan Rašić. All rights reserved.
//

#ifndef __optical_flow_sta__OpticalFlowKernel__
#define __optical_flow_sta__OpticalFlowKernel__

#include "Kernel.h"

namespace sta
{
  /*! Calculates n-bin histogram of optical flow of patch
   */
  class OpticalFlowKernel : public Kernel
  {
  public:
    OpticalFlowKernel(int n) : Kernel(n) {}
    virtual void operator()(const cv::InputArray &patch, cv::InputOutputArray &histogram);
  };
}

#endif /* defined(__optical_flow_sta__OpticalFlowKernel__) */
