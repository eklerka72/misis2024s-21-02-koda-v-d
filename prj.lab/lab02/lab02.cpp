#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <random>

cv::Mat histogramm(const cv::Mat& src) {
    int histogram[256];

    for (int i = 0; i < 255; i++)
    {
        histogram[i] = 0;
    }

    for (int y = 0; y < src.rows; y++)
        for (int x = 0; x < src.cols; x++)
            histogram[(int)src.at<uchar>(y, x)]++;

    for (int i = 0; i < 256; i++)
        std::cout << histogram[i] << " ";

    int hist_w = 256; int hist_h = 256;
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
cv::Mat addNoise(const cv::Mat& image, double msq) {
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

cv::Mat gluing_l_r(const cv::Mat& image1, const cv::Mat& image2, const cv::Mat& image3, const cv::Mat& image4) {
    int width = 4 * 256; // width of 4 images next to each other
    int height = 1 * 256; // height of 1 images over reach other

    cv::Mat inputAll = cv::Mat(height, width, image1.type());

    cv::Rect subImageROI = cv::Rect(0, 0, 256, 256);
    // copy to subimage:
    image1.copyTo(inputAll(subImageROI));

    // move to 2nd, 3nd, 4nd image ROI position:
    subImageROI.x = 256;
    image2.copyTo(inputAll(subImageROI));
    subImageROI.x = 2* 256;
    image3.copyTo(inputAll(subImageROI));
    subImageROI.x = 3* 256;
    image4.copyTo(inputAll(subImageROI));

    /*subImageROI.x = 0;
    subImageROI.y = image1.rows;
    image3.copyTo(inputAll(subImageROI));

    subImageROI.x = image1.cols;
    subImageROI.y = image1.rows;
    image3.copyTo(inputAll(subImageROI));*/
    return inputAll;
}

cv::Mat gluing_u_d(const cv::Mat& image1, const cv::Mat& image2, const cv::Mat& image3, const cv::Mat& image4, const cv::Mat& image5, const cv::Mat& image6, const cv::Mat& image7, const cv::Mat& image8) {
    int width = 1* 4 * 256; // width of 4 images next to each other
    int height = 8 * 256; // height of 1 images over reach other

    cv::Mat inputAll = cv::Mat(height, width, image1.type());

    cv::Rect subImageROI = cv::Rect(0, 0, 4*256, 256);
    // copy to subimage:
    image1.copyTo(inputAll(subImageROI));


  //  subImageROI.x = 0;
    subImageROI.y = image1.rows;
    image2.copyTo(inputAll(subImageROI));
    subImageROI.y = 2 * image1.rows;
    image3.copyTo(inputAll(subImageROI));
    subImageROI.y = 3 * image1.rows;
    image4.copyTo(inputAll(subImageROI));
    subImageROI.y = 4 * image1.rows;
    image5.copyTo(inputAll(subImageROI));
    subImageROI.y = 5 * image1.rows;
    image6.copyTo(inputAll(subImageROI));
    subImageROI.y = 6 * image1.rows;
    image7.copyTo(inputAll(subImageROI));
    subImageROI.y = 7 * image1.rows;
    image8.copyTo(inputAll(subImageROI));

  
    return inputAll;
}

int main() {
    //Построение 4-х изображений
	cv::Mat src_1 = cv::Mat::zeros(cv::Size(256, 256), CV_8U);
    cv::rectangle(src_1, cv::Rect(0, 0, 256, 256), cv::Scalar(0, 0, 0), -1);
	cv::rectangle(src_1, cv::Rect(23.5, 23.5, 209, 209), cv::Scalar(127, 127, 127), -1);
	cv::circle(src_1, cv::Point(128, 128), 83, cv::Scalar(255, 255, 255), -1);

    cv::Mat src_2 = cv::Mat::zeros(cv::Size(256, 256), CV_8U);
    cv::rectangle(src_2, cv::Rect(0, 0, 256, 256), cv::Scalar(20, 20, 20), -1);
    cv::rectangle(src_2, cv::Rect(23.5, 23.5, 209, 209), cv::Scalar(127, 127, 127), -1);
    cv::circle(src_2, cv::Point(128, 128), 83, cv::Scalar(235, 235, 235), -1);

    cv::Mat src_3 = cv::Mat::zeros(cv::Size(256, 256), CV_8U);
    cv::rectangle(src_3, cv::Rect(0, 0, 256, 256), cv::Scalar(55, 55, 55), -1);
    cv::rectangle(src_3, cv::Rect(23.5, 23.5, 209, 209), cv::Scalar(127, 127, 127), -1);
    cv::circle(src_3, cv::Point(128, 128), 83, cv::Scalar(200, 200, 200), -1);

    cv::Mat src_4 = cv::Mat::zeros(cv::Size(256, 256), CV_8U);
    cv::rectangle(src_4, cv::Rect(0, 0, 256, 256), cv::Scalar(90, 90, 90), -1);
    cv::rectangle(src_4, cv::Rect(23.5, 23.5, 209, 209), cv::Scalar(127, 127, 127), -1);
    cv::circle(src_4, cv::Point(128, 128), 83, cv::Scalar(165, 165, 165), -1);

    cv::Mat src = gluing_l_r(src_1, src_2, src_3, src_4);
    //cv::imshow("Image", src);
	
   //Построение гистограммы
    cv::Mat histImage_1 = histogramm(src_1);
    cv::Mat histImage_2 = histogramm(src_2);
    cv::Mat histImage_3 = histogramm(src_3);
    cv::Mat histImage_4 = histogramm(src_4);
    cv::Mat histImage = gluing_l_r(histImage_1, histImage_2, histImage_3, histImage_4);
    //cv::imshow("Histogram", histImage);
   
    // Наложение шума cо среднеквадратическим значением 3
    cv::Mat noise_image_3_1 = addNoise(src_1, 3);
    cv::Mat noise_image_3_2 = addNoise(src_2, 3);
    cv::Mat noise_image_3_3 = addNoise(src_3, 3);
    cv::Mat noise_image_3_4 = addNoise(src_4, 3);
    cv::Mat noise_image_3 = gluing_l_r(noise_image_3_1, noise_image_3_2, noise_image_3_3, noise_image_3_4);
    //cv::imshow("Image with noise 3", noise_image_3);

    //Построение гистограммы c шумом cо среднеквадратическим значением 3
    cv::Mat histImage_noise_3_1 = histogramm(noise_image_3_1);
    cv::Mat histImage_noise_3_2 = histogramm(noise_image_3_2);
    cv::Mat histImage_noise_3_3 = histogramm(noise_image_3_3);
    cv::Mat histImage_noise_3_4 = histogramm(noise_image_3_4);
    cv::Mat histImage_noise_3 = gluing_l_r(histImage_noise_3_1, histImage_noise_3_2, histImage_noise_3_3, histImage_noise_3_4);
    //cv::imshow("Histogram with noise 3", histImage_noise_3);
    
    // Наложение шума cо среднеквадратическим значением 7
    cv::Mat noise_image_7_1 = addNoise(src_1, 7);
    cv::Mat noise_image_7_2 = addNoise(src_2, 7);
    cv::Mat noise_image_7_3 = addNoise(src_3, 7);
    cv::Mat noise_image_7_4 = addNoise(src_4, 7);
    cv::Mat noise_image_7 = gluing_l_r(noise_image_7_1, noise_image_7_2, noise_image_7_3, noise_image_7_4);
    //cv::imshow("Image with noise 7", noise_image_7);

    //Построение гистограммы c шумом cо среднеквадратическим значением 7
    cv::Mat histImage_noise_7_1 = histogramm(noise_image_7_1);
    cv::Mat histImage_noise_7_2 = histogramm(noise_image_7_2);
    cv::Mat histImage_noise_7_3 = histogramm(noise_image_7_3);
    cv::Mat histImage_noise_7_4 = histogramm(noise_image_7_4);
    cv::Mat histImage_noise_7 = gluing_l_r(histImage_noise_7_1, histImage_noise_7_2, histImage_noise_7_3, histImage_noise_7_4);
    //cv::imshow("Histogram with noise 7", histImage_noise_7);

    // Наложение шума cо среднеквадратическим значением 15
    cv::Mat noise_image_15_1 = addNoise(src_1, 15);
    cv::Mat noise_image_15_2 = addNoise(src_2, 15);
    cv::Mat noise_image_15_3 = addNoise(src_3, 15);
    cv::Mat noise_image_15_4 = addNoise(src_4, 15);
    cv::Mat noise_image_15 = gluing_l_r(noise_image_15_1, noise_image_15_2, noise_image_15_3, noise_image_15_4);
    //cv::imshow("Image with noise 3", noise_image_15);

    //Построение гистограммы c шумом cо среднеквадратическим значением 3
    cv::Mat histImage_noise_15_1 = histogramm(noise_image_15_1);
    cv::Mat histImage_noise_15_2 = histogramm(noise_image_15_2);
    cv::Mat histImage_noise_15_3 = histogramm(noise_image_15_3);
    cv::Mat histImage_noise_15_4 = histogramm(noise_image_15_4);
    cv::Mat histImage_noise_15 = gluing_l_r(histImage_noise_15_1, histImage_noise_15_2, histImage_noise_15_3, histImage_noise_15_4);
    //cv::imshow("Histogram with noise 15", histImage_noise_15);

    cv::Mat result = gluing_u_d(src, histImage, noise_image_3, histImage_noise_3, noise_image_7, histImage_noise_7, noise_image_15, histImage_noise_15);
    cv::resize(result, result, cv::Size(result.cols/3, result.rows/3));
    cv::imshow("RESULT", result);
	cv::waitKey();
}