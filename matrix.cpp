#include "matrix.h"
#include <vector>
#include<iostream>
#include<iomanip>
using namespace std;

bool Matrix::vv_check(const vv& mat) {
	//��֤���󣺷ǿ�������ά������
	bool check_out = true;//�Ƿ���֤ͨ��
	bool check_empty = true;//�վ���

	if (mat.size() > 0) {//�������ǿ�
		if (mat[0].size() > 0) {//��������ǿ�
			check_empty = false;//����ǿ�
		}
	}

	if (!check_empty) {//�������ǿ�
		for (uint i = 0; i < mat.size(); i++) {//��������
			if (mat[0].size() != mat[i].size()) {//���һ�������е�����ά�Ȳ�ͳһ
				//�׳��쳣
				//ռλ��׷���쳣�������
				check_out = false;//��֤δͨ��
				break;//����ѭ��
			}
		}
	}
	else {//����Ϊ��
		//�վ����׳��쳣
		check_out = false;//��֤��ͨ��
	}
	return check_out;
}

vv Matrix::Transpose_(void) {
	vv matT;
	if (!empty_) {//����ǿ�
		vv vv_T(rows_, v(cols_));//����һ������Ϊ�����������¾���
		for (uint j = 0; j < rows_; j++) {//����������������ÿһ��Ԫ��
			for (uint i = 0; i < cols_; i++) {//���������ÿһ��������
				vv_T[j][i]=(mat_[i][j]);//��ÿһ�еĵ�j��Ԫ����Ϊ�µ�һ�д������ת��
			}
		}
		matT = vv_T;//�����ȡת�þ���
	}
	return matT;
}

vv Matrix::Elimination_(void) {
	vv matEliminated;
	if (!empty_) {

		//Ϊ���Ż��㷨����������Ԫ��λ����ת�õ�����Ԫ
		//��������������������ʽ�洢�ģ������бȽ�����Ҫʡʱ��
		matEliminated = Transpose_();//���ת�þ���
		uint matTCols = matEliminated.size();//��ȡת�þ��������
		uint matTRows = matEliminated[0].size();//��ȡת�þ��������
		uint uRowBegin = 0;//�׸�Ԫ�ز���ȫΪ0���У���ʼ������Ԫ����
		uint uEliminatedCounter = 0;//�Ѿ���Ԫ����еļ�����
		double dRatio = 0.0;//������Ԫ�ı���
		bool bColChanged = false;//�н��������Ƿ����

		//�����н�����ȷ�������׸�Ԫ�ز�Ϊ0
		for (uint m = 0; m < matTRows; m++) {//����ÿ��
			for (uint n = uEliminatedCounter; n < matTCols; n++) {//����ÿ��
				if (matEliminated[n][m] != 0) {//���n+1�е�m+1�е�Ԫ�ز�Ϊ0
					swap(matEliminated[n], matEliminated[uEliminatedCounter]);//������n+1�кʹ���Ԫ������
					uRowBegin = m;//��m+1�п�ʼ��Ԫ���ڴ���֮ǰ����Ԫ����Ȼȫ��Ϊ0
					bColChanged = true;//�н����������
					break;//�˳��н���ѭ��ѭ��
				}
			}
			if (bColChanged) {//����н����������
				//������Ԫ
				for (uint n = uEliminatedCounter + 1; n < matTCols; n++) {//����ÿ��
					dRatio = matEliminated[n][m] / matEliminated[uEliminatedCounter][m];
					for (uint m2 = uRowBegin; m2 < matTRows; m2++) {//����ÿ��
						matEliminated[n][m2] -= matEliminated[uEliminatedCounter][m2] * dRatio;
					}
					
				}
				uEliminatedCounter++;//�����Ԫ����+1
				bColChanged = false;//�н���������ʶ��λ�����ں���δ��Ԫ���е��н���
			}
		}

		//��Ԫ��ɺ��ٽ�����ת�û�ȥ
		vv vvE(matTRows, v(matTCols));//����һ������Ϊ�����������¾���
		for (uint j = 0; j < matTRows; j++) {//����������������ÿһ��Ԫ��
			for (uint i = 0; i < matTCols; i++) {//���������ÿһ��������
				vvE[j][i] = matEliminated[i][j];//��ÿһ�еĵ�j��Ԫ����Ϊ�µ�һ�д������ת��
			}
		}
		matEliminated = vvE;//�����ȡת�þ���
	}
	
	return matEliminated;
}

vv Matrix::Identity_(void) {
	vv matI;
	if (IsSquare()) {//��������Ƿ���
		matI.assign(cols_, v(rows_));//��ȡ����ȴ�ľ���
		for (uint n = 0; n < cols_; n++) {
			matI[n][n] = 1;//�Խ�����Ϊ1
		}
	}
	return matI;

}

Matrix::Matrix() {

}

Matrix::Matrix(const v& vec)
{
	if (vec.size() > 0) {//��������ǿ�
		mat_.push_back(vec);//��double���������뵽����
		PropertyInit(mat_);//�������ʳ�ʼ��
	}
}

