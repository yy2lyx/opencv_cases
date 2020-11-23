//
//  case2.cpp
//  OpencvCases
//
//  Created by 叶琰2018 on 2020/10/29.
//  Copyright © 2020 yeyan_2018. All rights reserved.
//

#include "case2.hpp"

Mat Case2DetectLines::get_ROI_img() {
    src = imread(img_path, IMREAD_GRAYSCALE);
    if (src.empty()) {
        cout << "Could not open the image!" << endl;
    }

    namedWindow("input", CV_WINDOW_AUTOSIZE);
    imshow("input", src);

    Rect roi = Rect(10, 10, src.cols - 20, src.rows - 20);
    roi_img = src(roi);
    imshow("ROI img", roi_img);
    return roi_img;
}

Mat Case2DetectLines::morphology_lines() {
    // 二值化图像
    threshold(roi_img, binary_img, 0, 255, THRESH_BINARY_INV | THRESH_OTSU); // 这里注意是要将图像变成黑底白字形式
    imshow("binary", binary_img);

    // 形态学操作：直线结构体
    Mat kernel = getStructuringElement(MORPH_RECT, Size(30, 1), Point(-1, -1));
    morphologyEx(binary_img, morph_img, MORPH_OPEN, kernel, Point(-1, -1));
    imshow("morph", morph_img);

    // 膨胀:将断掉的线或者不连续的线连起来
    Mat kernel2 = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1));
    dilate(morph_img, morph_img, kernel2);
    imshow("morph_dilate", morph_img);

    // 利用Hough直线检测
    vector<Vec4i>lines;
    HoughLinesP(morph_img, lines, 1, CV_PI / 180.0, 30, 30.0, 0);
    Mat result_img = roi_img.clone();
    cvtColor(result_img, result_img, COLOR_GRAY2BGR);

    for (size_t t = 0; t < lines.size(); t++) {
        Vec4i ln = lines[t];
        line(result_img, Point(ln[0], ln[1]), Point(ln[2], ln[3]), Scalar(0, 0, 255), 2, 8, 0); // 这里还能拿到直线的坐标（初始点，结束点）
    }
    return result_img;
}

void Case2DetectLines::case2process() {
    roi_img = get_ROI_img();
    Mat result_img = morphology_lines();
    imshow("Final Result", result_img);
    waitKey(0);
}
