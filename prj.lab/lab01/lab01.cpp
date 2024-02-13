#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

cv::Mat gammaCorrection(const cv::Mat& src, float gamma) {
    uchar table[256];
    for (int i = 0; i < 256; i++) {
        table[i] = static_cast<uchar>(pow((i / 255.0), gamma) * 255);
    }
    cv::Mat temp(1, 256, CV_8U, table);
    cv::Mat res;
    cv::LUT(src, temp, res);
    return res;
}


int main(int argc, char** argv) {
    cv::CommandLineParser parser(argc, argv,
        "{s   |3    |number of stripes}"
        "{h   |30   |height of stripes}"
        "{gamma|2.4 |gamma correction}"
        "{@filename|     |output filename}"
    );

    int s = parser.get<int>("s");
    int h = parser.get<int>("h");
    float gamma = parser.get<float>("gamma");
    std::string filename = parser.get<std::string>(0);

    cv::Mat gradient(h, 255 * s, CV_8U);
    for (int i = 0; i < gradient.cols; i++) {
        gradient.col(i).setTo(i / s);
    }

    cv::Mat gammaCorrected = gammaCorrection(gradient, gamma);

    cv::Mat final(h, 255 * s, CV_8U);
    cv::vconcat(gradient, gammaCorrected, final);

    if (!filename.empty()) {
        cv::imwrite(filename, final);
    }
    else {
        cv::imshow("Result", final);
        cv::waitKey(0);
    }
}