//
//  MeanIntegrator.cpp
//  optical_flow_sta
//
//  Created by Srđan Rašić on 5/24/13.
//

#include "MeanIntegrator.h"

namespace sta
{
  void MeanIntegrator::operator()(cv::InputOutputArray result, cv::InputArray histogram, int t)
  {
    double one_over_t_plus_one = 1.0 / (t + 1);
    cv::addWeighted(histogram, one_over_t_plus_one, result, 1 - one_over_t_plus_one, 0, result);
  }
}