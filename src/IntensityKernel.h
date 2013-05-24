//
//  IntensityKernel.h
//  optical_flow_sta
//
//  Created by Srđan Rašić on 5/24/13.
//  Copyright (c) 2013 Srđan Rašić. All rights reserved.
//

#ifndef optical_flow_sta_IntensityKernel_h
#define optical_flow_sta_IntensityKernel_h

#include "Kernel.h"

namespace sta
{
  /*! Calculates n-bin histogram of grayscale intensity of patch
   */
  class IntensityKernel : public Kernel
  {
  public:
    IntensityKernel(int n) : Kernel(n) {}
    virtual void operator()(const cv::InputArray &patch, cv::InputOutputArray &histogram);
  };
}

#endif
