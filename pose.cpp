#include"pose.h"

auto init_numpy() {
	import_array();
}

vector<float> pose_xy(Mat draw)
{
	vector<float> pose_point;
	Py_SetPythonHome(L"D:\\anaconda");

	Py_Initialize();//ʹ��python֮ǰ��Ҫ����Py_Initialize();����������г�ʼ��
	init_numpy();


	PyRun_SimpleString("import sys");
	//PyRun_SimpleString("import os");
	PyRun_SimpleString("sys.path.append('./')");//��һ������Ҫ���޸�Python·��./
	//PyRun_SimpleString("sys.path.append('D:\VS2019doc\MoreBranc')");//��һ������Ҫ���޸�Python·��./

	//PyRun_SimpleString("print (os.getcwd())");

	if (!Py_IsInitialized())
	{
		printf("��ʼ��ʧ�ܣ�");
		//return false;
	}
	else {
		
		PyObject* pModule = NULL;//��������
		PyObject* pFunc = NULL;// ��������

		//pModule = PyImport_ImportModule("my_position");//������Ҫ���õ��ļ���hello.py
		pModule = PyImport_Import(PyUnicode_FromString("my_position"));
		if (pModule == NULL)
		{
			cout << "û�ҵ���Python�ļ�" << endl;
		}
		else {
			//pDict = PyModule_GetDict(pModule);
			//if (!pDict)
			//{
			//	return -1;
			//}
			pFunc = PyObject_GetAttrString(pModule, "my_funtion");//������Ҫ���õĺ�����
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
			uchar* CArrays = new uchar[x * y * z];//��һ��������ڴ���Ҫ�ͷ�ָ�룬��������ڴ�й©������
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
					CArrays[++id] = draw.ptr<uchar>(i)[j];//�����ռ�
				}
			}

			npy_intp Dims[3] = { y, x, z }; //ע�����ά�����ݣ�
			  
			PyObject* PyArray = PyArray_SimpleNewFromData(3, Dims, NPY_UBYTE, CArrays);

			PyTuple_SetItem(ArgList, 0, PyArray);


			PyObject* pReturn = PyObject_CallObject(pFunc, ArgList);
			if (pReturn == NULL) return pose_point;

			if (PyList_Check(pReturn)) {
				//printf("���صĽ��result�� ");
				int SizeOfList = PyList_Size(pReturn);//List����Ĵ�С
				//cout << SizeOfList << endl;
				for (int j = 0; j < SizeOfList; j += 2) {
					PyObject* Itemx = PyList_GetItem(pReturn, j);//��ȡList�����е�ÿһ��Ԫ��
					double resultx = 0.0;
					PyArg_Parse(Itemx, "d", &resultx);
					//printf("%lf ", resultx);
					//��Ҫ���ؼ��㻭��ͼƬ��
					PyObject* Itemy = PyList_GetItem(pReturn, j + 1);//��ȡList�����е�ÿһ��Ԫ��
					double resulty = 0.0;
					PyArg_Parse(Itemy, "d", &resulty);
					
					pose_point.push_back(resultx);
					pose_point.push_back(resulty);
					//cout << "����MP�ĵ�"<< j << "�������" << resultx << "  " << resulty << endl;
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

