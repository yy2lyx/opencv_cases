//
//  case3.cpp
//  OpencvCases
//
//  Created by 叶琰2018 on 2020/10/29.
//  Copyright © 2020 yeyan_2018. All rights reserved.
//

#include "case3.hpp"

void Case3DetectCircle::main_process(){
    // 读取原始图像
    src = get_src_img();
    // 二值化处理
    threshold(src, bin_src, 0, 255, THRESH_BINARY | THRESH_OTSU);
    imshow("binary", bin_src);
    
    // 形态学闭操作——使得零碎的点变成完整的连通区域
    Mat kernal = getStructuringElement(MORPH_RECT, Size(3,3));
    morphologyEx(bin_src, dst, MORPH_CLOSE, kernal);
    imshow("close", dst);
    
    // 形态学开操作——去掉噪点
    Mat kernal_open = getStructuringElement(MORPH_RECT, Size(3,3));
    morphologyEx(dst, dst, MORPH_OPEN, kernal_open);
    imshow("open", dst);
    
    // 找到轮廓
    vector<vector<Point>> contours;
    vector<Vec4i>hireachy;
    findContours(dst, contours, hireachy, RETR_TREE, CHAIN_APPROX_SIMPLE);
    // 画出所有的外轮廓
    Point cc;
    Mat result_img = Mat::zeros(src.rows, src.cols, CV_8UC3);
    for (size_t t = 0;t < contours.size();t++){
        // 面积过滤
        double area = contourArea(contours[t]);
        if (area < 100) continue;
        
        // 横纵比过滤
        Rect rect = boundingRect(contours[t]);
        float ratio = float(rect.width) / float(rect.height);
        
        if (ratio < 1.1 && ratio > 0.9){
            drawContours(result_img, contours, t, Scalar(0,0,255));
            cout << "Circle area ：" << area << endl;
            cout << "Circle lenth : " << arcLength(contours[t], true) << endl;
            int x = rect.x + rect.width / 2;
            int y = rect.y + rect.height/2;
            cc = Point(x,y);
            circle(result_img, cc, 2, Scalar(0,0,255),2,8,0);
        }
        
    }
    imshow("contours", result_img);
    
    Mat circle_img = src.clone();
    cvtColor(circle_img, circle_img, COLOR_GRAY2BGR);
    circle(circle_img, cc, 2, Scalar(0,0,255),2,8,0);
    imshow("final result", circle_img);
    
    
    waitKey(0);
}

Mat Case3DetectCircle::get_src_img(){
    src = imread(img_path,IMREAD_GRAYSCALE);
    if (src.empty()){
        cout << "Could not load img!" << endl;
    }
    imshow("src", src);
    return src;
}
