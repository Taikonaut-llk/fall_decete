#pragma once
#include <iostream>
#include <cstdio>
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

// 每个像素点的样本个数默认值
// the Default Number of pixel's samples
#define DEFAULT_NUM_SAMPLES  20

// #min指数默认值
// the Default Match Number of make pixel as Background
#define DEFAULT_MIN_MATCHES  2

// Sqthere半径默认值
// the Default Radius of pixel value
#define DEFAULT_RADIUS 20

// 子采样概率默认值
// the Default the probability of random sample
#define DEFAULT_RANDOM_SAMPLE 16

class ViBe
{
public:
    ViBe(int num_sam = DEFAULT_NUM_SAMPLES,
        int min_match = DEFAULT_MIN_MATCHES,
        int r = DEFAULT_RADIUS,
        int rand_sam = DEFAULT_RANDOM_SAMPLE);
    ~ViBe(void);

    // 背景模型初始化
    // Init Background Model.
    void init(Mat img);

    // 处理第一帧图像
    // Process First Frame of Video Query
    void ProcessFirstFrame(Mat img);

    // 运行 ViBe 算法，提取前景区域并更新背景模型样本库
    // Run the ViBe Algorithm: Extract Foreground Areas & Update Background Model Sample Library.
    void Run(Mat img);

    // 获取前景模型二值图像
    // get Foreground Model Binary Image.
    Mat getFGModel();

    // 删除样本库
    // Delete Sample Library.
    void deleteSamples();

    // x的邻居点
    // x's neighborhood points
    int c_xoff[9];

    // y的邻居点
    // y's neighborhood points
    int c_yoff[9];

private:
    // 样本库
    // Sample Library, size = img.rows * img.cols *  DEFAULT_NUM_SAMPLES
    unsigned char*** samples;

    // 前景模型二值图像
    // Foreground Model Binary Image
    Mat FGModel;

    // 每个像素点的样本个数
    // Number of pixel's samples
    int num_samples;

    // #min指数
    // Match Number of make pixel as Background
    int num_min_matches;

    // Sqthere半径
    // Radius of pixel value
    int radius;

    // 子采样概率
    // the probability of random sample
    int random_sample;
};