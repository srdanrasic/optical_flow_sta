//
//  main.cpp
//  optical_flow_sta
//
//  Created by Srđan Rašić on 5/23/13.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/video.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "FirstOrderDescriptor.h"
#include "SecondOrderDescriptor.h"
#include "IntensityKernel.h"
#include "OpticalFlowKernel.h"
#include "MeanIntegrator.h"
#include "Utilities.h"

enum STAVariant { STA_FIRST, STA_SECOND };
enum STAOpticalFlowMethod { STA_FLOW_FARNEBACK, STA_FLOW_TVL1 };
enum STAMeasurement { STA_INTENSITY, STA_OPTICAL_FLOW };

std::string to_string(cv::Mat * descriptor, int size)
{
  std::stringstream ss;
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < descriptor[0].rows; j++) {
      ss << descriptor[i].at<float>(j,0) << ((i < size - 1 || j < descriptor[0].rows - 1) ? " " : "");
    }
  }
  return ss.str();
}

std::string to_string(cv::Mat ** descriptor, int v, int h)
{
  std::stringstream ss;
  for (int i = 0; i < v; i++) {
    ss << to_string(descriptor[i], h) << ((i < v - 1) ? " " : "");
  }
  return ss.str();
}

void usage()
{
  std::cout << std::endl;
  std::cout << "Use following parameters to configure program:" << std::endl << std::endl;

  std::cout << "   --help    Prints this info." << std::endl << std::endl;
  
  std::cout << "   -v    Visualize calculation. Do not use while evaluating (reduces performance)!" << std::endl;
  std::cout << "   -i    Interactive (wait for any key press before processing next frame)." << std::endl << std::endl;
  
  std::cout << "   --input <path>    Input file path. If not specified, default camera is used." << std::endl << std::endl;
  
  std::cout << "   --annotations <path>    Annotations file. Optional. Bounding box data per line." << std::endl;
  std::cout << "                           Each line defines bounding box: Xmin Ymin Xmax Ymax" << std::endl;
  std::cout << "                           Image origin is in top-left. N-th line represents N-th frame." << std::endl<< std::endl;
  
  std::cout << "   --annotations_stdin     Read annotations from stdin (just like from file)." << std::endl<< std::endl;
  
  std::cout << "   --range <start frame> <end frame>    Specifies range to process. Optional." << std::endl;
  std::cout << "                                        If annotations file is specified, first line" << std::endl;
  std::cout << "                                        must contain bounding box for <start frame> frame." << std::endl << std::endl;

  std::cout << "   --output <path>   Output file path. If not specified, output is printed to stdout." << std::endl;
  std::cout << "                     Descriptor is printed as array of numbers in one line." << std::endl << std::endl;
  
  std::cout << "   --all             Print descriptors for all frames, line by line." << std::endl << std::endl;
  
  std::cout << "   --sta_order 1 | 2  Defines which STA method to use." << std::endl;
  
  std::cout << "   --bins_sta1 N      Number of bins for first order STA." << std::endl;
  std::cout << "   --bins_sta2 N      Number of bins for second order STA. Used only if sta_order == 2." << std::endl;
  
  std::cout << "   --patches_v N      Number of patches vertically." << std::endl;
  std::cout << "   --patches_h N      Number of patches horizontally." << std::endl << std::endl;
  
  std::cout << "   --measurement intensity | flow     Image measurement to use. 'intensity' is default." << std::endl << std::endl;
  
  std::cout << "   If measurement == flow following options are available (all params are required!):" << std::endl << std::endl;
  std::cout << "       --flow_method farneback pyr_scale levels winsize iterations poly_n poly_sigma" << std::endl;
  std::cout << "       --flow_method tvl1 tau lambda theta nscales warps epsilon" << std::endl << std::endl;
  
  std::cout << "       Consult OpenCV documentation for more info on parameters:" << std::endl;
  std::cout << "       http://docs.opencv.org/modules/video/doc/motion_analysis_and_object_tracking.html" << std::endl<< std::endl;
}

