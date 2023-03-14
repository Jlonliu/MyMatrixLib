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
	//矩阵操作中的向量默认为列向量
	size_t rows_ = 0; //矩阵行数(向量维度)
	size_t cols_ = 0;//矩阵列数（向量数量）
	bool empty_ = true;//空矩阵
	bool propertyinit_ = true;//矩阵性质默认初始化
	vv mat_;//矩阵
	vv matT_;//转置矩阵
	vv matEliminated_;//消元后的矩阵
	bool vv_check(const vv& mat);
	vv InitTranspose(void);//转置矩阵初始化
	vv InitElimination(void);//初始化消元

public:
	Matrix();//默认构造函数：什么都不做
	Matrix(const v& vec);//只有一个向量的矩阵的构造函数:double型向量
	Matrix(const vv& mat);//多向量矩阵的构造函数:double型向量
	Matrix(uint rows, uint cols, const double element);
	//矩阵性质初始化，每当矩阵更改、删除、追加元素、向量后调用
	void PropertyInit(const vv&mat);


	size_t Columns(void)const;
	size_t Rows(void)const;
	bool IsEmpty(void)const;
	//size_t Rank(void)const;
	void Clear(void);
	void Assign(uint rows, uint cols, const double element);
	void SetPropertyInit(bool init);

	//线代操作函数
	bool IsSquare(void)const;//是否是方阵
	bool IsSingular(void)const;//是否是奇异矩阵（非奇异矩阵可逆）:尚未编写
	Matrix Elimination(void);//消元:复杂度O(n^3/3)
	Matrix T(void);//转置
	Matrix Inv(void);//逆矩阵 :尚未编写程序

	//重载运算符
	v& operator[](int col);
	v operator[](int col)const;
	void operator=(const v& vec);
	void operator=(const vv& mat);
	Matrix operator+(const Matrix& mat);
	Matrix operator-(const Matrix& mat);
	Matrix operator*(const Matrix& mat);//矩阵*矩阵
	Matrix operator*(const double c);//矩阵*常数

	//友元函数
	friend Matrix operator*(const double c, const Matrix& mat);
	friend ostream& operator<<(ostream& os, const Matrix& mat);
	~Matrix();//析构函数
};


/*****************数组转换为向量的模板函数*****************/
//模板函数声明
template<class Typename>
vector<double> ArrayToVector(Typename arr[], int size);
template<class Typename>
vv ArrayToVector(Typename* arr[], int rows, int cols);

//模板函数定义
//模板函数定义与声明必须放在同一个文件里
template<class Typename>
vector<double> ArrayToVector(Typename arr[], int size) {

	vector<double> vec(arr, arr + size);
	return vec;
}

template<class Typename>
vv ArrayToVector(Typename* arr[], int rows, int cols) {
	vv mat(rows, vector<double>(cols));
	//遍历数组内的每一个指针
	for (int i = 0; i < rows; i++) {
		//将每个指针指向的数组赋值给矩阵的列
		mat[i] = vector<double>(arr[i], arr[i] + cols);
	}
	return mat;
}

/***********************************************************/