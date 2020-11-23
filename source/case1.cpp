//
//  case1.cpp
//  OpencvCases
//
//  Created by 叶琰2018 on 2020/10/24.
//  Copyright © 2020 yeyan_2018. All rights reserved.
//

#include "case1.hpp"

void case1_demo(string img_path) {
    // 真实案例1：扫描仪扫描法律文件，需要切边，去掉边缘的空白。

    // 思路1：通过边缘检测 + 轮廓发现或者直线检测最大外接矩形实现
    // 思路2：通过二值分割 + 形态学方法 + Hough直线找到最大外接矩形
    // 思路2存在的问题：这里必须是扫描文件必须是正着放的，不是斜着放的，才能通过Hough直线检测找到边。


    Mat src = imread(img_path);
    if (src.empty()) {
        cout << "Could not load img!" << endl;
    }
    imshow("src", src);

    // 将全局变量封装到结构体中
    struct UsingData use_data = { src,100,255,"Thread window" };

    // 通过trackbar找到最优的二值阈值
    namedWindow(use_data.thread_win, CV_WINDOW_AUTOSIZE);

    createTrackbar("Threshold:", use_data.thread_win, &use_data.threshold_value, use_data.max_level, findROI, &use_data);

    waitKey(0);
}


void findROI(int bar_value, void* userdata) {
    cout << "bar_val is:" << bar_value << endl; //bar_val就是createTrackbar调用时的value参数
    UsingData use_data = *(UsingData*)(userdata);

    Mat gray_src;
    cvtColor(use_data.src, gray_src, CV_BGR2GRAY);
    Mat canny;
    Canny(gray_src, canny, use_data.threshold_value, use_data.threshold_value * 2, 3, false);

    vector<vector<Point>>contours;
    vector<Vec4i>hireachy;
    findContours(canny, contours, hireachy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

    int minw = use_data.src.cols * 0.75;
    int minh = use_data.src.rows * 0.75;
    RNG rng(12345); // 随机数
    Mat drawImage = Mat::zeros(use_data.src.size(), CV_8UC3);
    Rect bbox;


    for (size_t i = 0; i < contours.size(); i++) {
        // 画轮廓
        RotatedRect minRect = minAreaRect(contours[i]);
        float degree = abs(minRect.angle);
        cout << "current angle:" << endl;
        if (minRect.size.width > minw && minRect.size.height > minh && minRect.size.width < (use_data.src.cols - 5)) {
            Point2f pts[4];
            minRect.points(pts);
            bbox = minRect.boundingRect();

            Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
            for (int i = 0; i < 4; i++) {
                line(drawImage, pts[i], pts[(i + 1) % 4], color, 2, 8, 0);

            }
        }
    }
    imshow(use_data.thread_win, drawImage);
    if (bbox.width > 0 && bbox.height > 0) {
        Mat roiImg = use_data.src(bbox);
        imshow("boundingbox", roiImg);
    }

}
