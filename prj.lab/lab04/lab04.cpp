#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <random>

cv::Mat hconcat_all(const std::vector<cv::Mat> vec);
cv::Mat createScope(const int& color, const int& radius, const int blur);
cv::Mat createImage(const std::vector<std::vector<cv::Mat>>& images);
cv::Mat addNoise(const cv::Mat image, double msq);
void FindBlobs(const cv::Mat& binary,
               std::vector<std::vector<cv::Point2i>>& blobs);

cv::Mat img, dst, noisy;
int blur = 20;
static void on_trackbar(int, void*) {
  // cv::GaussianBlur(tmp, tmp, cv::Size(0, 0), blur);
  cv::threshold(noisy, dst, blur, 255, cv::THRESH_BINARY);
  imshow("lab04", dst);
}

int main(int argc, char** argv) {
  // int blur = 10;
  std::vector<int> radius = {20, 30, 40, 50, 60};
  std::vector<int> colors = {50, 100, 150, 200, 250};
  std::vector<std::vector<cv::Mat>> images(5, std::vector<cv::Mat>(5));
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      cv::Mat tmp = createScope(colors[i], radius[j], blur);
      images[i][j] = tmp;
    }
  }
  // cv::Mat img = createImage(images);
  img = createImage(images);
  cv::GaussianBlur(img, img, cv::Size(0, 0), blur);
  noisy = addNoise(img, 10);
  cv::namedWindow("lab04", cv::WINDOW_AUTOSIZE);
  on_trackbar(0, 0);
  cv::createTrackbar("Value", "lab04", &blur, 255, on_trackbar);
  std::vector<std::vector<cv::Point2i>> blobs;
  FindBlobs(img, blobs);
  cv::Mat output = cv::Mat::zeros(img.size(), CV_8UC1);
  for (size_t i = 0; i < blobs.size(); i++) {
    for (size_t j = 0; j < blobs[i].size(); j++) {
      int x = blobs[i][j].x;
      int y = blobs[i][j].y;

      output.at<uchar>(y, x) = 100;
    }
  }
  imshow("Priv", output);
  int k = 111;
  while (k != 113) {
    if (k == 111) cv::imshow("lab04", noisy);
    if (k == 110) cv::imshow("lab04", dst);
    k = cv::waitKey(0);
  }
  cv::destroyAllWindows();
  return 0;
}

cv::Mat hconcat_all(const std::vector<cv::Mat> vec) {
  cv::Mat result = vec[0].clone();
  for (int i = 1; i < vec.size(); i++) cv::hconcat(result, vec[i], result);
  return result;
}

cv::Mat createScope(const int& color, const int& radius, const int blur) {
  cv::Mat img = cv::Mat::zeros(256, 256, CV_8U);
  img.setTo(0);
  cv::circle(img, cv::Point(256 / 2, 256 / 2), radius, cv::Scalar(color), -1);
  // cv::GaussianBlur(img, img, cv::Size(0, 0), blur);
  return img;
}

cv::Mat createImage(const std::vector<std::vector<cv::Mat>>& images) {
  cv::Mat img = hconcat_all(images[0]);
  for (int i = 1; i < images.size(); i++) {
    cv::Mat tmp = hconcat_all(images[i]);
    cv::vconcat(img, tmp, img);
  }
  return img;
}

cv::Mat addNoise(const cv::Mat image, double msq) {
  cv::Mat noisyImage = image.clone();

  std::default_random_engine generator;
  std::normal_distribution<double> distribution(0, msq);

  for (int i = 0; i < noisyImage.rows; ++i) {
    for (int j = 0; j < noisyImage.cols; ++j) {
      double noise = distribution(generator);
      double newValue = noisyImage.at<uchar>(i, j) + noise;

      if (newValue < 0)
        newValue = 0;
      else if (newValue > 255)
        newValue = 255;

      noisyImage.at<uchar>(i, j) = static_cast<uchar>(newValue);
    }
  }

  return noisyImage;
}

void FindBlobs(const cv::Mat& binary,
               std::vector<std::vector<cv::Point2i>>& blobs) {
  blobs.clear();

  // Fill the label_image with the blobs
  // 0  - background
  // 1  - unlabelled foreground
  // 2+ - labelled foreground

  cv::Mat label_image;
  binary.convertTo(label_image, CV_32SC1);
  // cv::threshold(binary, binary, blur, 255, cv::THRESH_BINARY);
  // imshow("rfeferferf", binary);

  int label_count = 2;  // starts at 2 because 0,1 are used already

  for (int y = 0; y < label_image.rows; y++) {
    int* row = (int*)label_image.ptr(y);
    for (int x = 0; x < label_image.cols; x++) {
      if (row[x] != 1) {
        continue;
      }

      cv::Rect rect;
      cv::floodFill(label_image, cv::Point(x, y), label_count, &rect, 0, 0, 4);

      std::vector<cv::Point2i> blob;

      for (int i = rect.y; i < (rect.y + rect.height); i++) {
        int* row2 = (int*)label_image.ptr(i);
        for (int j = rect.x; j < (rect.x + rect.width); j++) {
          if (row2[j] != label_count) {
            continue;
          }

          blob.push_back(cv::Point2i(j, i));
        }
      }

      blobs.push_back(blob);

      label_count++;
    }
  }
}