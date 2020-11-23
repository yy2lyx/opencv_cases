//
//  case5.cpp
//  OpencvCases
//
//  Created by 叶琰2018 on 2020/11/14.
//  Copyright © 2020 yeyan_2018. All rights reserved.
//

#include "case5.hpp"

Mat PerspectiveTransform::get_src_img(string path){
    src = imread(path);
    if (src.empty()){
        cout << "Could not load img!" << endl;
    }
    imshow("src", src);
    return src;
    
}

PerspectiveTransform::PerspectiveTransform(string path){
    src = get_src_img(path);
    width = src.cols;
    height = src.rows;
}

Mat PerspectiveTransform::find_countours(){
    // 灰度化
    cvtColor(src, gray_src, CV_BGR2GRAY);
    // 二值化
    threshold(gray_src, bin_src, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);
    imshow("binary", bin_src);
    // 形态学练成一个整体轮廓(把外面轮廓空的地方填补，且不改变图像大小情况——闭操作)
    Mat kernel = getStructuringElement(MORPH_RECT, Size(3,3));
    morphologyEx(bin_src, bin_src, MORPH_CLOSE, kernel,Point(-1,-1),1);
    imshow("dilate", bin_src);
    
    // 通过findContours 发现轮廓
    vector<vector<Point>>contours;
    vector<Vec4i>hireachy;
    findContours(bin_src, contours, hireachy, CV_RETR_TREE, CHAIN_APPROX_SIMPLE);
    
    // 轮廓绘制
    Mat draw_img = Mat::zeros(src.rows, src.cols, CV_8UC3);
    for (size_t t =0;t < contours.size();t++){
        Rect rect = boundingRect(contours[t]);
        // 轮廓过滤
        if (rect.width > width*0.5 && rect.height > height * 0.5 && rect.width < width - 4 && rect.height < height - 4){
            drawContours(draw_img, contours, t, Scalar(0,0,255));
            
        }
    }
    imshow("contours", draw_img);
    return draw_img;
}


Mat PerspectiveTransform::find_lines(Mat draw_img){
    /*
     Hough直线检测P的参数：
     需要处理的图像，需要是灰度图。
     线段的几何表示rho和theta，一般分别取1和np.pi/180。
     阈值。低于该阈值的直线会被忽略。Hough变换可以理解为投票箱和投票数之间的关系，每一个投票箱代表一个直线，投票数达到阈值的直线会被保留，其他的会被删除。
     最小直线长度。
     最大线段间隙。
     */
    vector<Vec4i>lines;
    Mat contour_img;
    int accu = min(width*0.1,height * 0.1);
    // Hough直线检测
    cvtColor(draw_img, contour_img, CV_BGR2GRAY);
    HoughLinesP(contour_img, lines, 1, CV_PI/180, accu,accu,20);
    
    Mat line_img = Mat::zeros(src.rows, src.cols, CV_8UC3);
    for (size_t t = 0;t<lines.size();t++){
        Vec4i ln = lines[t];
        line(line_img, Point(ln[0],ln[1]), Point(ln[2],ln[3]), Scalar(0,0,255));
    }
    // 这里必须至少要>= 4条直线，且上下左右至少存在一条
    cout  <<  "Total lines :" << lines.size() << endl;
    imshow("line_hough", line_img);
    
    // 定位和寻找上下左右四条直线(每个只需要找到一条即可)
    
    for (int i = 0; i < lines.size();i++){
        Vec4i ln = lines[i];
        if (ln[1] < height*0.5 && ln[3]<height*0.5 && abs(ln[3] - ln[1]) < accu -1){
            top_line = lines[i];
        }
        else if (ln[1] > height*0.5 && ln[3]>height*0.5 && abs(ln[3] - ln[1]) < accu -1){
            bottom_line = lines[i];
        }
        else if (ln[0] < width*0.5 && ln[2]<width*0.5 ){
            left_line = lines[i];
        }
        else if (ln[0] > width*0.5 && ln[2]>width*0.5 ){
            right_line = lines[i];
        }
        
    }
    
    cout << "src.width:" << width << endl;;
    cout << "src.height:" << height << endl;
    cout << "Top line : p1(x,y)=" << top_line[0]<< "," << top_line[1] << "p2(x,y)=" << top_line[2]<< "," << top_line[3] << endl;
    cout << "Bottom line : p1(x,y)=" << bottom_line[0]<< "," << bottom_line[1] << "p2(x,y)=" << bottom_line[2]<< "," << bottom_line[3] << endl;
    cout << "Left line : p1(x,y)=" << left_line[0]<< "," << left_line[1] << "p2(x,y)=" << left_line[2]<< "," << left_line[3] << endl;
    cout << "Right line : p1(x,y)=" << right_line[0]<< "," << right_line[1] << "p2(x,y)=" << right_line[2]<< "," << right_line[3] << endl;
    return line_img;
}

