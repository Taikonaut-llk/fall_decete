#pragma once
#include <iostream>
#include <cstdio>
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

// ÿ�����ص����������Ĭ��ֵ
// the Default Number of pixel's samples
#define DEFAULT_NUM_SAMPLES  20

// #minָ��Ĭ��ֵ
// the Default Match Number of make pixel as Background
#define DEFAULT_MIN_MATCHES  2

// Sqthere�뾶Ĭ��ֵ
// the Default Radius of pixel value
#define DEFAULT_RADIUS 20

// �Ӳ�������Ĭ��ֵ
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

    // ����ģ�ͳ�ʼ��
    // Init Background Model.
    void init(Mat img);

    // �����һ֡ͼ��
    // Process First Frame of Video Query
    void ProcessFirstFrame(Mat img);

    // ���� ViBe �㷨����ȡǰ�����򲢸��±���ģ��������
    // Run the ViBe Algorithm: Extract Foreground Areas & Update Background Model Sample Library.
    void Run(Mat img);

    // ��ȡǰ��ģ�Ͷ�ֵͼ��
    // get Foreground Model Binary Image.
    Mat getFGModel();

    // ɾ��������
    // Delete Sample Library.
    void deleteSamples();

    // x���ھӵ�
    // x's neighborhood points
    int c_xoff[9];

    // y���ھӵ�
    // y's neighborhood points
    int c_yoff[9];

private:
    // ������
    // Sample Library, size = img.rows * img.cols *  DEFAULT_NUM_SAMPLES
    unsigned char*** samples;

    // ǰ��ģ�Ͷ�ֵͼ��
    // Foreground Model Binary Image
    Mat FGModel;

    // ÿ�����ص����������
    // Number of pixel's samples
    int num_samples;

    // #minָ��
    // Match Number of make pixel as Background
    int num_min_matches;

    // Sqthere�뾶
    // Radius of pixel value
    int radius;

    // �Ӳ�������
    // the probability of random sample
    int random_sample;
};