int main(int argc, char* argv[])
{
  /* Default params */
  bool visualize = false;
  bool interactive = false;
  
  char * path = NULL;
  char * out_path = NULL;
  char * annotations_path = NULL;
  bool annotations_from_stdin = false;
  
  bool all_frames = false;
  
  int number_of_bins_sta1 = 5;
  int number_of_bins_sta2 = 5;
  int patches_v = 2;
  int patches_h = 2;
  
  int start_frame = 1;
  int end_frame = 0xFFFFFF;
  
  STAVariant variant = STA_SECOND;
  STAOpticalFlowMethod flow_method = STA_FLOW_FARNEBACK;
  STAMeasurement measurement = STA_INTENSITY;
  
  sta::Kernel * kernel = NULL;
  sta::Integrator * integrator = new sta::MeanIntegrator;
  
  /* Farneback params */
  double fb_pyr_scale = 0.5;
  int fb_levels = 1;
  int fb_winsize = 5;
  int fb_iterations = 5;
  int fb_poly_n = 5;
  double fb_poly_sigma = 1.1;
  
  /* TVL1 params */
  double tvl1_tau = 0.25;
  double tvl1_lambda = 0.15;
  double tvl1_theta = 0.3;
  int tvl1_nscales = 5;
  int tvl1_warps = 5;
  double tvl1_epsilon = 0.01;
  
  /* Parse arguments */
  if (argc == 1 || (argc == 2 && strcmp(argv[1], "--help") == 0)) {
    usage();
    return 0;
  }

  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-v") == 0) {
      visualize = true;
    } else if (strcmp(argv[i], "-i") == 0) {
      interactive = true;
    } else if (strcmp(argv[i], "--input") == 0) {
      path = argv[++i];
    } else if (strcmp(argv[i], "--annotations") == 0) {
      annotations_path = argv[++i];
    } else if (strcmp(argv[i], "--annotations_stdin") == 0) {
      annotations_from_stdin = true;
    } else if (strcmp(argv[i], "--range") == 0) {
      if (i + 2 >= argc) { std::cerr << "Both start and end frame must be defined!" << std::endl; return 0; }
      start_frame = atoi(argv[++i]);
      end_frame = atoi(argv[++i]);
    } else if (strcmp(argv[i], "--output") == 0) {
      out_path = argv[++i];
    } else if (strcmp(argv[i], "--all") == 0) {
      all_frames = true;
    } else if (strcmp(argv[i], "--sta_order") == 0) {
      i++;
      if (strcmp(argv[i], "1") == 0)
        variant = STA_FIRST;
      else if (strcmp(argv[i], "2") == 0)
        variant = STA_SECOND;
      else {
        std::cerr << "Invalid argument --sta_order: must be 1 or 2." << std::endl; return 0;
      }
    } else if (strcmp(argv[i], "--bins_sta1") == 0) {
      i++;
      number_of_bins_sta1 = atoi(argv[i]);
    } else if (strcmp(argv[i], "--bins_sta2") == 0) {
      i++;
      number_of_bins_sta2 = atoi(argv[i]);
    } else if (strcmp(argv[i], "--patches_v") == 0) {
      i++;
      patches_v = atoi(argv[i]);
    } else if (strcmp(argv[i], "--patches_h") == 0) {
      i++;
      patches_h = atoi(argv[i]);
    } else if (strcmp(argv[i], "--measurement") == 0) {
      i++;
      if (strcmp(argv[i], "intensity") == 0) {
        measurement = STA_INTENSITY;
      } else if (strcmp(argv[i], "flow") == 0) {
        measurement = STA_OPTICAL_FLOW;
      } else {
        std::cerr << "Invalid argument --measurement: must be 'intensity' or 'flow'." << std::endl; return 0;
      }
    } else if (strcmp(argv[i], "--flow_method") == 0) {
      i++;
      if (strcmp(argv[i], "farneback") == 0) {
        if (i + 6 >= argc) { std::cerr << "All parameters for farneback method are required!" << std::endl; return 0; }
        flow_method = STA_FLOW_FARNEBACK;
        fb_pyr_scale = atof(argv[++i]);
        fb_levels = atoi(argv[++i]);
        fb_winsize = atoi(argv[++i]);
        fb_iterations = atoi(argv[++i]);
        fb_poly_n = atoi(argv[++i]);
        fb_poly_sigma = atof(argv[++i]);
      } else if (strcmp(argv[i], "tvl1") == 0) {
        if (i + 6 >= argc) { std::cerr << "All parameters for tvl1 method are required!" << std::endl; return 0; }
        flow_method = STA_FLOW_TVL1;
        tvl1_tau = atof(argv[++i]);
        tvl1_lambda = atof(argv[++i]);
        tvl1_theta = atof(argv[++i]);
        tvl1_nscales = atoi(argv[++i]);
        tvl1_warps = atoi(argv[++i]);
        tvl1_epsilon = atof(argv[++i]);
        //tvl1_iterations = atoi(argv[++i]);
      } else {
        std::cerr << "Invalid flow_method." << std::endl; return 0;
      }
    }
  }
  
  /* Open stream */
  cv::VideoCapture cap;
  if (path == NULL) {
    cap = cv::VideoCapture(0);
  } else {
    cap = cv::VideoCapture(path);
    cap.set(CV_CAP_PROP_POS_FRAMES, start_frame);
  }
  
  if (!cap.isOpened()) {
    std::cerr << "Error: Unable to open stream." << std::endl;
    return 0;
  }
  
 
   std::ifstream annotations_file;
  if (annotations_path != NULL) {
    annotations_file.open(annotations_path);
    if (!annotations_file.is_open()) {
      std::cerr << "Unable to open annotations file." << std::endl;
      return 0;
    }
  }
  
  std::istream &annotations = (annotations_from_stdin) ? std::cin : annotations_file;
  
  std::ofstream out_file_stream(out_path);
  std::ostream &out_stream = (out_file_stream.is_open()) ? out_file_stream : std::cout;
  
  if (measurement == STA_INTENSITY) {
    kernel = new sta::IntensityKernel(number_of_bins_sta1);
  } else if (measurement == STA_OPTICAL_FLOW) {
    kernel = new sta::OpticalFlowKernel(number_of_bins_sta1);
  }
  
  sta::FirstOrderDescriptor sta1_descriptor(patches_v, patches_h, *kernel, *integrator, true);
  sta::SecondOrderDescriptor sta2_descriptor(sta1_descriptor, number_of_bins_sta2);
  
  cv::Ptr<cv::DenseOpticalFlow> tvl1 = cv::createOptFlow_DualTVL1();
  tvl1->set("tau", tvl1_tau);
  tvl1->set("lambda", tvl1_lambda);
  tvl1->set("theta", tvl1_theta);
  tvl1->set("nscales", tvl1_nscales);
  tvl1->set("warps", tvl1_warps);
  tvl1->set("epsilon", tvl1_epsilon);
  
  int current_frame = start_frame;
  cv::Mat prev, next, flow;

  cap >> prev;
  
  double xmin = 0, ymin = 0, xmax = 0, ymax = 0;
  if (annotations.peek() != EOF) { annotations >> ymin >> xmin >> ymax >> xmax; }
  
  if (prev.empty()) {
    std::cerr << "Error: No frames found or unable to read stream." << std::endl;
    return 0;
  }
  
  cv::cvtColor(prev, prev, CV_BGR2GRAY);
  
  while (cap.isOpened()) {
    
    current_frame++;
    
    if (current_frame > end_frame) {
      break;
    }
    
    cap >> next;
    
    // if no more frames, break
    if (next.empty()) break;
        
    cv::cvtColor(next, next, CV_BGR2GRAY);
    
    // we'll need it later
    cv::Mat original_next = next;
    
    if (flow_method == STA_FLOW_FARNEBACK) {
      cv::calcOpticalFlowFarneback(prev, next, flow, fb_pyr_scale, fb_levels, fb_winsize, fb_iterations, fb_poly_n, fb_poly_sigma, 0);
    } else if (flow_method == STA_FLOW_TVL1) {
      tvl1->calc(prev, next, flow);
    }
    
    cv::Mat * data = &next;
    if (kernel->getName() == "OpticalFlowKernel") {
      data = &flow;
    }
    
    if (annotations.peek() != EOF) {
      annotations >> ymin >> xmin >> ymax >> xmax;
      (*data) = (*data).rowRange(xmin, xmax).colRange(ymin, ymax);
      next = original_next.rowRange(xmin, xmax).colRange(ymin, ymax);
    }
    
    if (variant == STA_FIRST) {
      sta1_descriptor.update(*data);
      if (all_frames) out_stream << to_string(sta1_descriptor.getDescriptor(), patches_v, patches_h) << std::endl;
    } else if (variant == STA_SECOND) {
      sta2_descriptor.update(*data);
      if (all_frames) out_stream << to_string(sta2_descriptor.getDescriptor(), patches_v * patches_h * number_of_bins_sta1) << std::endl;
    }
    
    prev = original_next;

    if (visualize) {
      if (variant == STA_SECOND) {
        draw_descriptor("STA 2", sta2_descriptor.getDescriptor(), patches_v, patches_h, number_of_bins_sta1, false);
        cv::moveWindow("STA 2", 50, 300);
      }
      
      draw_descriptor("STA 1", sta1_descriptor.getDescriptor(), patches_v, patches_h, false, next);
      cv::moveWindow("STA 1", 50, 0);
      
      if(cv::waitKey((interactive) ? 0 : 1) == 27) {
        break;
      }
    }
  }
  
  if (!all_frames) {
    if (variant == STA_FIRST) {
      out_stream << to_string(sta1_descriptor.getDescriptor(), patches_v, patches_h) << std::endl;
    } else if (variant == STA_SECOND) {
      out_stream << to_string(sta2_descriptor.getDescriptor(), patches_v * patches_h * number_of_bins_sta1) << std::endl;
    }
  }
  
  delete kernel;
  delete integrator;
  
  return 0;
}