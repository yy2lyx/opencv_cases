# include "distance_watershed.h"

/*
ͼ����Ŀ�ģ�
ԭʼͼ����ƽ������֮��ͼ���Ե��ͼ������ģ����������֡�Ϊ�˼������಻��Ч����Ӱ�죬����Ҫ����ͼ���񻯼�����ʹͼ��ı�Ե���������ͼ���񻯴����Ŀ����Ϊ��ʹͼ��ı�Ե��
�������Լ�ͼ���ϸ�ڱ������������ƽ����ͼ����ģ���ĸ���ԭ������Ϊͼ���ܵ���ƽ����������㣬��˿��Զ������������(��΢������)�Ϳ���ʹͼ����������

����Ҫע��ģ�
�ܹ������񻯴����ͼ������нϸߵ�����ȣ������񻯺�ͼ������ȷ������ͣ��Ӷ�ʹ���������ӵı��źŻ�Ҫ�࣬���һ������ȥ��������������ٽ����񻯴���
*/

void distanceWatershed() {
	/*
	�������̣�
		1.����ɫ������ɺ�ɫ-Ŀ����Ϊ����ı任��׼��
		2. ʹ��filter2D��������˹����ʵ��ͼ��Աȶ���ߣ�sharp
		3. תΪ��ֵͼ��ͨ��threshold
		4. ����任
		5. �Ծ���任������й�һ����[0~1]֮��
		6. ʹ����ֵ���ٴζ�ֵ�����õ����
		7. ��ʴ�õ�ÿ��Peak - erode
		8.�������� �C findContours
		9. ��������- drawContours
		10.��ˮ��任 watershed
		11. ��ÿ���ָ�������ɫ������

	*/


	Mat img = imread("D:/yeyan/vs_project/poker.png");
	if (img.empty()) {
		cout << "Could not load img..." << endl;
	}
	
	namedWindow("ori_img", WINDOW_AUTOSIZE);
	imshow("ori_img", img);
	
	// 1. ������ɺ�ɫ
	for (int row = 0; row < img.rows;row++) {
		for (int col = 0; col < img.cols;col++) {
			if (img.at<Vec3b>(row,col) == Vec3b(255,255,255)) {
				img.at<Vec3b>(row, col)[0] = 0;
				img.at<Vec3b>(row, col)[1] = 0;
				img.at<Vec3b>(row, col)[2] = 0;
			}
		}
	}
	imshow("change background",img);

	// 2. ͼ���񻯣���߱�Ե��Ϣ
	// create kernel
	Mat kernel = (Mat_<float>(3, 3) << 1, 1, 1, 1, -8, 1, 1, 1, 1);
	// make it more sharp
	Mat lap_img;
	Mat sharp = img;
	filter2D(sharp,lap_img,CV_32F,kernel,Point(-1,-1),0);
	img.convertTo(sharp, CV_32F);
	Mat res_img = sharp - lap_img;
	// show
	res_img.convertTo(res_img, CV_8UC3);
	lap_img.convertTo(lap_img, CV_8UC3);
	imshow("laplace",lap_img);
	imshow("sharp_result",res_img);

	// 3.��ֵͼ��
	Mat bin_img;
	cvtColor(res_img, res_img, CV_BGR2GRAY);
	threshold(res_img,bin_img,40,255,CV_THRESH_BINARY | CV_THRESH_OTSU);
	imshow("binary img", bin_img);

	// 4. ����任
	Mat dist_img;
	distanceTransform(bin_img, dist_img, CV_DIST_L2, 3);
	// ��һ����0��1֮��
	normalize(dist_img,dist_img,0,1,NORM_MINMAX);
	imshow("distance_transform", dist_img);

	// 5. ��ֵ��ʴ
	// ���±�ɶ�ֵͼ��
	threshold(dist_img,dist_img,0.4,1,THRESH_BINARY);
	imshow("bine_distance", dist_img);
	// ��ʴ�õ�ÿ����(֮ǰ����һ��Ļ�)
	Mat dst;
	Mat kernel_ero = Mat::ones(5,5,CV_8UC1);
	erode(dist_img,dst,kernel_ero);
	imshow("after erode", dst);

	// 6. �ҵ����������Ҹ�ÿ��������
	Mat dst_8u;
	dst.convertTo(dst_8u, CV_8U);
	// findcontours;
	vector<vector<Point>> contours;
	findContours(dst_8u, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
	// create marks
	Mat markers = Mat::zeros(dst.size(), CV_32SC1);
	// draw marks
	for (size_t i = 0; i < contours.size();i++) {
		drawContours(markers, contours, static_cast<int>(i), Scalar::all(static_cast<int>(i) + 1));
	}
	// draw background circle
	circle(markers,Point(5,5),3,CV_RGB(255,255,255),-1);
	imshow("markers", markers * 10000); // �������ϵ����Ϊ�������ǿ�����Ը�����

	// 7. ��ˮ���㷨
	watershed(img, markers);
	Mat mark = Mat::zeros(markers.size(), CV_8UC1);
	markers.convertTo(mark, CV_8UC1);
	bitwise_not(mark, mark);
	imshow("watershed marks",mark);

	// 8 .��ɫ�������򡪡�ͼ��ָ�

	// generate color
	vector<Vec3b> colors;
	for (size_t i = 0; i < contours.size(); i++) {
		int r = theRNG().uniform(0, 255);
		int g = theRNG().uniform(0,255);
		int b = theRNG().uniform(0, 255);
		colors.push_back(Vec3b((uchar)b, (uchar)g, (uchar)r));
	}


	// fill with color and display final result
	Mat dst_result = Mat::zeros(markers.size(), CV_8UC3);
	for (int row = 0; row < markers.rows;row++) {
		for (int col = 0; col < markers.cols;col++) {
			int index = markers.at<int>(row, col);
			if (index > 0 && index <= static_cast<int>(contours.size())) {
				dst_result.at<Vec3b>(row, col) = colors[index - 1];
			}
			else
			{
				dst_result.at<Vec3b>(row, col) = Vec3b(0, 0, 0);
			}
		}
	}
	imshow("final result", dst_result);

	waitKey(0);
}