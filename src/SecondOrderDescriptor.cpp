//
//  SecondOrderDescriptor.cpp
//  optical_flow_sta
//
//  Created by Srđan Rašić on 5/26/13.
//  Copyright (c) 2013 Srđan Rašić. All rights reserved.
//

#include "SecondOrderDescriptor.h"
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/video/video.hpp>
#include <opencv2/imgproc/imgproc.hpp>

namespace sta
{
  SecondOrderDescriptor::SecondOrderDescriptor(FirstOrderDescriptor &first_order_descriptor, int bins)
  : first_order_descriptor_(first_order_descriptor), bins_(bins)
  {
    first_order_descriptor_.normalize_ = true;
    reset();
  }
  SecondOrderDescriptor::~SecondOrderDescriptor()
  {
    delete [] descriptor_;
  }
  
  void SecondOrderDescriptor::reset()
  {
    first_order_descriptor_.reset();
    
    int descriptor_size = first_order_descriptor_.rows_ * first_order_descriptor_.cols_ * first_order_descriptor_.kernel_.getHistogramBinCount();
    descriptor_ = new cv::Mat[descriptor_size];
    for (int i = 0; i < descriptor_size; i++) {
      descriptor_[i] = cv::Mat::zeros(bins_, 1, CV_32F);
    }
  }
  
  cv::Mat * SecondOrderDescriptor::getDescriptor(bool normalized)
  {
    if (normalized) {
      cv::Mat * result;
      int descriptor_size = first_order_descriptor_.rows_ * first_order_descriptor_.cols_ * first_order_descriptor_.kernel_.getHistogramBinCount();
      result = new cv::Mat[descriptor_size];
      for (int i = 0; i < descriptor_size; i++) {
        cv::normalize(descriptor_[i], result[i]);
      }
      return result;
    } else {
      return descriptor_;
    }
  }
  
  void SecondOrderDescriptor::update(cv::InputArray _data)
  {
    if (first_order_descriptor_.update(_data)) {
      cv::Mat ** sta1 = first_order_descriptor_.getDescriptor();
      
      for (int row = 0; row < first_order_descriptor_.rows_; row++) {
        for (int col = 0; col < first_order_descriptor_.cols_; col++) {
          for (int bin = 0; bin < first_order_descriptor_.kernel_.getHistogramBinCount(); bin++) {
            int didx = (row * first_order_descriptor_.cols_ + col) * first_order_descriptor_.kernel_.getHistogramBinCount() + bin;
            cv::Mat histogram = descriptor_[didx];
            
            float width = bins_;
            float v = sta1[row][col].at<float>(bin, 0) * width;
            int hidx = floor(v);

            if( hidx < 0 )
              hidx += bins_;
            else if (hidx >= bins_)
              hidx -= bins_;
            
            histogram.at<float>(hidx, 0) += 1;
          }
        }
      }
    }
  }
}