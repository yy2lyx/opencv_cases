//
//  case1.hpp
//  OpencvCases
//
//  Created by 叶琰2018 on 2020/10/24.
//  Copyright © 2020 yeyan_2018. All rights reserved.
//

#ifndef case1_hpp
#define case1_hpp


#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>


using namespace cv;
using namespace std;


/*
其中最后一个参数，userdata，就是用来解救糟糕代码风格的。把回调函数中“不得不使用的全局变量”们，组装到一起，作为userdata传入createTrackbar函数，
就避免了全局变量的使用。组装方式？用一个结构体就可以了；而在回调函数中，则通过类型转化而使用(void*->自定义结构体类型*)
*/
struct UsingData {
    Mat src;
    int threshold_value;
    int max_level;
    const string thread_win;
};

void findROI(int bar_value, void* userdata);

void case1_demo(string img_path);



#endif /* case1_hpp */
