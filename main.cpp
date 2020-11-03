#include <iostream>
#include "case1.h"
#include "case2.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

int main() {
	// case6:��Ƭ����̫����Զ��������ͼ��ϣ��֪��������ܳ�
	string img_path = "D:/yeyan/vs_project/case/case6.png";
	Mat src,gray_src,bin_src;
	src = imread(img_path);
	if (src.empty()) {
		cout << "Could not load img!" << endl;
		return -1;
	}
	imshow("src", src);

	// ��˹�˲�����(������Ե������ģ���ڲ�ϸ�ڣ������ǲ���Ҫ��)�����������˲�����ϸ�ڶ�ģ����
	GaussianBlur(src,src,Size(13,13),0,0);
	//blur(src,src,Size(13,13));
	imshow("gaus", src);

	
	cvtColor(src, gray_src, CV_BGR2GRAY);
	// THRESH_OTSU ��򷨶�ֱ��ͼ�������壬�м������Բ��ȵ�ֱ��ͼ��Ӧͼ���ֵ��Ч���ȽϺã�������ֻ��һ�������ֱ��ͼ��Ӧ��ͼ��ָ�Ч��û��˫��ĺá�
	// THRESH_TRIANGLE �ڵ�����ʱ��ͼ���ֱ��ͼֻ��һ������,���ʱ��ʹ��TRIANGLE����Ѱ����ֵ�ǱȽϺõ�һ��ѡ��
	// �������Ե�ͼ���Ͻ���һ�����壬x��Ӻڵ��ף��м�ĻҶȲ������ص���࣬����жϽ���һ������
	threshold(gray_src,bin_src,0,255,THRESH_BINARY | THRESH_TRIANGLE);
	imshow("bin", bin_src);

	// ��̬ѧ�ղ����������������ڵ�һЩС��϶���ע�����ﲻ�������Ͳ�������ı������Ĵ�С��
	Mat kernel = getStructuringElement(MORPH_RECT,Size(3,3));
	morphologyEx(bin_src,bin_src,MORPH_CLOSE,kernel,Point(-1,-1),2);
	imshow("close", bin_src);

	// ��ȡ��������
	vector<vector<Point>> contours;
	vector<Vec4i> hireachy;

	findContours(bin_src,contours,hireachy,CV_RETR_EXTERNAL,CHAIN_APPROX_SIMPLE);
	Mat clone_img = Mat::zeros(src.size(), CV_8UC3);
	for (size_t t = 0; t < contours.size();t++) {
		// ��������Ĺ���
		Rect rect = boundingRect(contours[t]);
		if (rect.width < src.cols * 0.5 && rect.height < src.rows * 0.5 ) continue;
		if (rect.width > src.cols * 0.9 && rect.width > src.cols * 0.9) continue;
		drawContours(clone_img,contours,t,Scalar(0,0,255));
		// ��ͨ�����������ܳ�
		double area = contourArea(contours[t]);
		double lenth = arcLength(contours[t],true);

		cout << "Area: " << area << "Lenth: " << lenth << endl;
	}
	imshow("contours", clone_img);
	
	waitKey(0);
	return 0;
}

