#include <MeanShift.hpp>
#include <Point5D.hpp>
#include <array>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>

cv::Mat extractChannel(const cv::Mat& image, int channel) {
  std::vector<cv::Mat> mv;
  cv::split(image, mv);
  for (int i = 0; i < mv.size(); ++i) {
    if (i != channel) {
      mv[i] = 0;
    }
  }
  cv::Mat dst;
  merge(mv, dst);
  return dst;
}

cv::Mat detectEdges(const cv::Mat& img) {
  // Mat gray;
  // cvtColor(img, gray, COLOR_BGR2GRAY);
  cv::Mat edges;
  cv::Canny(img, edges, 50, 70, 3);
  cv::dilate(edges, edges, cv::Mat());
  // 100 200 5
  return edges;
}

void processChannel(const cv::Mat& channelImg,
                    std::vector<std::vector<cv::Point>>& contours) {
  cv::Mat edges = detectEdges(channelImg);
  std::vector<cv::Vec4i> hierarchy;
  findContours(edges, contours, hierarchy, cv::RETR_TREE,
               cv::CHAIN_APPROX_SIMPLE);

  std::vector<std::vector<cv::Point>> approxContours;
  for (size_t i = 0; i < contours.size(); ++i) {
    std::vector<cv::Point> approx;
    approxPolyDP(contours[i], approx, 0.03 * arcLength(contours[i], true),
                 true);
    if (cv::isContourConvex(approx) && approx.size() == 4 &&
        std::fabs(cv::contourArea(cv::Mat(approx))) > 300) {
      approxContours.push_back(approx);
    }
  }
  contours = approxContours;
}

double contourArea(const std::vector<cv::Point>& contour) {
  return fabs(cv::contourArea(cv::Mat(contour)));
}

std::map<std::string, std::vector<std::vector<cv::Point>>> parser(
    cv::FileStorage file) {
  cv::FileNode root = file.root();
  std::map<std::string, std::vector<std::vector<cv::Point>>> standard;
  for (auto it = root.begin(); it != root.end(); it++) {
    std::string name = (std::string)(*it)["filename"];
    std::vector<cv::Point> point;
    std::vector<std::vector<cv::Point>> contours;
    int count = (int)(*it)["regions"].size();
    for (int j = 0; j < count; j++) {
      for (int i = 0; i < 4; i++) {
        int x = (int)(*it)["regions"][j]["shape_attributes"]["all_points_x"][i];
        int y = (int)(*it)["regions"][j]["shape_attributes"]["all_points_y"][i];
        point.push_back(cv::Point(x, y));
      }
      contours.push_back(point);
      point.clear();
    }
    standard[name] = contours;
  }
  return standard;
};

double qualityTwoContours(std::vector<cv::Point> detect,
                          std::vector<cv::Point> standard) {
  cv::Mat area;
  double areaIntersection = intersectConvexConvex(detect, standard, area);
  if (areaIntersection == 0) return 0;
  double iou = (areaIntersection) /
               (contourArea(detect) + contourArea(standard) - areaIntersection);
  return iou;
}

std::vector<std::vector<cv::Point>> detectAllSquares(cv::Mat img) {
  //   resize(img, img, Size(img.cols / 2, img.rows / 2), 0, 0, 1);
  cvtColor(img, img, cv::COLOR_BGR2Lab);
  MeanShift MSProc(8, 16);
  MSProc.MSFiltering(img);
  cvtColor(img, img, cv::COLOR_Lab2BGR);
  cv::Mat redChannel = extractChannel(img, 2);
  cv::Mat blueChannel = extractChannel(img, 0);
  cv::Mat greenChannel = extractChannel(img, 1);
  std::vector<std::vector<cv::Point>> redContours, blueContours, greenContours;
  processChannel(redChannel, redContours);
  processChannel(blueChannel, blueContours);
  processChannel(greenChannel, greenContours);
  std::vector<std::vector<cv::Point>> allContours;
  allContours.insert(allContours.end(), redContours.begin(), redContours.end());
  allContours.insert(allContours.end(), blueContours.begin(),
                     blueContours.end());
  allContours.insert(allContours.end(), greenContours.begin(),
                     greenContours.end());
  sort(allContours.begin(), allContours.end(),
       [](const std::vector<cv::Point>& a, const std::vector<cv::Point>& b) {
         return contourArea(a) > contourArea(b);
       });

  std::set<int> indexToRemove;
  for (int i = 0; i < allContours.size() - 1; i++) {
    for (int j = i + 1; j < allContours.size(); j++) {
      double temp = qualityTwoContours(allContours[i], allContours[j]);
      if (temp != 0) indexToRemove.insert(j);
    }
  }

  for (auto it = indexToRemove.rbegin(); it != indexToRemove.rend(); ++it) {
    if (*it < allContours.size()) {
      allContours.erase(allContours.begin() + *it);
    }
  }

  return allContours;
}

void simpleDetection() {
  cv::Mat img = cv::imread("../prj.cw/cw/images/2.jpg");
  auto detect = detectAllSquares(img);
  std::cout << detect.size();
  cv::drawContours(img, detect, -1, cv::Scalar(255, 255, 255), 10);
  cv::imshow("img", img);
  cv::waitKey(-1);
}

void qualityDataset() {
  cv::FileStorage file =
      cv::FileStorage("../prj.cw/cw/images/SI_json.json", cv::FileStorage::READ);
  auto standard = parser(file);
  for (auto i : standard) {
    cv::Mat img = cv::imread("../prj.cw/cw/images/" + i.first);
    auto detect = detectAllSquares(img);
    std::set<int> obj;
    double count = 0;
    for (auto j : i.second) {
      for (auto it = detect.begin(); it != detect.end(); it++) {
        if (obj.find(std::distance(detect.begin(), it)) != obj.end()) continue;
        double quality = qualityTwoContours(j, *it);
        if (quality > 0.7) {
          obj.insert(std::distance(detect.begin(), it));
          count++;
          break;
        }
      }
      // cv::polylines(img, j, true, cv::Scalar(255, 0, 0), 10);
    }
    // cv::drawContours(img, detect, 2, cv::Scalar(0, 0, 0), 3);
    std::cout << i.first << " " << count/i.second.size() << "\n";
    // cv::imshow(i.first, img);
  }
  // cv::waitKey(-1);
}

int main() {
  qualityDataset();
  // simpleDetection();
  return 0;
}