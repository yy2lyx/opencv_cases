#include <iostream>
#include "case1.h"
#include "case2.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

int main() {
	// case6:照片来自太空望远镜的星云图像，希望知道面积和周长
	string img_path = "D:/yeyan/vs_project/case/case6.png";
	Mat src,gray_src,bin_src;
	src = imread(img_path);
	if (src.empty()) {
		cout << "Could not load img!" << endl;
		return -1;
	}
	imshow("src", src);

	// 高斯滤波降噪(保留边缘轮廓，模糊内部细节，这里是不想要的)，而其他的滤波器把细节都模糊了
	GaussianBlur(src,src,Size(13,13),0,0);
	//blur(src,src,Size(13,13));
	imshow("gaus", src);

	
	cvtColor(src, gray_src, CV_BGR2GRAY);
	// THRESH_OTSU 大津法对直方图有两个峰，中间有明显波谷的直方图对应图像二值化效果比较好，而对于只有一个单峰的直方图对应的图像分割效果没有双峰的好。
	// THRESH_TRIANGLE 在但是有时候图像的直方图只有一个波峰,这个时候使用TRIANGLE方法寻找阈值是比较好的一个选择。
	// 这里明显的图像上仅有一个波峰，x轴从黑到白，中间的灰度部分像素点最多，因此判断仅有一个波峰
	threshold(gray_src,bin_src,0,255,THRESH_BINARY | THRESH_TRIANGLE);
	imshow("bin", bin_src);

	// 形态学闭操作：将最大的轮廓内的一些小空隙填补，注意这里不能用膨胀操作，会改变轮廓的大小。
	Mat kernel = getStructuringElement(MORPH_RECT,Size(3,3));
	morphologyEx(bin_src,bin_src,MORPH_CLOSE,kernel,Point(-1,-1),2);
	imshow("close", bin_src);

	// 获取最大的轮廓
	vector<vector<Point>> contours;
	vector<Vec4i> hireachy;

	findContours(bin_src,contours,hireachy,CV_RETR_EXTERNAL,CHAIN_APPROX_SIMPLE);
	Mat clone_img = Mat::zeros(src.size(), CV_8UC3);
	for (size_t t = 0; t < contours.size();t++) {
		// 最大轮廓的过滤
		Rect rect = boundingRect(contours[t]);
		if (rect.width < src.cols * 0.5 && rect.height < src.rows * 0.5 ) continue;
		if (rect.width > src.cols * 0.9 && rect.width > src.cols * 0.9) continue;
		drawContours(clone_img,contours,t,Scalar(0,0,255));
		// 连通区域的面积和周长
		double area = contourArea(contours[t]);
		double lenth = arcLength(contours[t],true);

		cout << "Area: " << area << "Lenth: " << lenth << endl;
	}
	imshow("contours", clone_img);
	
	waitKey(0);
	return 0;
}

