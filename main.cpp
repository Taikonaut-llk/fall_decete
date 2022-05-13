#include "Vibe.h"             
#include "getvalue.h"           
#include "pose.h"                
#include<fstream>        
#include "my_svm.h"
#include<iostream>
#include<opencv2/opencv.hpp>
#include<opencv2/imgproc/types_c.h>

using namespace std;
using namespace cv;

vector<double> poseVpoint(vector<float> body);
int main(int argc, char* argv[])  
{  

    //Mat frame, gray, FGModel;
    vector<vector<double> > date; 
    vector<float> pose_point;      
    Mat frame, gray, FGModel, fgMask, uniont;  
    VideoCapture capture;
    capture = VideoCapture(1);  //"D:/QQ/pic/stest.avi"
    if (!capture.isOpened()) {
        capture = VideoCapture(1);
        if (!capture.isOpened())
        {
            capture = VideoCapture(1);
            if (!capture.isOpened())
            {
                cout << "ERROR: Did't find this video!" << endl;
                return 0;
            }
        }
    }

    if (!capture.isOpened())
    {
        cout << "No camera or video input!" << endl;
        return -1;
    }

    // 程序运行时间统计变量
    // the Time Statistical Variable of Program Running Time
    double time;
    double start;
    //start = static_cast<double>(getTickCount());

    Ptr<BackgroundSubtractorMOG2> pBgmodel = createBackgroundSubtractorMOG2();
    pBgmodel->setVarThreshold(20);

    ViBe vibe;
    VALUE Value;
    bool count = true;

    while (1)
    {
        capture >> frame;
        Size dsize = Size(512, 512);
        resize(frame, frame, dsize, INTER_AREA);

        if (frame.empty())
            break;

        // GMM获取前景图像的三颜色，灰白黑
        pBgmodel->apply(frame, fgMask);
        threshold(fgMask, fgMask, 200, 255, THRESH_BINARY);

        cvtColor(frame, gray, CV_RGB2GRAY);
        if (count)
        {
            vibe.init(gray);
            vibe.ProcessFirstFrame(gray);
            //cout << "Training ViBe Success." << endl;
            count = false;
        }
        else
        {
            //start = static_cast<double>(getTickCount());
            //time = ((double)getTickCount() - start) / getTickFrequency() * 1000;
            //cout << "Time of Update ViBe Background: " << time << "ms" << endl << endl;

            //MP获取pose点位坐标   
            Mat draw = frame;        //只能单人标取，可优化。先选出人的可能region，在每隔region进行人物提取
            pose_point = pose_xy(draw);
            int HaveAllPoints = 0;
            for (int i = 0; i < pose_point.size(); i++) {
                if (pose_point[i] < 0 || pose_point[i] > 512) {
                    cout << "图中人物点位没有全" << endl;
                    HaveAllPoints = 1;
                    break;
                }
            }
            if (HaveAllPoints == 1) continue;

            //cout << pose_point.size() << endl;

            vibe.Run(gray);
            FGModel = vibe.getFGModel();
            addWeighted(FGModel, 0.5, fgMask, 0.5, 0, uniont);

            date = Value.get(draw, uniont);
            if (date.size() > 2 || date.size() == 0 || pose_point.size() == 0) continue;
            //    HaveAllPoints = 1;
            //    break;
            //}
            //if (HaveAllPoints == 1) continue;

            //if (date.size() == 2) {
            //    date[0][0] = (date[0][0] + date[1][0]) / 2;
            //    date[0][1] = (date[0][1] + date[1][1]) / 2;
            //    date[0][2] = (date[0][2] + date[1][2]) / 2;
            //}
            vector<double> test;         //检测的数组
            cout << "唯一的(或两个平均下来的)离心率,宽高比,周长平方面积比:";
            for (int j = 0; j < 3; j++) {
                test.push_back(date[0][j]);
                cout << date[0][j] << "  ";   //离心率，宽高比， 周长平方面积比
            }
            cout << endl;
            vector<double> ans = poseVpoint(pose_point);
            for (int i = 0; i < ans.size(); i++) {    //存放MP的11个点，26个数据
                test.push_back(ans[i]);
                cout << i << "的数据是：" << ans[i] << endl;
            }
            cout << endl << endl;

            // 检测
            ClassificationSVM asdf;
            if (asdf.predictSVM(test, "Model.xml")) {
                cout << "yes" << endl;
            }
            else {
                Mat ddd = imread("D:/QQ/pic/ddd.jpg", 1);
                imshow("ddd", ddd);
                waitKey(10);
                destroyWindow("ddd");
                cout << "no" << endl;
            }

            imshow("uniont", uniont);
            //imshow("frame", frame);
            imshow("draw", draw);

            if (waitKey(1) == 27)
                break;
        }
    }
    Py_Finalize();//调用Py_Finalize，这个根Py_Initialize相对应的。
    destroyAllWindows();
    return 0;
}

vector<double> poseVpoint(vector<float> body) {
    vector<double> average;   //身体的十个点，求平均变成五个点
    for (int i = 0; i < body.size(); i += 4) {
        average.push_back((body[i] + body[i + 2]) / 2);
        average.push_back((body[i + 1] + body[i + 3]) / 2);
    }
    vector<double> ans;
    for (int i = 0; i < average.size(); i += 2) {
        for (int j = i + 2; j < average.size(); j += 2) {
            ans.push_back(average[i] / average[j]);
            ans.push_back(average[i + 1] / average[j + 1]);
        }
    }
    return ans;
}