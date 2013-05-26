//
//  Kernel.h
//  optical_flow_sta
//
//  Created by Srđan Rašić on 5/24/13.
//

#ifndef optical_flow_sta_Kernel_h
#define optical_flow_sta_Kernel_h

#include <opencv2/core/core.hpp>

namespace sta
{
  /*! Calculates n-bin histogram of patch
   */
  class Kernel
  {
  private:
    int number_of_bins_;
    
  public:
    /* Creates kernel
     *! \param n number of histogram bins
     */
    Kernel(int n) : number_of_bins_(n) {}
    
    virtual ~Kernel() {}
    
    /*! Return kernel name 
     */
    virtual cv::string getName() const = 0;
    
    /*! Returns number of histogram bins
     */
    int getHistogramBinCount() const { return number_of_bins_; }
    
    /*! Process patch
     * \param patch Patch to process
     * \param histogram Calculated histogram
     */
    virtual void operator()(const cv::InputArray &patch, cv::InputOutputArray &histogram) = 0;
  };
}

#endif
