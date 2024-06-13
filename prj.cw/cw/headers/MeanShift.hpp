#include <array>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
class MeanShift {
 public:
  float hs;  // spatial radius
  float hr;  // color radius
  std::vector<cv::Mat> IMGChannels;

 public:
  MeanShift(float,
            float);  // Constructor for spatial bandwidth and color bandwidth
  void MSFiltering(cv::Mat&);     // Mean Shift Filtering
  void MSSegmentation(cv::Mat&);  // Mean Shift Segmentation
};

//---------------- Definition ---------------------------------------
#define MS_MAX_NUM_CONVERGENCE_STEPS 5  // up to 10 steps are for convergence
#define MS_MEAN_SHIFT_TOL_COLOR 0.3     // minimum mean color shift change
#define MS_MEAN_SHIFT_TOL_SPATIAL 0.3   // minimum mean spatial shift change
const int dxdy[][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1},
                       {0, 1},   {1, -1}, {1, 0},  {1, 1}};  // Region Growing
