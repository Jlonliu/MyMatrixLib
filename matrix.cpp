#include "matrix.h"
#include <vector>
#include<iostream>
#include<iomanip>
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
		for (uint i = 0; i < mat.size(); i++) {//遍历向量
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

vv Matrix::Transpose_(void) {
	vv matT;
	if (!empty_) {//矩阵非空
		vv vv_T(rows_, v(cols_));//生成一个列数为矩阵行数的新矩阵
		for (uint j = 0; j < rows_; j++) {//遍历矩阵列向量的每一个元素
			for (uint i = 0; i < cols_; i++) {//遍历矩阵的每一个列向量
				vv_T[j][i]=(mat_[i][j]);//将每一列的第j个元素作为新的一列传入矩阵转置
			}
		}
		matT = vv_T;//此类获取转置矩阵
	}
	return matT;
}

vv Matrix::Elimination_(void) {
	vv matEliminated;
	if (!empty_) {

		//为了优化算法将矩阵行消元变位矩阵转置的列消元
		//矩阵内向量是列向量方式存储的，交换列比交换行要省时间
		matEliminated = Transpose_();//求解转置矩阵
		uint matTCols = matEliminated.size();//获取转置矩阵的列数
		uint matTRows = matEliminated[0].size();//获取转置矩阵的行数
		uint uRowBegin = 0;//首个元素并非全为0的行：开始进行消元的行
		uint uEliminatedCounter = 0;//已经消元完成列的计数器
		double dRatio = 0.0;//用于消元的比率
		bool bColChanged = false;//列交换调整是否完成

		//进行列交换：确保首列首个元素不为0
		for (uint m = 0; m < matTRows; m++) {//遍历每行
			for (uint n = uEliminatedCounter; n < matTCols; n++) {//遍历每列
				if (matEliminated[n][m] != 0) {//如果n+1列的m+1行的元素不为0
					swap(matEliminated[n], matEliminated[uEliminatedCounter]);//交换第n+1列和待消元的首列
					uRowBegin = m;//从m+1行开始消元，在此行之前的行元素已然全部为0
					bColChanged = true;//列交换调整完成
					break;//退出行交换循环循环
				}
			}
			if (bColChanged) {//如果列交换调整完成
				//进行消元
				for (uint n = uEliminatedCounter + 1; n < matTCols; n++) {//遍历每列
					dRatio = matEliminated[n][m] / matEliminated[uEliminatedCounter][m];
					for (uint m2 = uRowBegin; m2 < matTRows; m2++) {//遍历每行
						matEliminated[n][m2] -= matEliminated[uEliminatedCounter][m2] * dRatio;
					}
					
				}
				uEliminatedCounter++;//完成消元的列+1
				bColChanged = false;//列交换调整标识复位，用于后续未消元的列的列交换
			}
		}

		//消元完成后再将矩阵转置回去
		vv vvE(matTRows, v(matTCols));//生成一个列数为矩阵行数的新矩阵
		for (uint j = 0; j < matTRows; j++) {//遍历矩阵列向量的每一个元素
			for (uint i = 0; i < matTCols; i++) {//遍历矩阵的每一个列向量
				vvE[j][i] = matEliminated[i][j];//将每一列的第j个元素作为新的一列传入矩阵转置
			}
		}
		matEliminated = vvE;//此类获取转置矩阵
	}
	
	return matEliminated;
}

vv Matrix::Identity_(void) {
	vv matI;
	if (IsSquare()) {//如果矩阵是方阵
		matI.assign(cols_, v(rows_));//获取矩阵等大的矩阵
		for (uint n = 0; n < cols_; n++) {
			matI[n][n] = 1;//对角设置为1
		}
	}
	return matI;

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

size_t Matrix::Rank(void) {
	uint uRank = 0;
	//if (matEliminated_.size() <= 0) {//如果消元后的矩阵尚未初始化
	vv matEliminated = Elimination_();
	//}

	for (uint m = rows_; m > 0; m--) {//从末尾行开始向上遍历
		if (matEliminated[cols_ - 1][m - 1] != 0) {//如果最右侧列的数值不为0
			uRank = m;//获取秩
			break;
		}
	}
	return uRank;
}

void Matrix::Clear(void) {
	vv mat;
	mat_ = mat;//矩阵清空
	cols_ = 0;
	rows_ = 0;
	//propertyinit_ = true;
	//matT_ = mat;//转置清空
	//matEliminated_ = mat;//消元矩阵清空
	empty_ = true;//矩阵设置为空
}

void Matrix::Assign(uint rows, uint cols, const double element) {
	if (rows > 0 && cols > 0) {
		Clear();//首先清空矩阵
		v vec(rows, element);
		mat_.assign(cols, vec);//将double型向量推入到矩阵
		PropertyInit(mat_);//再重设矩阵
	}
}

void Matrix::PushBack(const v& vec) {
	mat_.push_back(vec);//追加一列
	cols_++;//列数加1
}

void Matrix::PushBack(const vv& mat) {
	for (uint n = 0; n < mat.size(); n++) {
		mat_.push_back(mat[n]);
		cols_++;
	}
}

void Matrix::PushBack(const Matrix& mat) {
	for (uint n = 0; n < mat.Columns(); n++) {
		mat_.push_back(mat[n]);
		cols_++;
	}
}

v Matrix::PopFront(void) {
	v vec = mat_[0];//获取第一列向量
	mat_.erase(mat_.begin());//删除第一列向量
	cols_--;
	return vec;
}
//void Matrix::SetPropertyInit(bool init = true) {
//	propertyinit_ = init;
//}

bool Matrix::IsSquare(void)const {
	if (cols_ == rows_) {
		return true;
	}
}

bool Matrix::IsSingular(void) {
	bool bIsSingular = true;//默认奇异矩阵
	if (IsSquare()) {//如果矩阵是方阵
		if (Rank() == cols_) {//如果满秩
			bIsSingular = false;//非奇异矩阵
		}
	}
	return bIsSingular;
}

Matrix Matrix::T(void)
{
	//vv vv_T;
	//if (matT_.size() > 0) {//如果转置矩阵已经存在
	//	vv_T = matT_;
	//}
	//else {
	//vv_T = InitTranspose();//获取转置矩阵
	//vv_T = matT_;//用于返回转置矩阵
	//}
	Matrix matT = Transpose_();
	return matT;//返回矩阵转置
}

Matrix Matrix::Elimination(void) {
	//vv vvEliminated;//消元后的矩阵
	//if (matEliminated_.size() > 0) {//如果消元后的矩阵已经初始化
	//	vvEliminated = matEliminated_;
	//}
	//else {

	/*	vvEliminated = InitElimination();*/
	//}
	Matrix matEliminated = Elimination_();
	return matEliminated;//返回消元后的矩阵
}

Matrix Matrix::Inv(void) {
	Matrix matInv;//逆矩阵
	if(!IsSingular()){//如果是非奇异矩阵：矩阵可逆
		matInv = mat_;//获取待求逆的矩阵
		uint uExCols = cols_;//增广单位矩阵的列数
		matInv.PushBack(I(uExCols));//增广一个单位矩阵
		matInv = matInv.Elimination();//将增广矩阵消元
		double dRatio = 0.0;//用于消元的比率
		double dDiagRatio = 0.0;//用于对角单位化的比率

		//对角单位化
		for (uint m = 0; m < matInv.Rows(); m++) {
			dDiagRatio = matInv[m][m];
			for (uint n = 0; n < matInv.Columns(); n++) {
				matInv[n][m] = matInv[n][m] / dDiagRatio;
			}
		}
		
		for (uint n = matInv.Columns() / 2; n > 0; n--) {//从矩阵中间向左遍历每列
			for (uint m = n-1; m > 0; m--) {
				dRatio = matInv[n-1][m-1] / matInv[n-1][n-1];
				matInv[n-1][m-1] -= dRatio * matInv[n-1][n-1];
				for (uint nr = matInv.Columns() / 2; nr < matInv.Columns(); nr++) {
					matInv[nr][m-1] -= dRatio*matInv[nr][n-1];
				}
			}
		}
		for (uint n = 0; n < uExCols; n++) {
			matInv.PopFront();//去掉左侧的单位矩阵
			
		}
	}
	else {//矩阵不可逆
		//占位符号：异常处理
	}
	return matInv;
}

v& Matrix::operator[](uint col) {
	return mat_[col];
}

v Matrix::operator[](uint col)const {
	v vec;
	vec = mat_[col];
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
			for (uint n = 0; n < cols_; n++) {//遍历列向量
				for (uint m = 0; m < rows_; m++) {//遍历行
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
			for (uint n = 0; n < cols_; n++) {//遍历列向量
				for (uint m = 0; m < rows_; m++) {//遍历行
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
			for (uint m = 0; m < rows_; m++) {//遍历左侧矩阵每一行
				for (uint rn = 0; rn < uRightMatCols; rn++) {//遍历右侧矩阵每列
					for (uint n = 0; n < cols_; n++) {
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
		for (uint n = 0; n < cols_; n++) {//遍历列向量
			for (uint m = 0; m < rows_; m++) {//遍历行
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
		for (uint n = 0; n < mat.Columns(); n++) {//遍历列向量
			for (uint m = 0; m < mat.Rows(); m++) {//遍历行
				vvNumnerDotMat[n][m] = mat[n][m] * c;
			}
		}
	}
	Matrix matNumberDotMat = vvNumnerDotMat;
	return matNumberDotMat;
}

ostream& operator<<(ostream& os, const Matrix& mat) {

	if (!mat.IsEmpty()) {
		
		for (uint m = 0; m < mat.Rows(); m++) {
			cout << " ";//矩阵分界
			for (uint n = 0; n < mat.Columns(); n++) {
				if (mat[n][m] < ZEROROUNDLIMIT) {//如果数值过小
					cout << setiosflags(ios::left | ios::showpoint | ios::fixed) 
						<< setprecision(3) << setw(8) << 0.0 << " ";//直接输出0
				}
				else {
					cout << setiosflags(ios::left | ios::showpoint | ios::fixed)
						<< setprecision(3) << setw(8) << mat[n][m] << " ";
				}
				
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


Matrix I(uint n) {//生成一个单位矩阵
	vv matI(n, v(n));
	for (uint i = 0; i < n; i++) {
		matI[i][i] = 1;//对角设置为1
	}
	Matrix matResult = matI;
	return matResult;
}

Matrix Ones(uint n, uint m) {//生成一个全部为1的矩阵
	vv matOnes(n, v(m));
	v vec(m, 1);
	matOnes.assign(n, vec);//将double型向量推入到矩阵
	Matrix matResult = matOnes;
	return matResult;
};

Matrix Zeros(uint n, uint m) {//生成一个全部为0的矩阵
	vv matZeros(n, v(m));
	Matrix matResult = matZeros;
	return matResult;
};

Matrix Lower(uint n) {
	vv matLower(n, v(n));
	srand(unsigned int(time(0)));//随机种子
	for (uint i = 0; i < n; i++) {//遍历每列
		for (uint j = i; j < n; j++) {//遍历每行
			matLower[i][j] = (rand() % 100) / 10.0;
		}
	}
	Matrix matResult = matLower;
	return matResult;
}

Matrix Upper(uint n) {
	vv matUpper(n, v(n));
	srand(unsigned int(time(0)));
	for (uint i = 0; i < n; i++) {
		for (uint j = 0; j <= i; j++) {
			matUpper[i][j] = (rand() % 100) / 10.0;
		}
	}
	Matrix matResult = matUpper;
	return matResult;
}

Matrix InvertibleMatrix(uint n) {
	return Lower(n) * Upper(n);
}