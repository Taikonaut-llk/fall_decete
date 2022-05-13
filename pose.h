#pragma once
#include <Python.h>
#include "object.h"
#include <numpy/arrayobject.h>
#include<iostream>
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;

vector<float> pose_xy(Mat draw);