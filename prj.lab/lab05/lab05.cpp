#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <random>

cv::Mat changes_3(const cv::Mat img_1, const cv::Mat img_2) {
    cv::Mat img1 = img_1;
    cv::Mat img2 = img_2;


//    img1.convertTo(img1, CV_32F);
//    img2.convertTo(img2, CV_32F);

    cv::Mat sum_of_squares = (img1.mul(img1) + img2.mul(img2));
    cv::Mat result;
    cv::sqrt(sum_of_squares, result);

//    result.convertTo(result, CV_8U);

    return(result);
}

void normalization(cv::Mat& img) {
    cv::normalize(img, img, 0, 255, cv::NORM_MINMAX, CV_8U);
}

void gray2color(cv::Mat& img) {
    cv::cvtColor(img, img, cv::COLOR_GRAY2BGR);
}

cv::Mat gluing_l_r(const cv::Mat& image1, const cv::Mat& image2, const cv::Mat& image3) {
    int width = 3 * 99; // width of 4 images next to each other
    int height = 1 * 99; // height of 1 images over reach other

    cv::Mat inputAll = cv::Mat(height, width, image1.type());

    cv::Rect subImageROI = cv::Rect(0, 0, 99, 99);
    // copy to subimage:
    image1.copyTo(inputAll(subImageROI));

    // move to 2nd, 3nd, 4nd image ROI position:
    subImageROI.x = 99;
    image2.copyTo(inputAll(subImageROI));
    subImageROI.x = 2 * 99;
    image3.copyTo(inputAll(subImageROI));

    /*subImageROI.x = 0;
    subImageROI.y = image1.rows;
    image3.copyTo(inputAll(subImageROI));

    subImageROI.x = image1.cols;
    subImageROI.y = image1.rows;
    image3.copyTo(inputAll(subImageROI));*/
    return inputAll;
}

cv::Mat gluing_u_d(const cv::Mat& image1, const cv::Mat& image2) {
    int width = 1 * 3 * 99; // width of 4 images next to each other
    int height = 2 * 99; // height of 1 images over reach other

    cv::Mat inputAll = cv::Mat(height, width, image1.type());

    cv::Rect subImageROI = cv::Rect(0, 0, 3 * 99, 99);
    // copy to subimage:
    image1.copyTo(inputAll(subImageROI));


    //  subImageROI.x = 0;
    subImageROI.y = image1.rows;
    image2.copyTo(inputAll(subImageROI));


    return inputAll;
}

int main() {
	cv::Mat src_1 = cv::Mat::zeros(cv::Size(99, 99), CV_8U);
	cv::rectangle(src_1, cv::Rect(0, 0, 99, 99), cv::Scalar(0, 0, 0), -1);
	cv::circle(src_1, cv::Point(49.5, 49.5), 25, cv::Scalar(128, 128, 128), -1);


	cv::Mat src_2 = cv::Mat::zeros(cv::Size(99, 99), CV_8U);
	cv::rectangle(src_2, cv::Rect(0, 0, 99, 99), cv::Scalar(0, 0, 0), -1);
	cv::circle(src_2, cv::Point(49.5, 49.5), 25, cv::Scalar(255, 255, 255), -1);

	cv::Mat src_3 = cv::Mat::zeros(cv::Size(99, 99), CV_8U);
	cv::rectangle(src_3, cv::Rect(0, 0, 99, 99), cv::Scalar(128, 128, 128), -1);
	cv::circle(src_3, cv::Point(49.5, 49.5), 25, cv::Scalar(0, 0, 0), -1);

	cv::Mat src_4 = cv::Mat::zeros(cv::Size(99, 99), CV_8U);
	cv::rectangle(src_4, cv::Rect(0, 0, 99, 99), cv::Scalar(128, 128, 128), -1);
	cv::circle(src_4, cv::Point(49.5, 49.5), 25, cv::Scalar(255, 255, 255), -1);

	cv::Mat src_5 = cv::Mat::zeros(cv::Size(99, 99), CV_8U);
	cv::rectangle(src_5, cv::Rect(0, 0, 99, 99), cv::Scalar(255, 255, 255), -1);
	cv::circle(src_5, cv::Point(49.5, 49.5), 25, cv::Scalar(0, 0, 0), -1);

	cv::Mat src_6 = cv::Mat::zeros(cv::Size(99, 99), CV_8U);
	cv::rectangle(src_6, cv::Rect(0, 0, 99, 99), cv::Scalar(255, 255, 255), -1);
	cv::circle(src_6, cv::Point(49.5, 49.5), 25, cv::Scalar(128, 128, 128), -1);

    cv::Mat img_1 = gluing_l_r(src_1, src_2, src_3);
    cv::Mat img_2 = gluing_l_r(src_4, src_5, src_6);

    cv::Mat img = gluing_u_d(img_1, img_2);
    cv::imshow("image", img);

    cv::Mat kernel_1 = (cv::Mat_<float>(2, 2) << 1, 0,
        0, -1);
    cv::Mat I_1;
    cv::filter2D(img, I_1, CV_32F, kernel_1);
    cv::imshow("kernel_1", I_1);

    cv::Mat kernel_2 = (cv::Mat_<float>(2, 2) << 0, 1,
        -1, 0);
    cv::Mat I_2;
    cv::filter2D(img, I_2, CV_32F, kernel_2);
    cv::imshow("kernel_2", I_2);

    cv::Mat I_3 = changes_3(I_1, I_2);
    imshow("kernel_3", I_3);

    normalization(I_1);
    normalization(I_2);
    normalization(I_3);

    cv::Mat channels[] = { I_1, I_2, I_3 };
    cv::Mat mergeImage;
    cv::merge(channels, 3, mergeImage);


    gray2color(img);
    gray2color(I_1);
    gray2color(I_2);
    gray2color(I_3);



    cv::imshow("result", mergeImage);
    cv::waitKey(0);
}

