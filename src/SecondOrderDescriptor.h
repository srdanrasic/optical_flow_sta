//
//  SecondOrderDescriptor.h
//  optical_flow_sta
//
//  Created by Srđan Rašić on 5/26/13.
//  Copyright (c) 2013 Srđan Rašić. All rights reserved.
//

#ifndef __optical_flow_sta__SecondOrderDescriptor__
#define __optical_flow_sta__SecondOrderDescriptor__

#include "FirstOrderDescriptor.h"

namespace sta
{
  /*! Calculates second order STA on input data
   */
  class SecondOrderDescriptor
  {
  private:
    FirstOrderDescriptor &first_order_descriptor_;
    cv::Mat * descriptor_;
    int bins_;
    
  public:
    /*! Creates descriptor
     *  \param first_order_descriptor STA1 upon to build STA2
     */
    SecondOrderDescriptor(FirstOrderDescriptor &first_order_descriptor, int bins);
    ~SecondOrderDescriptor();
    
    /*! Returns decriptor
     */
    cv::Mat * getDescriptor(bool normalized = true);
    
    /*! Resets state
     *  Call before processing new dataset
     */
    void reset();
    
    /*! Updates descriptor with new data
     * \param data Input data (depending on kernel - image or optical flow)
     */
    void update(cv::InputArray data);
  };
}

#endif /* defined(__optical_flow_sta__SecondOrderDescriptor__) */
