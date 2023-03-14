#include "matrix.h"
#include <vector>
#include<iostream>
using namespace std;

bool Matrix::vv_check(const vv& mat) {
	//验证矩阵：非空且向量维度整齐
	bool check_out = true;//是否验证通过
	bool check_empty = true;//空矩阵

	if (mat.size() > 0) {//如果矩阵非空
		if (mat[0].size() > 0) {//如果向量非空
			check_empty = false;//矩阵非空
		}
	}

	if (!check_empty) {//如果矩阵非空
		for (int i = 0; i < mat.size(); i++) {//遍历向量
			if (mat[0].size() != mat[i].size()) {//如果一个矩阵中的向量维度不统一
				//抛出异常
				//占位：追加异常处理代码
				check_out = false;//验证未通过
				break;//跳出循环
			}
		}
	}
	else {//矩阵为空
		//空矩阵不抛出异常
		check_out = false;//验证不通过
	}
	return check_out;
}

vv Matrix::InitTranspose(void) {
	if (!empty_) {//矩阵非空
		vv vv_T(rows_, v(cols_));//生成一个列数为矩阵行数的新矩阵
		for (uint j = 0; j < rows_; j++) {//遍历矩阵列向量的每一个元素
			for (uint i = 0; i < cols_; i++) {//遍历矩阵的每一个列向量
				vv_T[j][i]=(mat_[i][j]);//将每一列的第j个元素作为新的一列传入矩阵转置
			}
		}
		matT_ = vv_T;//此类获取转置矩阵
	}
	return matT_;
}

vv Matrix::InitElimination(void) {
	if (!empty_) {
		if (matT_.size() <= 0) {//如果转置矩阵为空
			InitTranspose();//求解转置矩阵
		}
		
		//为了优化算法将矩阵行消元变位矩阵转置的列消元
		//矩阵内向量是列向量方式存储的，交换列比交换行要省时间
		vv matEliminated = matT_;//获取转置矩阵
		uint matTCols = matEliminated.size();//获取转置矩阵的列数
		uint matTRows = matEliminated[0].size();//获取转置矩阵的行数
		int uRowBegin = 0;//首个元素并非全为0的行：开始进行消元的行
		int uEliminatedCounter = 0;//已经消元完成列的计数器
		double dRatio = 0.0;//用于消元的比率
		bool bColChanged = false;//列交换调整是否完成

		//进行列交换：确保首列首个元素不为0
		for (int m = 0; m < matTRows; m++) {//遍历每行
			for (int n = uEliminatedCounter; n < matTCols; n++) {//遍历每列
				if (matEliminated[n][m] != 0) {//如果n+1列的m+1行的元素不为0
					swap(matEliminated[n], matEliminated[uEliminatedCounter]);//交换第n+1列和待消元的首列
					uRowBegin = m;//从m+1行开始消元，在此行之前的行元素已然全部为0
					bColChanged = true;//列交换调整完成
					break;//退出行交换循环循环
				}
			}
			if (bColChanged) {//如果列交换调整完成
				//进行消元
				for (int n = uEliminatedCounter + 1; n < matTCols; n++) {//遍历每列
					dRatio = matEliminated[n][m] / matEliminated[uEliminatedCounter][m];
					for (int m2 = uRowBegin; m2 < matTRows; m2++) {//遍历每行
						matEliminated[n][m2] -= matEliminated[uEliminatedCounter][m2] * dRatio;
					}
					
				}
				uEliminatedCounter++;//完成消元的列+1
				bColChanged = false;//列交换调整标识复位，用于后续未消元的列的列交换
			}
		}

		//消元完成后再将矩阵转置回去
		vv vvE(matTRows, v(matTCols));//生成一个列数为矩阵行数的新矩阵
		for (int j = 0; j < matTRows; j++) {//遍历矩阵列向量的每一个元素
			for (int i = 0; i < matTCols; i++) {//遍历矩阵的每一个列向量
				vvE[j][i] = matEliminated[i][j];//将每一列的第j个元素作为新的一列传入矩阵转置
			}
		}
		matEliminated_ = vvE;//此类获取转置矩阵
	}
	
	return matEliminated_;
}

