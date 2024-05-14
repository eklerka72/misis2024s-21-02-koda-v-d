/*#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>



int main() {
    cv::Mat image = cv::imread("C:/Users/kvale/Desktop/CV/misis2024s-21-02-koda-v-d/prj.lab/kursovaya/img.jpg");
    imshow("Original Image", image);


    // �������������� � �������� ������
    cv::Mat grayImage;
    cvtColor(image, grayImage, cv::COLOR_BGR2GRAY);

    // ���������� ������� ������ ��� �����������
    cv::Mat blurredImage;
    GaussianBlur(grayImage, blurredImage, cv::Size(5, 5), 0);

    // ����� ������������� �����������
    imshow("Processed Image", blurredImage);

    // ���������� ��������� ��������� ��� ���������� ������� ����� (����� = 30)
    cv::Mat binaryImage;
    threshold(blurredImage, binaryImage, 30, 255, cv::THRESH_BINARY);

    // ����������� ������ � ������� ��������� Canny
    cv::Mat edges;
    Canny(blurredImage, edges, 50, 150);

    // ����� ����������� � ������������� ���������
    cv::imshow("Edges", edges);

    cv::waitKey(0);

    return 0;
}*/

/*
    // �������������� ����������� � ������� ������
    cvtColor(src, detected_edges, COLOR_BGR2GRAY);
    imshow("2", detected_edges);

    // ����������� �����������
    GaussianBlur(detected_edges, detected_edges, Size(3, 3), 0);
    imshow("3", detected_edges);

    // ����������� ������ � �������������� ��������� Canny
    Canny(detected_edges, detected_edges, cannyThreshold, cannyThreshold * 3, kernel_size);
    imshow("4", detected_edges);*/

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>

using namespace cv;
using namespace std;

//Mat src, detected_edges;
int cannyThreshold = 100;
int ratio = 3;
int kernel_size = 3;

// ������� ��� ���������� ����� ���������� �� ������ �������� ����� ������ �������
Scalar findSquareColor(const Mat& image, const vector<Point>& squareContour)
{
    Rect squareRect = boundingRect(squareContour); // ����������� ��������������� �������������� ����������
    Mat roi = image(squareRect); // ��������� ������� �������� (ROI) ��� ����������
    Scalar meanColor = mean(roi); // ������ �������� ����� ������ ROI

    return meanColor;
}

cv::Mat segment(const int COLOR_MIN, const int COLOR_MAX) {
    Mat src = imread("C:/Users/kvale/Desktop/CV/misis2024s-21-02-koda-v-d/prj.cw/cw/img.jpg"); //�������� �����������
    
    //��������� � ������ HSV
    Mat hsv = Mat(src.cols, src.rows, 8, 3); //
    vector<Mat> splitedHsv = vector<Mat>();
    cvtColor(src, hsv, COLOR_BGR2HSV);
    split(hsv, splitedHsv);
    for (int y = 0; y < hsv.cols; y++) {
        for (int x = 0; x < hsv.rows; x++) {

            // �������� HSV-���������� �������
            int H = static_cast<int>(splitedHsv[0].at<uchar>(x, y));        // ���
            int S = static_cast<int>(splitedHsv[1].at<uchar>(x, y));        // �������������
            int V = static_cast<int>(splitedHsv[2].at<uchar>(x, y));        // �������

            //���� ������� ������� ������ ���� ��� �� �������� � �������� ��������, �� ����������� ����� 
            if ((V < 20) || (H < COLOR_MIN) || (H > COLOR_MAX)) {
                src.at<Vec3b>(x, y)[0] = 255;
                src.at<Vec3b>(x, y)[1] = 255;
                src.at<Vec3b>(x, y)[2] = 255;
            }
        }
    }
    Mat tmp;

    //��������������� ��������� ��� �������� ���������� �����.
    int an = 5;
    Mat element = getStructuringElement(MORPH_ELLIPSE, Size(an * 2 + 1, an * 2 + 1), Point(an, an));
    dilate(src, tmp, element);
    erode(tmp, tmp, element);

    //��������� ����������� � ���������� ������
    Mat grayscaleMat;
    cvtColor(tmp, grayscaleMat, COLOR_BGR2GRAY);

    //������ �������� �����
    Mat mask(grayscaleMat.size(), grayscaleMat.type());
    Mat out(src.size(), src.type());
    threshold(grayscaleMat, mask, 200, 255, THRESH_BINARY_INV);

    //��������� ����������� ������������� ������ � ����� ����
    out = Scalar::all(255);
    //�������� ����������� ����������� ����� �����
    src.copyTo(out, mask);
    return out;

}

int main()
{
    /*
    // �������� �����������
    src = imread("C:/Users/kvale/Desktop/CV/misis2024s-21-02-koda-v-d/prj.cw/cw/img.jpg");
    imshow("1", src);
    if (src.empty())
    {
        cout << "Failed to load image" << endl;
        return -1;
    }
    // �������������� � �������� ������
    cv::Mat grayImage;
    cvtColor(src, grayImage, cv::COLOR_BGR2GRAY);

    // ���������� ������� ������ ��� �����������
    cv::Mat blurredImage;
    GaussianBlur(grayImage, blurredImage, cv::Size(5, 5), 0);

    // ����� ������������� �����������
    imshow("Processed Image", blurredImage);

    // ���������� ��������� ��������� ��� ���������� ������� ����� (����� = 30)
    cv::Mat binaryImage;
    threshold(blurredImage, binaryImage, 30, 255, cv::THRESH_BINARY);

    // ����������� ������ � ������� ��������� Canny
    cv::Mat edges;
    Canny(blurredImage, edges, 50, 150);

    // ����� ����������� � ������������� ���������
    cv::imshow("Edges", edges);

    // ����� �������� �� ����������� ������
    vector<vector<Point>> contours;
    findContours(edges, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    // ������� ���� �������� ��� ��������� ������� ����������
    for (const auto& contour : contours)
    {
        // ����������� ������� � �������������� (� ������ ������ ��������������)
        vector<Point> approx;
        approxPolyDP(contour, approx, arcLength(contour, true) * 0.05, true);

        // ��������, ��� ������ ������������ � ��������������
        if (approx.size() == 4 && isContourConvex(approx))
        {
            // ����������� ����� ����������
            Scalar squareColor = findSquareColor(src, approx);

            // ��������� ������� ���������� �� �������� �����������
            polylines(src, approx, true, Scalar(0, 255, 0), 2);

            // ����� ����� ����������
            cout << "Square Color: B = " << squareColor[0] << ", G = " << squareColor[1] << ", R = " << squareColor[2] << endl;
        }
    }

    // ����������� ����������
    imshow("Detected Squares", src);*/


    //�������� ����������� 

    const int GREEN_MIN = 60;
    const int GREEN_MAX = 98;

    const int BLUE_MIN =100;
    const int BLUE_MAX = 127;

    const int YELLOW_MIN = 20;
    const int YELLOW_MAX = 60;

    Mat img = imread("C:/Users/kvale/Desktop/CV/misis2024s-21-02-koda-v-d/prj.cw/cw/img.jpg"); //�������� �����������
    imshow("input", img);
    

    //������� ���
    cv::Mat out_green = segment( GREEN_MIN, GREEN_MAX);
    //imshow("Green segment", out_green);

    cv::Mat out_blue = segment(BLUE_MIN, BLUE_MAX);
    //imshow("Blue segment", out_blue);

    cv::Mat out_red = segment(YELLOW_MIN, YELLOW_MAX);
    cv::imshow("Red segment", out_red);
   
    waitKey(0);

    return 0;
}
