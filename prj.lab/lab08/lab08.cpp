#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

int main() {
	cv::Mat img = cv::imread("C:/Users/kvale/Desktop/CV/misis2024s-21-02-koda-v-d/prj.lab/lab08/image.jpg");
	cv::imshow("1", img);

	cv::Mat res = cv::Mat::zeros(cv::Size(200, 200), CV_8U);
	cv::rectangle(res, cv::Rect(0, 0, 200, 200), cv::Scalar(255, 255, 255), -1);
	cv::drawMarker(res, cv::Point(100, 100), cv::Scalar(0, 0, 0), cv::MARKER_CROSS, 0, 3);
	cv::line(res, cv::Point(0, 100), cv::Point(200, 100), cv::Scalar(0, 0, 0));
	cv::line(res, cv::Point(100, 0), cv::Point(100, 200), cv::Scalar(0, 0, 0));
	cv::imshow("2", res);

	for (int i = 0; i < img.rows; i++)
		for (int j = 0; j < img.cols; j++) {
			double r = double(img.at<cv::Vec3b>(i, j)[2])/255.0;
			double g = double(img.at<cv::Vec3b>(i, j)[1])/255.0;
			double b= double(img.at<cv::Vec3b>(i, j)[0])/255.0;
			//std::cout << "first  R " << r << " G " << g << " B " << b << std::endl;
			double alpha = (r + g + b) / 3;
			double r_ = r - alpha;
			double g_ = g - alpha;
			double b_ = b - alpha;
			//std::cout << "second  R' " << r_ << " G' " << g_ << " B' " << b_ << std::endl;
			double x = 2 * g_ + b_;
			double y = -2 * r_ - b_;
			double z = -r_ + g_;
			double X = sqrt(x*x + y*y + z*z) / sqrt(1 + 1 + 4);
			x = 2 * b_;
			y = 2 * b_;
			z = -2 * r_ - 2 * g_;
			double Y = sqrt(x * x + y * y + z * z) / sqrt(4 + 4 + 0);
			if ( g_ <= -r_ && g_ >= r_ && b_>=0) {
			}
			else if(g_ <= r_ && g_ <= - r_ && b_ >=0) {
				X = -X;
			}
			else if (g_>= - r_ && g_ <= r_ && b_ <=0) {
				X = -X;
				Y = -Y;
			}
			else if (g_ >= r_ && g >= - r_ && b_ <= 0 ) {
				Y = -Y;
			}
			//std::cout << X << " " << Y << "\n";
			cv::drawMarker(res, cv::Point(X*100 + 100, Y*100 + 100), cv::Scalar(128, 128, 128));

		}

	cv::imshow("RESULT", res);
	cv::waitKey(0);


}