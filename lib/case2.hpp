//
//  case2.hpp
//  OpencvCases
//
//  Created by 叶琰2018 on 2020/10/29.
//  Copyright © 2020 yeyan_2018. All rights reserved.
//

#ifndef case2_hpp
#define case2_hpp

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

/*
case2:寻找英文试卷填空题的下划线，对后期的切圆和自动识别都比较重要

方案一：利用canny算子找到边缘，直接用hough直线找到直线 ： 不行
方案二：构建二值图像，利用形态学操作找到直线，利用霍夫获取位置信息和显示
*/
class Case2DetectLines
{
private:
    string img_path;
    Mat src,roi_img, binary_img, morph_img;
public:
    // 构造函数
    Case2DetectLines(string path) :img_path(path) {}
    Mat get_ROI_img();
    Mat morphology_lines();
    void case2process();
};

#endif /* case2_hpp */
