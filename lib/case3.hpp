//
//  case3.hpp
//  OpencvCases
//
//  Created by 叶琰2018 on 2020/10/29.
//  Copyright © 2020 yeyan_2018. All rights reserved.
//

#ifndef case3_hpp
#define case3_hpp

#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

/*
 这里第三个案例：提取需要的目标，去掉干扰项（只想提取目标圆）
 思路：二值分割 + 心态学处理 + 计算连通区域并标记 + 横纵比计算
*/

class Case3DetectCircle{
private:
    Mat src,bin_src,dst;
    string img_path;
public:
    // 构造函数
    Case3DetectCircle(string path):img_path(path){}
    void main_process();
    Mat get_src_img();
};

#endif /* case3_hpp */
