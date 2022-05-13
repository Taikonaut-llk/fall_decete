#include<iostream>
#include<opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class VALUE{
public:
	vector<vector<double> > get(Mat frame, Mat uniont);
private:
	int blue_three = 0, blue_w = 0, blue_h = 0, Hu_test = 0; // 蓝色框锁定计数三次与蓝色框坐标
	Mat pre;
};
