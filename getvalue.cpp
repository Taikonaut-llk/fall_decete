#include "getvalue.h"

double Distance(int x1, int y1, int x2, int y2) {
	int d_x = x1 - x2;
	int d_y = y1 - y2;
	double distance = sqrt(d_x * d_x + d_y * d_y);
	return distance;
}


vector<vector<double> > VALUE::get(Mat frame, Mat uniont) {
	//double* nums = new double[10];
	//nums = (double *)malloc(10);
	
	vector<vector<Point> >contours;
	vector<Vec4i> hierarchy;
	findContours(uniont, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE);
	
	//绘制边界矩阵
	// 先用数组把矩形数据存起来，再根据比较中心点的距离与边的斜边距离判断是否相交了，
	// 如果相交了把相交的另一个矩形做好标记，不要再用它画矩形框了
	vector<double> X, Y, W, H;   // w,h存放的都是W/2，H/2
	int ave_x, ave_y, ave_w, ave_h;
	
	for (int i = 0; i < contours.size(); i++) {
	    //cout << contourArea(contours[i]) << ends << contours[i] << endl;
	    //&& contourArea(contours[i]) > 1500.0 
	    if (contourArea(contours[i]) > 800.0 && arcLength(contours[i], true) > 200.0) {
	
	        Rect rects = boundingRect(contours[i]);
	        int avgX = (rects.x + rects.width / 2); 
	        X.emplace_back(avgX);
	        W.emplace_back(rects.width / 2);
	        int avgY = (rects.y + rects.height / 2);
	        Y.emplace_back(avgY);
	        H.emplace_back(rects.height / 2);
	
	    }
	}
	
	vector<vector<double> >date;
	for (int i = 0; i < X.size(); i++) {
		vector<double> four_data;
	    int num = 0, flag = 0;
	    ave_x = ave_y = ave_w = ave_h = 0;
	
	    if (X[i] == 0) continue;
	    for (int j = i + 1; j < X.size(); j++) {
	        if (Distance(X[i], Y[i], X[j], Y[j]) - sqrt((H[i] + W[j]) * (H[i] + W[j]) / 4 + (H[j] + W[i]) * (H[j] + W[i]) / 4) == 0){
	            num++;
	            ave_x += X[j];
	            X[j] = 0;
	            ave_y += Y[j];
	            ave_w += W[j];
	            ave_h += H[j];
	            flag = 1;
	        } 
	    }
	    // 计算 矩形的平均值
	    num++;
	    ave_x += X[i];  ave_x /= num;
	    ave_y += Y[i];  ave_y /= num;
	    ave_w += W[i];  ave_w /= num;
	    ave_h += H[i];  ave_h /= num;
	
	    double value_w, value_h;
	    if (flag == 1 && Distance(X[i], Y[i], ave_x, ave_y) > 50) {
	
	        rectangle(frame, Point(ave_x - blue_w, ave_y - blue_h), Point(ave_x + blue_w, ave_y + blue_h), (0, 255, 0), 2);
	        value_w = 2 * blue_w;
	        value_h = 2 * blue_h;
	        //cout << 1 << endl;
	    }
	    else {
	        if ((W[i] < blue_w / 2 || H[i] < blue_h / 2) && blue_three < 3) {
	            rectangle(frame, Point(X[i] - blue_w, Y[i] - blue_h), Point(X[i] + blue_w, Y[i] + blue_h), (0 ,0, 255), 5);              
	            blue_three++;
	            value_w = 2 * blue_w;
	            value_h = 2 * blue_h;
	        }
	        else {
	            rectangle(frame, Point(X[i] - W[i], Y[i] - H[i]), Point(X[i] + W[i], Y[i] + H[i]), (0, 0, 255), 2);
	            blue_h = H[i];
	            blue_w = W[i];
	            blue_three = 0;
	            value_w = 2 * W[i];
	            value_h = 2 * H[i];
	        }
	    }
	
	    //得到数据
	  //  Mat ROI = uniont(Rect(X[i] - W[i], Y[i] - H[i], W[i] * 2, H[i] * 2));
	  //  if (Hu_test > 30) {  //相隔30个框的时间，更新上一个hu矩取得的矩形框
	  //      double value = matchShapes(ROI, pre, CONTOURS_MATCH_I1, 0);
			//four_data.push_back(1);
	  //      pre = ROI;
	  //      Hu_test = 0;
	  //      //imshow("pre", pre);
	  //      cout << "Hu矩" << value << ends;
	  //  }
	  //  //imshow("ROI", ROI);
	  //  Hu_test++;
	
	    //a2 = b2 + c2，c2 = a2 - b2，c = √(a2 - b2)，e = c / a = √[(a2 - b2) / a2] = √[1 - (b / a)2]
	    double WvHt = sqrt(1 - ((value_w * value_w) / (value_h * value_h))); // 离心率
		double WvH = value_w / value_h; // 宽高比
		double LvS = pow(2 * (value_w + value_h), 2) / (value_w * value_h);  // 周长平方比 
		//nums[i][1] = WvHt; nums[i][2] = WvH; nums[i][3] = LvS; 
		four_data.push_back(WvHt); four_data.push_back(WvH); four_data.push_back(LvS);
	    //cout << "离心率" << WvHt ;
	    //cout << "宽高比" << WvH <<  "周长平方面积比" << LvS << endl;
		date.push_back(four_data);
		four_data.clear();
	}

	return date;
}

