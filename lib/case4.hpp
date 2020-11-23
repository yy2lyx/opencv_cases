//
//  case4.hpp
//  OpencvCases
//
//  Created by 叶琰2018 on 2020/11/1.
//  Copyright © 2020 yeyan_2018. All rights reserved.
//

#ifndef case4_hpp
#define case4_hpp

#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

/*
真实案例：农业领域通常需要拍照的自动计数，提供效率，降低成本
思路：二值化分割（三角阈值），形态学处理(看是否需要将小的块进行去掉)，距离分割（在每个距离的最高处形成一个高亮山峰的峰顶），二值化处理（将每个小山头分割出来了，轮廓的比较得到每个小山头），连通区域计数
*/

class Case4ObjectCount{
private:
    Mat src,gray_src,binary_img,dist,dist_8u;
    string img_path;
public:
    // 构造函数
    Case4ObjectCount(string path):img_path(path){}
    void main_process();
    Mat get_src_img();
};

#endif /* case4_hpp */
