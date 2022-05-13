#include"pose.h"

auto init_numpy() {
	import_array();
}

vector<float> pose_xy(Mat draw)
{
	vector<float> pose_point;
	Py_SetPythonHome(L"D:\\anaconda");

	Py_Initialize();//使用python之前，要调用Py_Initialize();这个函数进行初始化
	init_numpy();


	PyRun_SimpleString("import sys");
	//PyRun_SimpleString("import os");
	PyRun_SimpleString("sys.path.append('./')");//这一步很重要，修改Python路径./
	//PyRun_SimpleString("sys.path.append('D:\VS2019doc\MoreBranc')");//这一步很重要，修改Python路径./

	//PyRun_SimpleString("print (os.getcwd())");

	if (!Py_IsInitialized())
	{
		printf("初始化失败！");
		//return false;
	}
	else {
		
		PyObject* pModule = NULL;//声明变量
		PyObject* pFunc = NULL;// 声明变量

		//pModule = PyImport_ImportModule("my_position");//这里是要调用的文件名hello.py
		pModule = PyImport_Import(PyUnicode_FromString("my_position"));
		if (pModule == NULL)
		{
			cout << "没找到该Python文件" << endl;
		}
		else {
			//pDict = PyModule_GetDict(pModule);
			//if (!pDict)
			//{
			//	return -1;
			//}
			pFunc = PyObject_GetAttrString(pModule, "my_funtion");//这里是要调用的函数名
			if (!pFunc || !PyCallable_Check(pFunc))
			{
				printf("can't find function [fun]");
				//return -1;
			}

			PyObject* ArgList = PyTuple_New(1);
			auto sz = draw.size();
			int x = sz.width;
			int y = sz.height;
			int z = draw.channels();
			uchar* CArrays = new uchar[x * y * z];//这一行申请的内存需要释放指针，否则存在内存泄漏的问题
			int iChannels = draw.channels();
			int iRows = draw.rows;
			int iCols = draw.cols * iChannels;
			if (draw.isContinuous())
			{
				iCols *= iRows;
				iRows = 1;
			}

			//uchar* p=NULL;
			int id = -1;
			for (int i = 0; i < iRows; i++)
			{
				// get the pointer to the ith row
			   // p = img.ptr<uchar>(i);
				// operates on each pixel
				for (int j = 0; j < iCols; j++)
				{
					CArrays[++id] = draw.ptr<uchar>(i)[j];//连续空间
				}
			}

			npy_intp Dims[3] = { y, x, z }; //注意这个维度数据！
			  
			PyObject* PyArray = PyArray_SimpleNewFromData(3, Dims, NPY_UBYTE, CArrays);

			PyTuple_SetItem(ArgList, 0, PyArray);


			PyObject* pReturn = PyObject_CallObject(pFunc, ArgList);
			if (pReturn == NULL) return pose_point;

			if (PyList_Check(pReturn)) {
				//printf("返回的结果result： ");
				int SizeOfList = PyList_Size(pReturn);//List对象的大小
				//cout << SizeOfList << endl;
				for (int j = 0; j < SizeOfList; j += 2) {
					PyObject* Itemx = PyList_GetItem(pReturn, j);//获取List对象中的每一个元素
					double resultx = 0.0;
					PyArg_Parse(Itemx, "d", &resultx);
					//printf("%lf ", resultx);
					//需要将关键点画到图片上
					PyObject* Itemy = PyList_GetItem(pReturn, j + 1);//获取List对象中的每一个元素
					double resulty = 0.0;
					PyArg_Parse(Itemy, "d", &resulty);
					
					pose_point.push_back(resultx);
					pose_point.push_back(resulty);
					//cout << "这是MP的第"<< j << "个坐标点" << resultx << "  " << resulty << endl;
				}
			}
			else {
				cout << "Not a List" << endl;
			}

			delete[]CArrays;
			CArrays = nullptr;
		}
		
		Py_DECREF(pFunc);
		Py_DECREF(pModule);
		
	}
	return pose_point;
}

