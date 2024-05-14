#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream> 
#include <vector>
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

cv::Mat World_Grey(const cv::Mat imgIn) {
	//Sum the colour values in each channel
	cv::Scalar sumImg = sum(imgIn);

	//normalise by the number of pixes in the image to obtain an extimate for the illuminant
	cv::Scalar illum = sumImg / (imgIn.rows * imgIn.cols);

	// Split the image into different channels
	std::vector<cv::Mat> rgbChannels(3);

	cv::split(imgIn, rgbChannels);

	//Assign the three colour channels to CV::Mat variables for processing
	cv::Mat redImg = rgbChannels[2];
	cv::Mat graanImg = rgbChannels[1];
	cv::Mat blueImg = rgbChannels[0];

	//calculate scale factor for normalisation you can use 255 instead
	double scale = (illum(0) + illum(1) + illum(2)) / 3;
	redImg = redImg * scale / illum(2);
	graanImg = graanImg * scale / illum(1);
	blueImg = blueImg * scale / illum(0);

	//Assign the processed channels back into vector to use in the cv::merge() function
	rgbChannels[0] = blueImg;
	rgbChannels[1] = graanImg;
	rgbChannels[2] = redImg;

	cv::Mat imgOut; //to hold the output image

	/// Merge the processed colour channels
	merge(rgbChannels, imgOut);

	return imgOut;
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

int main() {
	//Read the image from file(replace the image file name in your code)

	cv::Mat imgIn = cv::imread("C:/Users/kvale/Desktop/CV/misis2024s-21-02-koda-v-d/prj.lab/lab09/img.jpg");
	cv::imshow("Init Img", imgIn);

	cv::Mat hist_IN = histogramm(imgIn);
	cv::imshow("hist Init Img", hist_IN);
	cv::imwrite("C:/Users/kvale/Desktop/CV/misis2024s-21-02-koda-v-d/prj.lab/lab09/hist_IN.jpg", hist_IN);

	//Check if the image is empty
	if (imgIn.empty())
	{
		std::cout << "Error occured when loading the image" << std::endl;
		return -1;
	}
	
	cv::Mat img_GWC = World_Grey(imgIn);
	cv::imshow("World Grey Img", img_GWC);
	cv::imwrite("C:/Users/kvale/Desktop/CV/misis2024s-21-02-koda-v-d/prj.lab/lab09/img_GWC.jpg", img_GWC);

	cv::Mat hist_WGI = histogramm(img_GWC);
	cv::imshow("Hist World Grey Img", hist_WGI);
	cv::imwrite("C:/Users/kvale/Desktop/CV/misis2024s-21-02-koda-v-d/prj.lab/lab09/hist_WGI.jpg", hist_WGI);

	cv::Mat img_CbC = color_img_ch_by_ch(imgIn);
	cv::imshow("Chanel by hanel Img", img_CbC);
	cv::imwrite("C:/Users/kvale/Desktop/CV/misis2024s-21-02-koda-v-d/prj.lab/lab09/img_CbC.jpg", img_CbC);

	cv::Mat hist_CbC = histogramm(img_CbC);
	cv::imshow("Hist Chanel by Chanel Img", hist_CbC);
	cv::imwrite("C:/Users/kvale/Desktop/CV/misis2024s-21-02-koda-v-d/prj.lab/lab09/hist_CbC.jpg", hist_CbC);



	cv::waitKey(0);

	return 0;
}