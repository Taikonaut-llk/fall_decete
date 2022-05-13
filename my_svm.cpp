#include "my_svm.h"
#include <fstream>

void ClassificationSVM::setParam()
{
	param.svm_type = C_SVC;
	param.kernel_type = POLY;
	param.degree = 3;
	param.gamma = 0.5;
	param.coef0 = 0;
	param.nu = 0.5;
	param.cache_size = 50;
	param.C = 500;
	param.eps = 1e-3;
	param.p = 0.1;
	param.shrinking = 1;
	param.nr_weight = 0;
	param.weight = NULL;
	param.weight_label = NULL;
}
void readdatas(vector<vector<double>>& datav, std::string type) {
	double temp;
	ifstream file(type, ios::in);
	if (file.is_open() == false) {
		cerr << "Error!" << endl;
		exit(-1);
	}
	//int j = 0;
	while (!file.eof())
	{
		vector<double> rx;
		for (int i = 0; i < 23; i++)
		{
			file >> temp;
			rx.push_back(temp);
			//file.get();//�ָ�����
			//cout << temp << ends;
		}
		//cout << endl;
		datav.push_back(rx);
	}
	datav.pop_back();
	file.close();
}
//��path\type�ڵ�st�����ݿ�ʼ��ȡnum�����ݵ�datas�У�����tags����Ϊtag
void ClassificationSVM::readTrainData(std::string type, bool tag)
{
	vector<vector<double>> datav;
	readdatas(datav, type);
	//data_standard(datav);
	int dim = datav[0].size(); //number of features
	cout << "��������" << datav.size() << "�����������" << dim << endl;

	//��vector�����ݰ���ʽ����datas��tag
	for (int i = 0; i < datav.size(); i++)		//����num������
	{
		svm_node* sample = new svm_node[dim + 1];
		for (int j = 0; j < dim; j++)	//����ÿ�����ݵ�dim������
		{
			sample[j].index = j + 1;		//index��1��ʼ
			sample[j].value = datav[i][j];
		}
		sample[dim].index = -1;			//��ǽ���
		double dtag = tag ? 1 : 0;		//��tagת��Ϊdouble

		datas.push_back(sample);
		tags.push_back(dtag);
		SampleNum++;
	}
}

//ѵ��ģ�Ͳ���ΪModelFileName
void ClassificationSVM::trainSVM()
{
	setParam();
	string type;
	//int st, num;
	//reading positive datas
	cout << "input positive datas(type startindex number):" << endl;
	cin >> type;
	readTrainData(type, true);

	//reading negative datas
	cout << "input negative datas (type startindex number):" << endl;
	cin >> type;
	readTrainData(type, false);

	if (datas.size() != SampleNum || tags.size() != SampleNum)//���ȼ���
	{
		cout << "datas or tags length error." << endl;
		return;
	}
	prob.l = SampleNum;
	prob.x = new svm_node * [prob.l];
	prob.y = new double[prob.l];


	//�����ݶ���svm_problem
	int index = 0;
	while (!datas.empty())
	{
		prob.x[index] = datas.front();
		prob.y[index] = tags.front();
		datas.pop_front();
		tags.pop_front();
		index++;
	}

	cout << "start training..." << endl;
	svm_model* SVMmodel = svm_train(&prob, &param);

	string ModelFileName;
	cout << "input name of svm_model:" << endl;
	cin >> ModelFileName;
	cout << "save model..." << endl;
	svm_save_model(ModelFileName.c_str(), SVMmodel);

	cout << "done!" << endl;
}















//��vector<double>ת��Ϊsvm_node[]
svm_node* ClassificationSVM::vector2svmnode(const vector<double>& v)
{
	int l = v.size();
	svm_node* sn = new svm_node[l + 1];
	int i = 0;
	for (auto d : v)
	{
		sn[i].index = i + 1;	//index��1��ʼ
		sn[i].value = d;
		i++;
	}
	sn[l].index = -1;			//��־����
	return sn;
}
//��������Ԥ��
bool ClassificationSVM::predictSVM(vector<double>& datav, const std::string& ModelFileName)
{
	svm_node* sample = vector2svmnode(datav);
	svm_model* SVMmodel = svm_load_model(ModelFileName.c_str());
	double predictvalue = svm_predict(SVMmodel, sample);		//����ֵΪ1��0
	return predictvalue ? true : false;
}

////һ������Ԥ��
//void ClassificationSVM::multi_predictSVM(vector<bool>& v)
//{
//	string modelname, type;
//	int st, num;
//	cout << "start predicting..." << endl;
//	cout << "input name of svm_model" << endl;
//	cin >> modelname;
//	cout << "input predict datas (type startindex number):" << endl;
//	cin >> type;
//	cin >> st;
//	cin >> num;
//	cout << "predicting..." << endl;
//	vector<vector<double>> datav;
//	readdatas(datav, type, num, st);				//��ȡԤ�����ݵ�datav��
//	vector<bool> judge;
//
//	for (auto v : datav)
//	{
//		data_standard1(v);
//		judge.push_back(predictSVM(v, modelname.c_str()));
//	}
//
//	cout << "classification resluts:" << endl;
//	cout << boolalpha;			//�������true��false������1��0
//	for (auto b : judge)
//	{
//		cout << b << " ";
//	}
//	cout << noboolalpha << endl;
//}
