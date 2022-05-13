#include "svm.h"
//#include "readdata.h"
//#include "data_standard.h"
#include <iostream>
#include <string>
#include <vector>
#include <deque>

using namespace std;

class ClassificationSVM
{
public:
	ClassificationSVM() :SampleNum(0) {};
	~ClassificationSVM() = default;

	void trainSVM();							//训练数据
	bool predictSVM(vector<double>& datav, const std::string& ModelFileName);	//单个数据预测
	//void multi_predictSVM(vector<bool>& v);							//一组数据预测

private:
	svm_node* vector2svmnode(const vector<double>& v);						//将vector<double>转换为svm_node[]
	void setParam();
	void readTrainData(std::string type, bool tag);	//从 path\typename 第st个数据开始读取num个数据，并记录对应标签为tag（正样本为true，负样本为false）

private:
	svm_parameter param;
	svm_problem prob;					//all the data to train.
	std::deque<svm_node*> datas;		//fatures of all the sample
	std::deque<double> tags;			//type of all the sample
	int SampleNum;
};