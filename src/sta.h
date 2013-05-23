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


/*! Calculates n-bin histogram of patch
 */
class STAKernel
{
private:
  int number_of_bins_;
  
public:
  /* Creates kernel
   *! \param n number of histogram bins
   */
  STAKernel(int n) : number_of_bins_(n) {}
  
  /*! Returns number of histogram bins
   */
  int getHistogramBinCount() const { return number_of_bins_; }
  
  /*! Process patch
   * \param patch Patch to process
   * \param histogram Calculated/updated histogram
   */
  virtual void operator()(const cv::InputArray &patch, cv::InputOutputArray &histogram) = 0;
};


/*! Calculates n-bin histogram of grayscale intensity of patch
 */
class STAIntensityKernel : public STAKernel
{
public:
  STAIntensityKernel(int n) : STAKernel(n) {}
  virtual void operator()(const cv::InputArray &patch, cv::InputOutputArray &histogram);
};


/*! Calculates STA on input data
 */
class STA1Descriptor
{
private:
  cv::Mat descriptor_;
  cv::Size2i grid_size_;
  STAKernel &kernel_;
  
public:
  /*! Creates descriptor
   *  \param grid_size Grid size
   *  \param kernel Kernel to use
   */
  STA1Descriptor(cv::Size2i grid_size, STAKernel &kernel);

  /*! Returns decriptor
   */
  cv::Mat getDescriptor() const { return descriptor_; }
  
  /*! Updates descriptor with new data
   * \param data Input data (depending on kernel - image or optical flow)
   */
  void update(cv::InputArray data);
};

#endif /* defined(__optical_flow_sta__optical_flow_sta__) */