void PerspectiveTransform::perspective_img(Mat line_img){
    // 拟合四条直线方程
    float k1,b1;
    k1 = float(top_line[3] - top_line[1]) / float(top_line[2] - top_line[0]);
    b1 = top_line[1] - k1*top_line[0];
    float k2,b2;
    k2 = float(bottom_line[3] - bottom_line[1]) / float(bottom_line[2] - bottom_line[0]);
    b2 = bottom_line[1] - k2*bottom_line[0];
    float k3,b3;
    k3 = float(left_line[3] - left_line[1]) / float(left_line[2] - left_line[0]);
    b3 = left_line[1] - k3*left_line[0];
    float k4,b4;
    k4 = float(right_line[3] - right_line[1]) / float(right_line[2] - right_line[0]);
    b4 = right_line[1] - k4*right_line[0];
    
    
    //找到四条直线的交点
    Point p1; // 左上角
    p1.x = static_cast<int>((b3-b1)/(k1-k3));
    p1.y = static_cast<int>((k1*p1.x + b1));
    
    Point p2; // 左下角
    p2.x = static_cast<int>((b3-b2)/(k2-k3));
    p2.y = static_cast<int>((k2*p2.x + b2));
    
    Point p3; // 右上角
    p3.x = static_cast<int>((b4-b1)/(k1-k4));
    p3.y = static_cast<int>((k1*p3.x + b1));
    
    Point p4; // 右下角
    p4.x = static_cast<int>((b4-b2)/(k2-k4));
    p4.y = static_cast<int>((k2*p4.x + b2));
    
    cout << "p1(x,y) = " << p1.x << "," << p1.y << endl;
    cout << "p2(x,y) = " << p2.x << "," << p2.y << endl;
    cout << "p3(x,y) = " << p3.x << "," << p3.y << endl;
    cout << "p4(x,y) = " << p4.x << "," << p4.y << endl;
    
    // 画出交点
    circle(line_img, p1, 2, Scalar(0,255,0),2,8,0);
    circle(line_img, p2, 2, Scalar(0,255,0),2,8,0);
    circle(line_img, p3, 2, Scalar(0,255,0),2,8,0);
    circle(line_img, p4, 2, Scalar(0,255,0),2,8,0);
    imshow("line img", line_img);
    
    // 透视变换(注意这里需要把src和dst的每个点进行对应)
    vector<Point2f>src_corners(4);
    src_corners[0] = p1;
    src_corners[1] = p2;
    src_corners[2] = p3;
    src_corners[3] = p4;
    
    vector<Point2f> dst_coners(4);
    dst_coners[0] = Point(0,0);
    dst_coners[1] = Point(0,height);
    dst_coners[2] = Point(width,0);
    dst_coners[3] = Point(width,height);
    
    Mat result_img;
    Mat warpmatrix = getPerspectiveTransform(src_corners, dst_coners);
    warpPerspective(src, result_img, warpmatrix, result_img.size(),INTER_LINEAR);
    namedWindow("result",WINDOW_AUTOSIZE);
    imshow("result", result_img);
    waitKey(0);
}

void PerspectiveTransform::main_process(){
    Mat draw_img = find_countours();
    Mat line_img = find_lines(draw_img);
    perspective_img(line_img);
}
