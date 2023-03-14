#include "matrix.h"
#include <vector>
#include<iostream>
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
		for (int i = 0; i < mat.size(); i++) {//��������
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

vv Matrix::InitTranspose(void) {
	if (!empty_) {//����ǿ�
		vv vv_T(rows_, v(cols_));//����һ������Ϊ�����������¾���
		for (uint j = 0; j < rows_; j++) {//����������������ÿһ��Ԫ��
			for (uint i = 0; i < cols_; i++) {//���������ÿһ��������
				vv_T[j][i]=(mat_[i][j]);//��ÿһ�еĵ�j��Ԫ����Ϊ�µ�һ�д������ת��
			}
		}
		matT_ = vv_T;//�����ȡת�þ���
	}
	return matT_;
}

vv Matrix::InitElimination(void) {
	if (!empty_) {
		if (matT_.size() <= 0) {//���ת�þ���Ϊ��
			InitTranspose();//���ת�þ���
		}
		
		//Ϊ���Ż��㷨����������Ԫ��λ����ת�õ�����Ԫ
		//��������������������ʽ�洢�ģ������бȽ�����Ҫʡʱ��
		vv matEliminated = matT_;//��ȡת�þ���
		uint matTCols = matEliminated.size();//��ȡת�þ��������
		uint matTRows = matEliminated[0].size();//��ȡת�þ��������
		int uRowBegin = 0;//�׸�Ԫ�ز���ȫΪ0���У���ʼ������Ԫ����
		int uEliminatedCounter = 0;//�Ѿ���Ԫ����еļ�����
		double dRatio = 0.0;//������Ԫ�ı���
		bool bColChanged = false;//�н��������Ƿ����

		//�����н�����ȷ�������׸�Ԫ�ز�Ϊ0
		for (int m = 0; m < matTRows; m++) {//����ÿ��
			for (int n = uEliminatedCounter; n < matTCols; n++) {//����ÿ��
				if (matEliminated[n][m] != 0) {//���n+1�е�m+1�е�Ԫ�ز�Ϊ0
					swap(matEliminated[n], matEliminated[uEliminatedCounter]);//������n+1�кʹ���Ԫ������
					uRowBegin = m;//��m+1�п�ʼ��Ԫ���ڴ���֮ǰ����Ԫ����Ȼȫ��Ϊ0
					bColChanged = true;//�н����������
					break;//�˳��н���ѭ��ѭ��
				}
			}
			if (bColChanged) {//����н����������
				//������Ԫ
				for (int n = uEliminatedCounter + 1; n < matTCols; n++) {//����ÿ��
					dRatio = matEliminated[n][m] / matEliminated[uEliminatedCounter][m];
					for (int m2 = uRowBegin; m2 < matTRows; m2++) {//����ÿ��
						matEliminated[n][m2] -= matEliminated[uEliminatedCounter][m2] * dRatio;
					}
					
				}
				uEliminatedCounter++;//�����Ԫ����+1
				bColChanged = false;//�н���������ʶ��λ�����ں���δ��Ԫ���е��н���
			}
		}

		//��Ԫ��ɺ��ٽ�����ת�û�ȥ
		vv vvE(matTRows, v(matTCols));//����һ������Ϊ�����������¾���
		for (int j = 0; j < matTRows; j++) {//����������������ÿһ��Ԫ��
			for (int i = 0; i < matTCols; i++) {//���������ÿһ��������
				vvE[j][i] = matEliminated[i][j];//��ÿһ�еĵ�j��Ԫ����Ϊ�µ�һ�д������ת��
			}
		}
		matEliminated_ = vvE;//�����ȡת�þ���
	}
	
	return matEliminated_;
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
	if (propertyinit_) {//�����Ҫ��ʼ�������һϵ������
		InitTranspose();//ת�þ���
		InitElimination();//��Ԫ����
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
		mat_.assign(cols, vec);//��double���������뵽����
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
	if (matT_.size() > 0) {//���ת�þ����Ѿ�����
		vv_T = matT_;
	}
	else {
		matT_ = InitTranspose();//��ȡת�þ���
		vv_T = matT_;//���ڷ���ת�þ���
	}
	Matrix matT = vv_T;
	return matT;//���ؾ���ת��
}

Matrix Matrix::Elimination(void) {
	vv vvEliminated;//��Ԫ��ľ���
	if (matEliminated_.size() > 0) {//�����Ԫ��ľ����Ѿ���ʼ��
		vvEliminated = matEliminated_;
	}
	else {

		vvEliminated = InitElimination();
	}
	Matrix matEliminated = vvEliminated;
	return matEliminated;//������Ԫ��ľ���
}

Matrix Matrix::Inv(void) {
	Matrix matInv;//�����
	return matInv;
}

v& Matrix::operator[](int col) {
	if (col >= 0) {//����±�Ϊ����
		return mat_[col];
	}
	else {//�±�����
		return mat_[cols_ - abs(col)];
	}
}

v Matrix::operator[](int col)const {
	v vec;
	if (col >= 0) {//����±�Ϊ����
		vec = mat_[col];
	}
	else {//�±�����
		vec = mat_[cols_ - abs(col)];
	}
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
			for (int n = 0; n < cols_; n++) {//����������
				for (int m = 0; m < rows_; m++) {//������
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
			for (int n = 0; n < cols_; n++) {//����������
				for (int m = 0; m < rows_; m++) {//������
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
			for (int m = 0; m < rows_; m++) {//����������ÿһ��
				for (int rn = 0; rn < uRightMatCols; rn++) {//�����Ҳ����ÿ��
					for (int n = 0; n < cols_; n++) {
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
		for (int n = 0; n < cols_; n++) {//����������
			for (int m = 0; m < rows_; m++) {//������
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
		for (int n = 0; n < mat.Columns(); n++) {//����������
			for (int m = 0; m < mat.Rows(); m++) {//������
				vvNumnerDotMat[n][m] = mat[n][m] * c;
			}
		}
	}
	Matrix matNumberDotMat = vvNumnerDotMat;
	return matNumberDotMat;
}

ostream& operator<<(ostream& os, const Matrix& mat) {
	//����������Ϊ�У�����������ӡ��ʱ���ǰ��з�ʽ��ӡ�ģ�
	//���Դ�ӡ��ʱ��Ҫת��һ��

	if (!mat.IsEmpty()) {
		for (int m = 0; m < mat.Rows(); m++) {
			cout << " ";//����ֽ�
			for (int n = 0; n < mat.Columns(); n++) {
				cout << mat[n][m] << " ";
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

