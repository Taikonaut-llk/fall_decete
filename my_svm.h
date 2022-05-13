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

	void trainSVM();							//ѵ������
	bool predictSVM(vector<double>& datav, const std::string& ModelFileName);	//��������Ԥ��
	//void multi_predictSVM(vector<bool>& v);							//һ������Ԥ��

private:
	svm_node* vector2svmnode(const vector<double>& v);						//��vector<double>ת��Ϊsvm_node[]
	void setParam();
	void readTrainData(std::string type, bool tag);	//�� path\typename ��st�����ݿ�ʼ��ȡnum�����ݣ�����¼��Ӧ��ǩΪtag��������Ϊtrue��������Ϊfalse��

private:
	svm_parameter param;
	svm_problem prob;					//all the data to train.
	std::deque<svm_node*> datas;		//fatures of all the sample
	std::deque<double> tags;			//type of all the sample
	int SampleNum;
};