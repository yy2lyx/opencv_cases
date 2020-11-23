//
//  case4.cpp
//  OpencvCases
//
//  Created by 叶琰2018 on 2020/11/1.
//  Copyright © 2020 yeyan_2018. All rights reserved.
//

#include "case4.hpp"

Mat Case4ObjectCount::get_src_img(){
    src = imread(img_path);
    if (src.empty()){
        cout << "Could not load img!" << endl;
    }
    imshow("src", src);
    return src;
}

void Case4ObjectCount::main_process(){
    src = get_src_img();
    
    cvtColor(src, gray_src, CV_BGR2GRAY);
    
    // 二值分割
    threshold(gray_src, binary_img, 0, 255, THRESH_BINARY | THRESH_TRIANGLE);
    imshow("binary img", binary_img);
    
    /*
    // 形态学操作(希望通过腐蚀来将每个玉米粒分开，重叠部分还是无法分开)
    Mat kernel = getStructuringElement(MORPH_RECT, Size(5,5));
    dilate(binary_img, dst, kernel,Point(-1,-1),10); // 即便调整很大，但是还是无法将中间两个重叠的玉米粒分开
    imshow("dilate img", dst);
    */
    
    // 形态学操作(通过膨胀减少毛糙的边缘)
    Mat kernel = getStructuringElement(MORPH_RECT, Size(3,3));
    dilate(binary_img, binary_img, kernel,Point(-1,-1),1); // 即便调整很大，但是还是无法将中间两个重叠的玉米粒分开
    imshow("dilate img", binary_img);
    
    // 距离变换——分割成不同的小山顶
    bitwise_not(binary_img, binary_img); // 取反
    distanceTransform(binary_img, dist,  CV_DIST_L2, 5);
    // 这里注意需要将像素归一化，不然显示不了像素距离的差别
    normalize(dist, dist,0,1.0,NORM_MINMAX);
    imshow("distance Transform", dist);
    // 二值分割
    dist.convertTo(dist_8u, CV_8U);
    
    //threshold(dist, dist, 0.7, 1.0, THRESH_BINARY);// 这里的效果还是不太好，使用自适应阈值
    adaptiveThreshold(dist_8u, dist_8u, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 85, 0.0);
    imshow("before", dist_8u);
    // 利用膨胀将其断开的地方接起来
    dilate(dist_8u, dist_8u, kernel,Point(-1,-1),1); // 最后参数是使用的次数
    imshow("distance threshold", dist_8u);
    
    // findContours来找到所有的连通域，并计数
    vector<vector<Point>>contours;
    findContours(dist_8u, contours, CV_RETR_EXTERNAL,CHAIN_APPROX_SIMPLE);
    
    // 画出结果
    Mat markers = Mat::zeros(src.rows, src.cols, CV_8UC3);
    RNG rng(12345);
    for (size_t t =0;t < contours.size();t++){
        drawContours(markers, contours, static_cast<int>(t), Scalar(rng.uniform(0, 255),rng.uniform(0, 255),rng.uniform(0,255)),-1,8,Mat());
    }
    imshow("result", markers);
    cout << "The number of coins:" << contours.size() << endl;
     
    waitKey(0);
}

