//
//  MeanIntegrator.h
//  optical_flow_sta
//
//  Created by Srđan Rašić on 5/24/13.
//  Copyright (c) 2013 Srđan Rašić. All rights reserved.
//

#ifndef __optical_flow_sta__MeanIntegrator__
#define __optical_flow_sta__MeanIntegrator__

#include "Integrator.h"

namespace sta
{
  /*! Mean Integrator
   *  Iterativly updates current result with new histogram
   *  so final result is average.
   */
  class MeanIntegrator : public Integrator
  {
  public:
    virtual void operator()(cv::InputOutputArray &result, cv::InputArray histogram, int t);
  };
}

#endif /* defined(__optical_flow_sta__MeanIntegrator__) */