Matrix::Matrix(const vv& mat)
{
	if (vv_check(mat)) {//������֤
		mat_ = mat;//��double�;���ֵ������
		PropertyInit(mat_);//�������ʳ�ʼ��
	}
}

Matrix::Matrix(uint rows, uint cols, const double element) {
	if (rows > 0 && cols > 0) {
		v vec(rows, element);
		mat_.assign(cols,vec);//��double���������뵽����
		PropertyInit(mat_);//�������ʳ�ʼ��
	}
}

void Matrix::PropertyInit(const vv& mat) {
	cols_ = mat.size();//����
	rows_ = mat[0].size();//����Ϊ����ά��
	empty_ = false;//����ǿ�
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
	//if (matEliminated_.size() <= 0) {//�����Ԫ��ľ�����δ��ʼ��
	vv matEliminated = Elimination_();
	//}

	for (uint m = rows_; m > 0; m--) {//��ĩβ�п�ʼ���ϱ���
		if (matEliminated[cols_ - 1][m - 1] != 0) {//������Ҳ��е���ֵ��Ϊ0
			uRank = m;//��ȡ��
			break;
		}
	}
	return uRank;
}

void Matrix::Clear(void) {
	vv mat;
	mat_ = mat;//�������
	cols_ = 0;
	rows_ = 0;
	//propertyinit_ = true;
	//matT_ = mat;//ת�����
	//matEliminated_ = mat;//��Ԫ�������
	empty_ = true;//��������Ϊ��
}

void Matrix::Assign(uint rows, uint cols, const double element) {
	if (rows > 0 && cols > 0) {
		Clear();//������վ���
		v vec(rows, element);
		mat_.assign(cols, vec);//��double���������뵽����
		PropertyInit(mat_);//���������
	}
}

void Matrix::PushBack(const v& vec) {
	mat_.push_back(vec);//׷��һ��
	cols_++;//������1
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
	v vec = mat_[0];//��ȡ��һ������
	mat_.erase(mat_.begin());//ɾ����һ������
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
	bool bIsSingular = true;//Ĭ���������
	if (IsSquare()) {//��������Ƿ���
		if (Rank() == cols_) {//�������
			bIsSingular = false;//���������
		}
	}
	return bIsSingular;
}

Matrix Matrix::T(void)
{
	//vv vv_T;
	//if (matT_.size() > 0) {//���ת�þ����Ѿ�����
	//	vv_T = matT_;
	//}
	//else {
	//vv_T = InitTranspose();//��ȡת�þ���
	//vv_T = matT_;//���ڷ���ת�þ���
	//}
	Matrix matT = Transpose_();
	return matT;//���ؾ���ת��
}

Matrix Matrix::Elimination(void) {
	//vv vvEliminated;//��Ԫ��ľ���
	//if (matEliminated_.size() > 0) {//�����Ԫ��ľ����Ѿ���ʼ��
	//	vvEliminated = matEliminated_;
	//}
	//else {

	/*	vvEliminated = InitElimination();*/
	//}
	Matrix matEliminated = Elimination_();
	return matEliminated;//������Ԫ��ľ���
}

