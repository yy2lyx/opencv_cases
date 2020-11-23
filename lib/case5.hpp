//
//  case5.hpp
//  OpencvCases
//
//  Created by 叶琰2018 on 2020/11/14.
//  Copyright © 2020 yeyan_2018. All rights reserved.
//

#ifndef case5_hpp
#define case5_hpp

#include <stdio.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>

// 思路：二值化 + 形态学 + Hough直线检测 + 透视变换
// case5 透视变换：OCR识别的时候往往拍摄的角度存在问题，往往拍摄得到的是不规则的矩形，这里需要对其变成规则的矩形，透视矫正（算法需要4个点的坐标）
using namespace cv;
using namespace std;


class PerspectiveTransform{
private:
    Mat src,gray_src,bin_src;
    int width,height;
    Vec4i top_line,bottom_line,left_line,right_line;
public:
    PerspectiveTransform(string path);
    Mat find_countours();
    Mat find_lines(Mat draw_img);
    void perspective_img(Mat line_img);
    Mat get_src_img(string path);
    void main_process();
};

#endif /* case5_hpp */
