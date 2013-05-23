//
//  sta.h
//  optical_flow_sta
//
//  Created by Srđan Rašić on 5/23/13.
//  Copyright (c) 2013 Srđan Rašić. All rights reserved.
//

#ifndef __optical_flow_sta__optical_flow_sta__
#define __optical_flow_sta__optical_flow_sta__

#include <opencv2/core/core.hpp>

//! Calculates n-bin histogram of patch
class STAKernel
{
private:
  int number_of_bins_;
  
public:
  //! \param n number of histogram bins
  STAKernel(int n) : number_of_bins_(n) {}
  
  //! \return number of histogram bins
  int getHistogramBinCount() const { return number_of_bins_; }
  
  //! process patch
  virtual void operator()(cv::InputArray patch,       //!> patch to process
                          cv::OutputArray histogram   //!> calculated histogram
                          ) = 0;
};

//! Calculates n-bin histogram of grayscale intensity of patch
class STAIntensityKernel : public STAKernel
{
public:
  virtual void operator()(cv::InputArray patch, cv::OutputArray histogram);
};

//! Calculates STA on input data
class STADescriptor
{
public:
  static cv::OutputArray calc_first_order(cv::InputArray data,  //!> input data (image or optical flow)
                                          cv::Size2i,           //!> grid size
                                          STAKernel& kernel     //!> kernel to use
                                          );

  static cv::OutputArray calc_second_order(cv::InputArray data, //!> input data (image or optical flow)
                                           cv::Size2i,          //!> grid size
                                           STAKernel& kernel,   //!> kernel to use
                                           int m                //!> number of bins of histograms' histogram
                                           );
};

#endif /* defined(__optical_flow_sta__optical_flow_sta__) */
