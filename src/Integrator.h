//
//  Integrator.h
//  optical_flow_sta
//
//  Created by Srđan Rašić on 5/24/13.
//  Copyright (c) 2013 Srđan Rašić. All rights reserved.
//

#ifndef __optical_flow_sta__Integrator__
#define __optical_flow_sta__Integrator__

#include <opencv2/core/core.hpp>

namespace sta
{
  /*! Integrator defines integration step.
   *  Updates result with newly calculated histogram.
   */
  class Integrator
  {
  public:
    virtual void operator()(cv::InputOutputArray &result, cv::InputArray histogram, int t) = 0;
  };
}

#endif /* defined(__optical_flow_sta__Integrator__) */
