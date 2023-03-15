#pragma once
#include<vector>
#include<iostream>
using namespace std;

//typedef unsigned __int64 size_t;
#define UINTLIMIT 18446744073709551615//__int64型数值的极限
#define ZEROROUNDLIMIT (1E-16)//可四舍五入为0的最大数值
typedef unsigned __int64 uint;
typedef vector<double> v;
typedef vector<vector<double>> vv;

class Matrix {
private:
	//矩阵操作中的向量默认为列向量
	size_t rows_ = 0; //矩阵行数(向量维度)
	size_t cols_ = 0;//矩阵列数（向量数量）
	bool empty_ = true;//空矩阵
	//bool propertyinit_ = true;//矩阵性质默认初始化
	vv mat_;//矩阵
	//vv matT_;//转置矩阵
	//vv matEliminated_;//消元后的矩阵
	bool vv_check(const vv& mat);
	vv Transpose_(void);//转置矩阵初始化
	vv Elimination_(void);//初始化消元
	vv Identity_(void);//生成单位矩阵

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
	size_t Rank(void);
	void Clear(void);
	void Assign(uint rows, uint cols, const double element);
	void PushBack(const v& vec);//追加向量
	void PushBack(const vv& mat);//增广矩阵
	void PushBack(const Matrix& mat);//增广矩阵
	v PopFront(void);//弹出头部向量
	//void SetPropertyInit(bool init);

	//线代操作函数
	bool IsSquare(void)const;//是否是方阵
	bool IsSingular(void);//是否是奇异矩阵（非奇异矩阵可逆）:尚未编写
	Matrix Elimination(void);//消元:复杂度O(n^3/3)
	Matrix T(void);//转置
	Matrix Inv(void);//逆矩阵 :尚未编写程序

	//重载运算符
	v& operator[](uint col);
	v operator[](uint col)const;
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

Matrix I(uint n);//生成一个单位矩阵
Matrix Ones(uint n, uint m);//生成一个全部为1的矩阵
Matrix Zeros(uint n, uint m);//生成一个全部为0的矩阵
Matrix Lower(uint n);//随机生成一个n维下三角矩阵
Matrix Upper(uint n);//随机生成一个n维上三角矩阵
Matrix InvertibleMatrix(uint n);//随机生成一个n维可逆矩阵

/*****************数组转换为向量的模板函数*****************/
//模板函数声明
template<class Typename>
vector<double> ArrayToVector(Typename arr[], uint size);
template<class Typename>
vv ArrayToVector(Typename* arr[], uint rows, uint cols);

//模板函数定义
//模板函数定义与声明必须放在同一个文件里
template<class Typename>
vector<double> ArrayToVector(Typename arr[], uint size) {

	vector<double> vec(arr, arr + size);
	return vec;
}

template<class Typename>
vv ArrayToVector(Typename* arr[], uint rows, uint cols) {
	vv mat(rows, vector<double>(cols));
	//遍历数组内的每一个指针
	for (uint i = 0; i < rows; i++) {
		//将每个指针指向的数组赋值给矩阵的列
		mat[i] = vector<double>(arr[i], arr[i] + cols);
	}
	return mat;
}

/***********************************************************/