//
//  case6.cpp
//  OpencvCases
//
//  Created by 叶琰2018 on 2020/11/23.
//  Copyright © 2020 yeyan_2018. All rights reserved.
//

#include "case6.hpp"

/*
图像锐化目的：
原始图像在平滑处理之后，图像边缘和图像轮廓模糊的情况出现。为了减少这类不利效果的影响，就需要利用图像锐化技术，使图像的边缘变得清晰。图像锐化处理的目的是为了使图像的边缘、
轮廓线以及图像的细节变得清晰，经过平滑的图像变得模糊的根本原因是因为图像受到了平均或积分运算，因此可以对其进行逆运算(如微分运算)就可以使图像变得清晰。

锐化需要注意的：
能够进行锐化处理的图像必须有较高的性噪比，否则锐化后图像性噪比反而更低，从而使得噪声增加的比信号还要多，因此一般是先去除或减轻噪声后再进行锐化处理。
*/

void distanceWatershed(string path) {
    /*
    整个流程：
        1.将白色背景变成黑色-目的是为后面的变换做准备
        2. 使用filter2D与拉普拉斯算子实现图像对比度提高，sharp
        3. 转为二值图像通过threshold
        4. 距离变换
        5. 对距离变换结果进行归一化到[0~1]之间
        6. 使用阈值，再次二值化，得到标记
        7. 腐蚀得到每个Peak - erode
        8.发现轮廓 – findContours
        9. 绘制轮廓- drawContours
        10.分水岭变换 watershed
        11. 对每个分割区域着色输出结果

    */


    Mat img = imread(path);
    if (img.empty()) {
        cout << "Could not load img..." << endl;
    }
    
    namedWindow("ori_img", WINDOW_AUTOSIZE);
    imshow("ori_img", img);
    
    // 1. 背景变成黑色
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

    // 2. 图像锐化，提高边缘信息
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

    // 3.二值图像
    Mat bin_img;
    cvtColor(res_img, res_img, CV_BGR2GRAY);
    threshold(res_img,bin_img,40,255,CV_THRESH_BINARY | CV_THRESH_OTSU);
    imshow("binary img", bin_img);

    // 4. 距离变换
    Mat dist_img;
    distanceTransform(bin_img, dist_img, CV_DIST_L2, 3);
    // 归一化到0到1之间
    normalize(dist_img,dist_img,0,1,NORM_MINMAX);
    imshow("distance_transform", dist_img);

    // 5. 二值腐蚀
    // 重新变成二值图像
    threshold(dist_img,dist_img,0.4,1,THRESH_BINARY);
    imshow("bine_distance", dist_img);
    // 腐蚀拿到每个块(之前连在一起的化)
    Mat dst;
    Mat kernel_ero = Mat::ones(5,5,CV_8UC1);
    erode(dist_img,dst,kernel_ero);
    imshow("after erode", dst);

    // 6. 找到轮廓，并且给每个区域标记
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
    imshow("markers", markers * 10000); // 这里×的系数是为了让我们看清可以给更大

    // 7. 分水岭算法
    watershed(img, markers);
    Mat mark = Mat::zeros(markers.size(), CV_8UC1);
    markers.convertTo(mark, CV_8UC1);
    bitwise_not(mark, mark);
    imshow("watershed marks",mark);

    // 8 .着色划分区域——图像分割

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
