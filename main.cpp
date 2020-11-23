//
//  main.cpp
//  OpencvCases
//
//  Created by 叶琰2018 on 2020/10/24.
//  Copyright © 2020 yeyan_2018. All rights reserved.
//

#include <iostream>
#include "case1.hpp"
#include "case2.hpp"
#include "case3.hpp"
#include "case4.hpp"
#include "case5.hpp"
#include "case6.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;


int main(int argc,char** argv){
    
//     case1
    string img1_path = "/Users/yeyan2018/xcode_project/C++_project/CasesImg/case1.png";
    case1_demo(img1_path);

//     case2
    string img2_path = "/Users/yeyan2018/xcode_project/C++_project/CasesImg/case2.png";
    Case2DetectLines case2 = Case2DetectLines(img2_path);
    case2.case2process();

//     case3
    string img3_path = "/Users/yeyan2018/xcode_project/C++_project/CasesImg/case3.png";
    Case3DetectCircle case3 = Case3DetectCircle(img3_path);
    case3.main_process();

//     case4
    string img4_path = "/Users/yeyan2018/xcode_project/C++_project/CasesImg/case4.png";
    Case4ObjectCount case4 = Case4ObjectCount(img4_path);
    case4.main_process();
    
    // case5
    string img5_path = "/Users/yeyan2018/xcode_project/C++_project/CasesImg/case5.png";
    PerspectiveTransform case5 = PerspectiveTransform(img5_path);
    case5.main_process();
    
    // case6 : 分水岭算法 —— 扑克牌
    string img6_path = "/Users/yeyan2018/xcode_project/C++_project/CasesImg/case6.png";
    distanceWatershed(img6_path);
}
 