Matrix::Matrix() {

}

Matrix::Matrix(const v& vec)
{
	if (vec.size() > 0) {//如果向量非空
		mat_.push_back(vec);//将double型向量推入到矩阵
		PropertyInit(mat_);//矩阵性质初始化
	}
}

Matrix::Matrix(const vv& mat)
{
	if (vv_check(mat)) {//矩阵验证
		mat_ = mat;//将double型矩阵赋值给矩阵
		PropertyInit(mat_);//矩阵性质初始化
	}
}

Matrix::Matrix(uint rows, uint cols, const double element) {
	if (rows > 0 && cols > 0) {
		v vec(rows, element);
		mat_.assign(cols,vec);//将double型向量推入到矩阵
		PropertyInit(mat_);//矩阵性质初始化
	}
}

void Matrix::PropertyInit(const vv& mat) {
	cols_ = mat.size();//列数
	rows_ = mat[0].size();//行数为向量维度
	empty_ = false;//矩阵非空
	if (propertyinit_) {//如果需要初始化矩阵的一系列性质
		InitTranspose();//转置矩阵
		InitElimination();//消元矩阵
	}
	
}

size_t Matrix::Columns(void)const {
	return cols_;
}

size_t Matrix::Rows(void)const {
	return rows_;
}

bool Matrix::IsEmpty(void)const {
	return empty_;
}

void Matrix::Clear(void) {
	vv mat;
	mat_ = mat;
	cols_ = 0;
	rows_ = 0;
	empty_ = true;
}

void Matrix::Assign(uint rows, uint cols, const double element) {
	if (rows > 0 && cols > 0) {
		v vec(rows, element);
		mat_.assign(cols, vec);//将double型向量推入到矩阵
		PropertyInit(mat_);
	}
}

void Matrix::SetPropertyInit(bool init = true) {
	propertyinit_ = init;
}

bool Matrix::IsSquare(void)const {
	if (cols_ == rows_) {
		return true;
	}
}

bool Matrix::IsSingular(void)const {
	return true;
}

Matrix Matrix::T(void)
{
	vv vv_T;
	if (matT_.size() > 0) {//如果转置矩阵已经存在
		vv_T = matT_;
	}
	else {
		matT_ = InitTranspose();//获取转置矩阵
		vv_T = matT_;//用于返回转置矩阵
	}
	Matrix matT = vv_T;
	return matT;//返回矩阵转置
}

Matrix Matrix::Elimination(void) {
	vv vvEliminated;//消元后的矩阵
	if (matEliminated_.size() > 0) {//如果消元后的矩阵已经初始化
		vvEliminated = matEliminated_;
	}
	else {

		vvEliminated = InitElimination();
	}
	Matrix matEliminated = vvEliminated;
	return matEliminated;//返回消元后的矩阵
}

Matrix Matrix::Inv(void) {
	Matrix matInv;//逆矩阵
	return matInv;
}

v& Matrix::operator[](int col) {
	if (col >= 0) {//如果下标为正序
		return mat_[col];
	}
	else {//下标逆序
		return mat_[cols_ - abs(col)];
	}
}

v Matrix::operator[](int col)const {
	v vec;
	if (col >= 0) {//如果下标为正序
		vec = mat_[col];
	}
	else {//下标逆序
		vec = mat_[cols_ - abs(col)];
	}
	return vec;
}

void Matrix::operator=(const v& vec) {
	if (vec.size() > 0) {//如果向量非空
		mat_.push_back(vec);//将double型向量推入到矩阵
		PropertyInit(mat_);
	}
}

void Matrix::operator=(const vv& mat) {
	if (vv_check(mat)) {//矩阵验证
		mat_ = mat;//将double型矩阵赋值给矩阵
		PropertyInit(mat_);
	}
}

Matrix Matrix::operator+(const Matrix& mat) {
	vv vvAddMat;//结果矩阵
	if (mat.IsEmpty() || empty_) {//如果右侧矩阵为空或左侧矩阵为空
		//什么都不做
	}
	else {
		if (cols_ == mat.Columns() && rows_ == mat.Rows()) {//如果两矩阵大小匹配
			vvAddMat.assign(cols_,v(rows_));
			for (int n = 0; n < cols_; n++) {//遍历列向量
				for (int m = 0; m < rows_; m++) {//遍历行
					vvAddMat[n][m] = mat_[n][m] + mat[n][m];
				}
			}
		}
	}
	Matrix matAddMat = vvAddMat;
	return matAddMat;
}

