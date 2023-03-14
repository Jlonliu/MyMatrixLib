#pragma once
#include<vector>
#include<iostream>
using namespace std;

//typedef unsigned __int64 size_t;
typedef unsigned __int64 uint;
typedef vector<double> v;
typedef vector<vector<double>> vv;

class Matrix {
private:
	//��������е�����Ĭ��Ϊ������
	size_t rows_ = 0; //��������(����ά��)
	size_t cols_ = 0;//��������������������
	bool empty_ = true;//�վ���
	bool propertyinit_ = true;//��������Ĭ�ϳ�ʼ��
	vv mat_;//����
	vv matT_;//ת�þ���
	vv matEliminated_;//��Ԫ��ľ���
	bool vv_check(const vv& mat);
	vv InitTranspose(void);//ת�þ����ʼ��
	vv InitElimination(void);//��ʼ����Ԫ

public:
	Matrix();//Ĭ�Ϲ��캯����ʲô������
	Matrix(const v& vec);//ֻ��һ�������ľ���Ĺ��캯��:double������
	Matrix(const vv& mat);//����������Ĺ��캯��:double������
	Matrix(uint rows, uint cols, const double element);
	//�������ʳ�ʼ����ÿ��������ġ�ɾ����׷��Ԫ�ء����������
	void PropertyInit(const vv&mat);


	size_t Columns(void)const;
	size_t Rows(void)const;
	bool IsEmpty(void)const;
	//size_t Rank(void)const;
	void Clear(void);
	void Assign(uint rows, uint cols, const double element);
	void SetPropertyInit(bool init);

	//�ߴ���������
	bool IsSquare(void)const;//�Ƿ��Ƿ���
	bool IsSingular(void)const;//�Ƿ���������󣨷����������棩:��δ��д
	Matrix Elimination(void);//��Ԫ:���Ӷ�O(n^3/3)
	Matrix T(void);//ת��
	Matrix Inv(void);//����� :��δ��д����

	//���������
	v& operator[](int col);
	v operator[](int col)const;
	void operator=(const v& vec);
	void operator=(const vv& mat);
	Matrix operator+(const Matrix& mat);
	Matrix operator-(const Matrix& mat);
	Matrix operator*(const Matrix& mat);//����*����
	Matrix operator*(const double c);//����*����

	//��Ԫ����
	friend Matrix operator*(const double c, const Matrix& mat);
	friend ostream& operator<<(ostream& os, const Matrix& mat);
	~Matrix();//��������
};


/*****************����ת��Ϊ������ģ�庯��*****************/
//ģ�庯������
template<class Typename>
vector<double> ArrayToVector(Typename arr[], int size);
template<class Typename>
vv ArrayToVector(Typename* arr[], int rows, int cols);

//ģ�庯������
//ģ�庯�������������������ͬһ���ļ���
template<class Typename>
vector<double> ArrayToVector(Typename arr[], int size) {

	vector<double> vec(arr, arr + size);
	return vec;
}

template<class Typename>
vv ArrayToVector(Typename* arr[], int rows, int cols) {
	vv mat(rows, vector<double>(cols));
	//���������ڵ�ÿһ��ָ��
	for (int i = 0; i < rows; i++) {
		//��ÿ��ָ��ָ������鸳ֵ���������
		mat[i] = vector<double>(arr[i], arr[i] + cols);
	}
	return mat;
}

/***********************************************************/