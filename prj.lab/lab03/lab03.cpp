#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

cv::Mat histogramm(const cv::Mat& src) {
    int histogram[256];

    for (int i = 0; i < 255; i++)
    {
        histogram[i] = 0;
    }

    for (int y = 0; y < src.rows; y++)
        for (int x = 0; x < src.cols; x++)
            histogram[(int)src.at<uchar>(y, x)]++;


    int hist_w = src.cols; int hist_h = src.rows;
    int bin_w = cvRound((double)hist_w / 256);

    cv::Mat histImage(hist_h, hist_w, CV_8UC1, cv::Scalar(255, 255, 255));

    int max = histogram[0];
    for (int i = 1; i < 256; i++) {
        if (max < histogram[i]) {
            max = histogram[i];
        }
    }

    for (int i = 0; i < 255; i++) {
        histogram[i] = ((double)histogram[i] / max) * histImage.rows;
    }


    for (int i = 0; i < 255; i++)
    {
        line(histImage, cv::Point(bin_w * (i), hist_h),
            cv::Point(bin_w * (i), hist_h - histogram[i]),
            cv::Scalar(0, 0, 0), 1, 8, 0);
    }
    return histImage;
}

int* pixel(const cv::Mat& src) {
    int* histogram = new int [256];

    for (int i = 0; i < 255; i++)
    {
        histogram[i] = 0;
    }

    for (int y = 0; y < src.rows; y++)
        for (int x = 0; x < src.cols; x++)
            histogram[(int)src.at<uchar>(y, x)]++;

    return histogram;
}

cv::Mat grey_img_by_quantiles(const cv::Mat img, const double quantil_black, const double quantil_white){
    int* pix = pixel(img);
    for (int i = 0; i < 256; i++)
        std::cout << pix[i] << std::endl;
    int q_b = 0;
    while (pix[q_b] == 0)
        q_b++;
    int q_w = 255;
    while (pix[q_w] == 0)
        q_w--;

    q_b += int(255 * quantil_black);
    q_w -= int(255 * quantil_black);
    cv::Mat new_img;

    double alpha = 255.0 / (double(q_w) - double(q_b));
    int betta = -(q_b);
    img.convertTo(new_img, CV_8UC1, alpha, betta);
    return new_img;
}

cv::Mat color_img_ch_by_ch(const cv::Mat img) {
    cv::Mat lab;
    cv::cvtColor(img, lab, cv::COLOR_BGR2Lab);

    std::vector<cv::Mat> lab_channels;
    cv::split(lab, lab_channels);
    cv::Mat l = lab_channels[0];
    cv::Mat a = lab_channels[1];
    cv::Mat b = lab_channels[2];

    cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE(3.0, cv::Size(8, 8));
    clahe->apply(l, l);

    cv::merge(std::vector<cv::Mat>{l, a, b}, lab);

    cv::Mat output;
    cv::cvtColor(lab, output, cv::COLOR_Lab2BGR);

    return output;
}

cv::Mat color_img_shared(const cv::Mat img, const double quantil_black, const double quantil_white) {
    cv::Mat result;

    std::vector<cv::Mat> hist;
    std::vector<cv::Mat> channels;
    cv::split(img, channels);

    double min = 256.0, max = 0;
    for (int c = 0; c < img.channels(); c++) {
        cv::Mat& channel = channels[c];

        int blackBorder, whiteBorder;
        cv::Mat sorted;
        cv::sort(channel, sorted, cv::SORT_EVERY_COLUMN + cv::SORT_ASCENDING);
        blackBorder = sorted.at<uchar>(cvRound(static_cast<double>(img.rows * quantil_black)), 0);
        if (blackBorder < min) {
            min = blackBorder;
        }
        whiteBorder = sorted.at<uchar>(cvRound(static_cast<double>(img.rows * quantil_white)), 0);
        if (whiteBorder > max) {
            max = whiteBorder;
        }
    }

    for (int c = 0; c < img.channels(); c++) {
        cv::Mat& channel = channels[c];

        int blackBorder, whiteBorder;
        cv::Mat sorted;
        cv::sort(channel, sorted, cv::SORT_EVERY_COLUMN + cv::SORT_ASCENDING);
        blackBorder = min;
        whiteBorder = max;

        for (int i = 0; i < channel.rows; ++i) {
            for (int j = 0; j < channel.cols; ++j) {
                uchar& pixel = channel.at<uchar>(i, j);
                if (pixel < blackBorder)
                    pixel = 0;
                else if (pixel > whiteBorder)
                    pixel = 255;
                else
                    pixel = static_cast<uchar>((pixel - blackBorder) * (255.0 / (whiteBorder - blackBorder)));
            }
        }
        
    }
    cv::merge(channels, result);
    return result;
}


int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " where path to file?!" << std::endl;
        return -1;
    }

    //    cv::Mat image = cv::imread("/Users/master/Downloads/dog_img.jpeg");
    cv::Mat img = cv::imread(argv[1]);
    if (img.empty()) {
        std::cerr << "Could not open or find the image!" << std::endl;
        return -1;
    }

    cv::imshow("img", img);
    cv::Mat hist = histogramm(img);
    cv::imshow("hist", hist);

    double quantil_black = 0.05;
    double quantil_white = 0.95;

    if (img.channels() > 1) {
        cv::Mat img_new_shared = color_img_shared(img, quantil_black, quantil_white);
        cv::imshow("img_new_shared", img_new_shared);
        cv::Mat hist_img_new_shared = histogramm(img_new_shared);
        cv::imshow("hist_img_new_shared", hist_img_new_shared);

        cv::Mat img_new_ch = color_img_ch_by_ch(img);
        cv::imshow("img_new_ch", img_new_ch);
        cv::Mat hist_img_new_ch = histogramm(img_new_ch);
        cv::imshow("hist_img_new_ch", hist_img_new_ch);
    }
    else {
        cv::Mat img_new_grey = grey_img_by_quantiles(img, quantil_black, quantil_white);
        cv::imshow("img_new_grey", img_new_grey);
        cv::Mat hist_img_new_grey = histogramm(img_new_grey);
        cv::imshow("hist_img_new_grey", hist_img_new_grey);
    }

    cv::waitKey();



}