Matrix Matrix::operator-(const Matrix& mat) {
	vv vvAddMat;//结果矩阵
	if (mat.IsEmpty() || empty_) {//如果右侧矩阵为空或左侧矩阵为空
		//什么都不做
	}
	else {
		if (cols_ == mat.Columns() && rows_ == mat.Rows()) {//如果两矩阵大小匹配
			vvAddMat.assign(cols_, v(rows_));
			for (int n = 0; n < cols_; n++) {//遍历列向量
				for (int m = 0; m < rows_; m++) {//遍历行
					vvAddMat[n][m] = mat_[n][m] - mat[n][m];
				}
			}
		}
	}
	Matrix matAddMat = vvAddMat;
	return matAddMat;
}

Matrix Matrix::operator*(const Matrix& mat)
{
	vv vvDotMat;//乘积矩阵
	if (mat.IsEmpty()|| empty_) {//如果右侧矩阵为空或左侧矩阵为空
		//什么都不做
	}
	else {
		double element = 0.0;//矩阵元素
		size_t uRightMatCols = mat.Columns(); //获取右侧矩阵列数（向量数）
		size_t uRightMatRows = mat.Rows();//获取右侧矩阵行数（向量维度）
		Matrix mat_T = T();//获取左侧矩阵的转置

		if (cols_ == uRightMatRows) {//如果两矩阵可乘
			vvDotMat.assign(uRightMatCols, v(rows_));//分配乘积矩阵大小

			//矩阵乘法：左侧行乘右侧列
			for (int m = 0; m < rows_; m++) {//遍历左侧矩阵每一行
				for (int rn = 0; rn < uRightMatCols; rn++) {//遍历右侧矩阵每列
					for (int n = 0; n < cols_; n++) {
						element += mat_T[m][n] * mat[rn][n];
					}
					vvDotMat[m][rn] = element;//将元素置入临时矩阵中
					element = 0.0;//element清零
				}
			}
		}
	}
	Matrix matDotMat = vvDotMat;
	return matDotMat;//返回相乘矩阵
}

Matrix Matrix::operator*(const double c) {
	vv vvNumnerDotMat;//结果矩阵
	if (empty_) {//左侧矩阵为空
		//什么都不做
	}
	else {
		vvNumnerDotMat.assign( cols_,v(rows_));
		for (int n = 0; n < cols_; n++) {//遍历列向量
			for (int m = 0; m < rows_; m++) {//遍历行
				vvNumnerDotMat[n][m] = mat_[n][m]*c;
			}
		}
	}
	Matrix matNumnerDotMat = vvNumnerDotMat;
	return matNumnerDotMat;
}

Matrix operator*(const double c, const Matrix& mat) {
	vv vvNumnerDotMat;//结果矩阵
	if (mat.IsEmpty()) {//右侧矩阵为空
		//什么都不做
	}
	else {
		vvNumnerDotMat.assign(mat.Columns(),v(mat.Rows()));
		for (int n = 0; n < mat.Columns(); n++) {//遍历列向量
			for (int m = 0; m < mat.Rows(); m++) {//遍历行
				vvNumnerDotMat[n][m] = mat[n][m] * c;
			}
		}
	}
	Matrix matNumberDotMat = vvNumnerDotMat;
	return matNumberDotMat;
}

ostream& operator<<(ostream& os, const Matrix& mat) {
	//矩阵以向量为列，但是向量打印的时候是按行方式打印的，
	//所以打印的时候要转置一下

	if (!mat.IsEmpty()) {
		for (int m = 0; m < mat.Rows(); m++) {
			cout << " ";//矩阵分界
			for (int n = 0; n < mat.Columns(); n++) {
				cout << mat[n][m] << " ";
			}
			cout << "\n";//矩阵分界
		}
	}

	return os;
}

Matrix::~Matrix()
{
	cout << "析构函数被调用" << endl;
}