Matrix Matrix::Inv(void) {
	Matrix matInv;//�����
	if(!IsSingular()){//����Ƿ�������󣺾������
		matInv = mat_;//��ȡ������ľ���
		uint uExCols = cols_;//���㵥λ���������
		matInv.PushBack(I(uExCols));//����һ����λ����
		matInv = matInv.Elimination();//�����������Ԫ
		double dRatio = 0.0;//������Ԫ�ı���
		double dDiagRatio = 0.0;//���ڶԽǵ�λ���ı���

		//�Խǵ�λ��
		for (uint m = 0; m < matInv.Rows(); m++) {
			dDiagRatio = matInv[m][m];
			for (uint n = 0; n < matInv.Columns(); n++) {
				matInv[n][m] = matInv[n][m] / dDiagRatio;
			}
		}
		
		for (uint n = matInv.Columns() / 2; n > 0; n--) {//�Ӿ����м��������ÿ��
			for (uint m = n-1; m > 0; m--) {
				dRatio = matInv[n-1][m-1] / matInv[n-1][n-1];
				matInv[n-1][m-1] -= dRatio * matInv[n-1][n-1];
				for (uint nr = matInv.Columns() / 2; nr < matInv.Columns(); nr++) {
					matInv[nr][m-1] -= dRatio*matInv[nr][n-1];
				}
			}
		}
		for (uint n = 0; n < uExCols; n++) {
			matInv.PopFront();//ȥ�����ĵ�λ����
			
		}
	}
	else {//���󲻿���
		//ռλ���ţ��쳣����
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
	if (vec.size() > 0) {//��������ǿ�
		mat_.push_back(vec);//��double���������뵽����
		PropertyInit(mat_);
	}
}

void Matrix::operator=(const vv& mat) {
	if (vv_check(mat)) {//������֤
		mat_ = mat;//��double�;���ֵ������
		PropertyInit(mat_);
	}
}

Matrix Matrix::operator+(const Matrix& mat) {
	vv vvAddMat;//�������
	if (mat.IsEmpty() || empty_) {//����Ҳ����Ϊ�ջ�������Ϊ��
		//ʲô������
	}
	else {
		if (cols_ == mat.Columns() && rows_ == mat.Rows()) {//����������Сƥ��
			vvAddMat.assign(cols_,v(rows_));
			for (uint n = 0; n < cols_; n++) {//����������
				for (uint m = 0; m < rows_; m++) {//������
					vvAddMat[n][m] = mat_[n][m] + mat[n][m];
				}
			}
		}
	}
	Matrix matAddMat = vvAddMat;
	return matAddMat;
}

Matrix Matrix::operator-(const Matrix& mat) {
	vv vvAddMat;//�������
	if (mat.IsEmpty() || empty_) {//����Ҳ����Ϊ�ջ�������Ϊ��
		//ʲô������
	}
	else {
		if (cols_ == mat.Columns() && rows_ == mat.Rows()) {//����������Сƥ��
			vvAddMat.assign(cols_, v(rows_));
			for (uint n = 0; n < cols_; n++) {//����������
				for (uint m = 0; m < rows_; m++) {//������
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
	vv vvDotMat;//�˻�����
	if (mat.IsEmpty()|| empty_) {//����Ҳ����Ϊ�ջ�������Ϊ��
		//ʲô������
	}
	else {
		double element = 0.0;//����Ԫ��
		size_t uRightMatCols = mat.Columns(); //��ȡ�Ҳ������������������
		size_t uRightMatRows = mat.Rows();//��ȡ�Ҳ��������������ά�ȣ�
		Matrix mat_T = T();//��ȡ�������ת��

		if (cols_ == uRightMatRows) {//���������ɳ�
			vvDotMat.assign(uRightMatCols, v(rows_));//����˻������С

			//����˷�������г��Ҳ���
			for (uint m = 0; m < rows_; m++) {//����������ÿһ��
				for (uint rn = 0; rn < uRightMatCols; rn++) {//�����Ҳ����ÿ��
					for (uint n = 0; n < cols_; n++) {
						element += mat_T[m][n] * mat[rn][n];
					}
					vvDotMat[m][rn] = element;//��Ԫ��������ʱ������
					element = 0.0;//element����
				}
			}
		}
	}
	Matrix matDotMat = vvDotMat;
	return matDotMat;//������˾���
}

Matrix Matrix::operator*(const double c) {
	vv vvNumnerDotMat;//�������
	if (empty_) {//������Ϊ��
		//ʲô������
	}
	else {
		vvNumnerDotMat.assign( cols_,v(rows_));
		for (uint n = 0; n < cols_; n++) {//����������
			for (uint m = 0; m < rows_; m++) {//������
				vvNumnerDotMat[n][m] = mat_[n][m]*c;
			}
		}
	}
	Matrix matNumnerDotMat = vvNumnerDotMat;
	return matNumnerDotMat;
}

Matrix operator*(const double c, const Matrix& mat) {
	vv vvNumnerDotMat;//�������
	if (mat.IsEmpty()) {//�Ҳ����Ϊ��
		//ʲô������
	}
	else {
		vvNumnerDotMat.assign(mat.Columns(),v(mat.Rows()));
		for (uint n = 0; n < mat.Columns(); n++) {//����������
			for (uint m = 0; m < mat.Rows(); m++) {//������
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
			cout << " ";//����ֽ�
			for (uint n = 0; n < mat.Columns(); n++) {
				if (mat[n][m] < ZEROROUNDLIMIT) {//�����ֵ��С
					cout << setiosflags(ios::left | ios::showpoint | ios::fixed) 
						<< setprecision(3) << setw(8) << 0.0 << " ";//ֱ�����0
				}
				else {
					cout << setiosflags(ios::left | ios::showpoint | ios::fixed)
						<< setprecision(3) << setw(8) << mat[n][m] << " ";
				}
				
			}
			cout << "\n";//����ֽ�
		}
	}

	return os;
}

Matrix::~Matrix()
{
	cout << "��������������" << endl;
}


Matrix I(uint n) {//����һ����λ����
	vv matI(n, v(n));
	for (uint i = 0; i < n; i++) {
		matI[i][i] = 1;//�Խ�����Ϊ1
	}
	Matrix matResult = matI;
	return matResult;
}

Matrix Ones(uint n, uint m) {//����һ��ȫ��Ϊ1�ľ���
	vv matOnes(n, v(m));
	v vec(m, 1);
	matOnes.assign(n, vec);//��double���������뵽����
	Matrix matResult = matOnes;
	return matResult;
};

Matrix Zeros(uint n, uint m) {//����һ��ȫ��Ϊ0�ľ���
	vv matZeros(n, v(m));
	Matrix matResult = matZeros;
	return matResult;
};

Matrix Lower(uint n) {
	vv matLower(n, v(n));
	srand(unsigned int(time(0)));//�������
	for (uint i = 0; i < n; i++) {//����ÿ��
		for (uint j = i; j < n; j++) {//����ÿ��
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