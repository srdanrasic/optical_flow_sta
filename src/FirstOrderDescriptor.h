//
//  FirstOrderDescriptor.h
//  optical_flow_sta
//
//  Created by Srđan Rašić on 5/23/13.
//

#ifndef __optical_flow_sta__optical_flow_sta__
#define __optical_flow_sta__optical_flow_sta__

#include "Kernel.h"
#include "Integrator.h"
#include <opencv2/core/core.hpp>

namespace sta
{
  /*! Calculates first order STA on input data
   */
  class FirstOrderDescriptor
  {
  private:
    cv::Mat ** descriptor_;
    int rows_;
    int cols_;
    Kernel &kernel_;
    Integrator &integrator_;
    int current_time_;
    bool normalize_;
    
  public:
    /*! Creates descriptor
     *  \param grid_size Grid size
     *  \param kernel Kernel to use
     */
    FirstOrderDescriptor(int rows, int cols, Kernel &kernel, Integrator &integate_function, bool normalize);
    
    /*! Returns decriptor
     */
    cv::Mat ** getDescriptor() { return descriptor_; }
    
    /*! Resets state
     *  Call before processing new dataset
     */
    void reset();
    
    /*! Updates descriptor with new data
     * \param data Input data (depending on kernel - image or optical flow)
     */
    bool update(cv::InputArray data);
  
    
    friend class SecondOrderDescriptor;
  };
}

#endif /* defined(__optical_flow_sta__optical_flow_sta__) */
