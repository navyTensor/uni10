/****************************************************************************
*  @file CMakeLists.txt
*  @license
*    Universal Tensor Network Library
*    Copyright (c) 2013-2014
*    Yun-Da Hsieh, Pochung Chen and Ying-Jer Kao
*
*    This file is part of Uni10, the Universal Tensor Network Library.
*
*    Uni10 is free software: you can redistribute it and/or modify
*    it under the terms of the GNU Lesser General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*    Uni10 is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU Lesser General Public License for more details.
*
*    You should have received a copy of the GNU Lesser General Public License
*    along with Uni10.  If not, see <http://www.gnu.org/licenses/>.
*  @endlicense
*  @brief Main specification file for CMake
*  @author Ying-Jer Kao
*  @date 2014-05-06
*  @since 0.1.0
*
*****************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <uni10/tensor-network/Matrix.h>
#include <uni10/numeric/uni10_lapack.h>
#include <uni10/tools/uni10_tools.h>
namespace uni10{
std::ostream& operator<< (std::ostream& os, const Matrix& m){
	os << m.Rnum << " x " << m.Cnum << " = " << m.m_elemNum;
	if(m.diag)
		os << ", Diagonal";
	if(m.ongpu)
		os<< ", onGPU";
	os <<std::endl << std::endl;
	double* elem;
	if(m.ongpu){
		elem = (double*)malloc(m.m_elemNum * sizeof(double));
		elemCopy(elem, m.m_elem, m.m_elemNum * sizeof(double), false, m.ongpu);
	}
	else
		elem = m.m_elem;
	for(size_t i = 0; i < m.Rnum; i++){
		for(size_t j = 0; j < m.Cnum; j++)
			if(m.diag){
				if(i == j)
					os << std::setw(7) << std::fixed << std::setprecision(3) << elem[i];
				else
					os << std::setw(7) << std::fixed << std::setprecision(3) << 0.0;
			}
			else
				os << std::setw(7) << std::fixed << std::setprecision(3) << elem[i * m.Cnum + j];
		os << std::endl << std::endl;
	}
	if(m.ongpu)
		free(elem);
	return os;
}

Matrix::Matrix(): Rnum(0), Cnum(0), m_elemNum(0), diag(false), m_elem(NULL), ongpu(false){
}
Matrix::Matrix(const Matrix& _m): Rnum(_m.Rnum), Cnum(_m.Cnum), m_elemNum(_m.m_elemNum), diag(_m.diag), m_elem(NULL), ongpu(false){
	if(m_elemNum){
		m_elem = (double*)elemAlloc(m_elemNum * sizeof(double), ongpu);
		elemCopy(m_elem, _m.m_elem, m_elemNum * sizeof(double), ongpu, _m.ongpu);
	}
}

void Matrix::init(bool _ongpu){
	if(diag)
		m_elemNum = Rnum < Cnum ? Rnum : Cnum;
	if(m_elemNum){
		if(_ongpu)	// Try to allocate GPU memory
			m_elem = (double*)elemAlloc(m_elemNum * sizeof(double), ongpu);
		else{
			m_elem = (double*)elemAllocForce(m_elemNum * sizeof(double), false);
			ongpu = false;
		}
	}
}

void Matrix::init(double* _elem, bool src_ongpu){
	init(true);
	elemCopy(m_elem, _elem, m_elemNum * sizeof(double), ongpu, src_ongpu);
}
Matrix::Matrix(size_t _Rnum, size_t _Cnum, double* _elem, bool _diag, bool src_ongpu): Rnum(_Rnum), Cnum(_Cnum), m_elemNum(_Rnum * _Cnum), diag(_diag), m_elem(NULL), ongpu(false){
	init(_elem, src_ongpu);
}

Matrix::Matrix(size_t _Rnum, size_t _Cnum, std::vector<double> _elem, bool _diag, bool src_ongpu): Rnum(_Rnum), Cnum(_Cnum), m_elemNum(_Rnum * _Cnum), diag(_diag), m_elem(NULL), ongpu(false){
	init(&_elem[0], src_ongpu);
}

Matrix::Matrix(size_t _Rnum, size_t _Cnum, bool _diag, bool _ongpu): Rnum(_Rnum), Cnum(_Cnum), m_elemNum(_Rnum * _Cnum), diag(_diag), m_elem(NULL), ongpu(false){
	init(_ongpu);
	if(m_elemNum)
		elemBzero(m_elem, m_elemNum * sizeof(double), ongpu);
}

Matrix& Matrix::operator=(const Matrix& _m){
	Rnum = _m.Rnum;
	Cnum = _m.Cnum;
	m_elemNum = _m.m_elemNum;
	diag = _m.diag;
	if(m_elem != NULL)
		elemFree(m_elem, m_elemNum * sizeof(double), ongpu);
	m_elem = (double*)elemAlloc(m_elemNum * sizeof(double), ongpu);
	elemCopy(m_elem, _m.m_elem, m_elemNum * sizeof(double), ongpu, _m.ongpu);
	return *this;
}

Matrix::~Matrix(){
	if(m_elem != NULL)
		elemFree(m_elem, m_elemNum * sizeof(double), ongpu);
}

size_t Matrix::row()const{
	return Rnum;
}

size_t Matrix::col()const{
	return Cnum;
}
size_t Matrix::elemNum()const{
	return m_elemNum;
}

Matrix operator* (const Matrix& Ma, const Matrix& Mb){
	assert(Ma.Cnum == Mb.Rnum);
	if((!Ma.diag) && (!Mb.diag)){
		Matrix Mc(Ma.Rnum, Mb.Cnum);
	  	matrixMul(Ma.m_elem, Mb.m_elem, Ma.Rnum, Mb.Cnum, Ma.Cnum, Mc.m_elem, Ma.ongpu, Mb.ongpu, Mc.ongpu);
		return Mc;
	}
	else if(Ma.diag && (!Mb.diag)){
		Matrix Mc(Mb);
		double* Ma_elem = Ma.m_elem;
		if(Ma.ongpu){
			Ma_elem = (double*)malloc(Ma.m_elemNum * sizeof(double));
			elemCopy(Ma_elem, Ma.m_elem, Ma.m_elemNum * sizeof(double), false, Ma.ongpu);
		}
		std::cout<<Ma<<std::endl;
		std::cout<<Mc<<std::endl;
		for(size_t i = 0; i < Ma.m_elemNum; i++)
			vectorScal(Ma_elem[i], &(Mc.m_elem[i * Mc.Cnum]), Mc.Cnum, Mc.ongpu);          
		if(Ma.ongpu){
			free(Ma_elem);
		}
		return Mc;
	}
	else if((!Ma.diag) && Mb.diag){
		Matrix Mc(Ma.Rnum, Mb.Cnum);
		for(size_t i = 0; i < Ma.Rnum; i++)
			for(size_t j = 0; j < Mb.m_elemNum; j++)
				Mc.m_elem[i * Mb.Cnum + j] = Ma.m_elem[i * Ma.Cnum + j] * Mb.m_elem[j];
		return Mc;
	}
	else{
		Matrix Mc(Ma.Rnum, Mb.Cnum, true);
		for(size_t i = 0; i < Ma.Rnum; i++)
			Mc.m_elem[i] = Ma.m_elem[i] * Mb.m_elem[i];
		return Mc;
	}
}

bool operator== (const Matrix& m1, const Matrix& m2){
	double diff;
	if(m1.m_elemNum == m2.m_elemNum){
		for(size_t i = 0; i < m1.m_elemNum; i++){
			diff = fabs(m1.m_elem[i] - m2.m_elem[i]);
			if(diff > 1E-6)
				return false;
		}
	}
	else
		return false;
	return true;
}


Matrix& Matrix::operator*= (const Matrix& Mb){
	return *this = *this * Mb;
}

void Matrix::setElem(std::vector<double> elem, bool _ongpu){
  setElem(&elem[0], _ongpu);
}
void Matrix::setElem(double* elem, bool _ongpu){
	elemCopy(m_elem, elem, m_elemNum * sizeof(double), ongpu, _ongpu);
}

std::vector<Matrix> Matrix::diagonalize()const{
	assert(Rnum == Cnum);
	assert(!diag);
	std::vector<Matrix> outs;
	Matrix Eig(Rnum, Cnum, true, ongpu);
	Matrix EigV(Rnum, Cnum, false, ongpu);
	syDiag(m_elem, Rnum, Eig.m_elem, EigV.m_elem, ongpu);
	outs.push_back(Eig);
	outs.push_back(EigV);
	return outs;
}

std::vector<Matrix> Matrix::svd() const{
	assert(!diag);
	std::vector<Matrix> outs;
	size_t min = Rnum < Cnum ? Rnum : Cnum;	//min = min(Rnum,Cnum)
	Matrix U(Rnum, min, false, ongpu);
	Matrix S(min, min, true, ongpu);
	Matrix VT(min, Cnum, false, ongpu);
	assert(U.isOngpu() == ongpu && VT.isOngpu() == ongpu);
	matrixSVD(m_elem, Rnum, Cnum, U.m_elem, S.m_elem, VT.m_elem, ongpu);
	outs.push_back(U);
	outs.push_back(S);
	outs.push_back(VT);
	return outs;
}

void Matrix::randomize(){
	elemRand(m_elem, m_elemNum, ongpu);
}


void Matrix::orthoRand(){
	if(!diag){
		orthoRandomize(m_elem, Rnum, Cnum, ongpu);
	}
}

void Matrix::set_zero(){
	if(m_elemNum)
		elemBzero(m_elem, m_elemNum * sizeof(double), ongpu);
}

Matrix operator*(const Matrix& Ma, double a){
	Matrix Mb(Ma);
	vectorScal(a, Mb.m_elem, Mb.m_elemNum, Mb.ongpu);
	return Mb;
}

Matrix& Matrix::operator*= (double a){
	vectorScal(a, m_elem, m_elemNum, ongpu);
	return *this;
}

Matrix operator+(const Matrix& Ma, const Matrix& Mb){
	Matrix Mc(Ma);
	vectorAdd(Mc.m_elem, Mb.m_elem, Mc.m_elemNum, Mc.ongpu, Mb.ongpu);
	return Mc;
}

Matrix& Matrix::operator+= (const Matrix& Mb){
	vectorAdd(m_elem, Mb.m_elem, m_elemNum, ongpu, Mb.ongpu);
	return *this;
}

Matrix& Matrix::transpose(){
	if(!diag){
		double* transElem;
		size_t memsize = m_elemNum * sizeof(double);
		transElem = (double*)elemAllocForce(memsize, ongpu);
	  	setTranspose(m_elem, Rnum, Cnum, transElem, ongpu);
		elemFree(m_elem, memsize, ongpu);
		m_elem = transElem;
	}
	size_t tmp = Rnum;
	Rnum = Cnum;
	Cnum = tmp;
  return *this;
}

Matrix& Matrix::resize(size_t row, size_t col){
	if(diag){
		size_t elemNum = row < col ? row : col;
		if(elemNum > m_elemNum){
			bool des_ongpu;
			double* elem = (double*)elemAlloc(elemNum * sizeof(double), des_ongpu);
			elemBzero(elem, elemNum * sizeof(double), des_ongpu);
			printf("RESIZE: %d\n", des_ongpu);
			elemCopy(elem, m_elem, m_elemNum * sizeof(double), des_ongpu, ongpu);
			elemFree(m_elem, m_elemNum * sizeof(double), ongpu);
			/*
			double* elem = (double*)calloc(elemNum, sizeof(double));
			memcpy(elem, m_elem, elemNum * sizeof(double));
			free(m_elem);
			*/
			m_elem = elem;
			ongpu = des_ongpu;
		}
		else
			shrinkWithoutFree((m_elemNum - elemNum) * sizeof(double), ongpu);
		Rnum = row;
		Cnum = col;
		m_elemNum = elemNum;
		return *this;
	}
	else{
		if(col == Cnum){
			size_t elemNum = row * col;
			if(row > Rnum){
				bool des_ongpu;
				double* elem = (double*)elemAlloc(elemNum * sizeof(double), des_ongpu);
				elemBzero(elem, elemNum * sizeof(double), des_ongpu);
				elemCopy(elem, m_elem, m_elemNum * sizeof(double), des_ongpu, ongpu);
				elemFree(m_elem, m_elemNum * sizeof(double), ongpu);
				m_elem = elem;
				ongpu = des_ongpu;
			}
			else
				shrinkWithoutFree((m_elemNum - elemNum) * sizeof(double), ongpu);
			Rnum = row;
			m_elemNum = elemNum;
			return *this;
		}
		else{
			size_t data_row = row < Rnum ? row : Rnum;
			size_t data_col = col < Cnum ? col : Cnum;
			bool des_ongpu;
			double* elem = (double*)elemAlloc(row * col * sizeof(double), des_ongpu);
			elemBzero(elem, row * col * sizeof(double), des_ongpu);
			for(size_t r = 0; r < data_row; r++)
				elemCopy(&(elem[r * col]), &(m_elem[r * Cnum]), data_col * sizeof(double), des_ongpu, ongpu);
			elemFree(m_elem, m_elemNum * sizeof(double), ongpu);
			m_elem = elem;
			ongpu = des_ongpu;
			Rnum = row;
			Cnum = col;
			m_elemNum = row * col;
			return *this;
		}
	}
}

double Matrix::norm(){
	double nm = 0;
	return vectorNorm(m_elem, m_elemNum, 1, ongpu);
	//for(size_t i = 0; i < m_elemNum; i++)
	//	nm += m_elem[i] * m_elem[i];
	//return sqrt(nm);
}
double Matrix::sum(){
	double sm = 0;
	return vectorSum(m_elem, m_elemNum, 1, ongpu);
	//for(size_t i = 0; i < m_elemNum; i++)
	//	sm += m_elem[i];
	//return sm;
}
double Matrix::trace(){
	assert(Rnum == Cnum);
	if(diag)
		return vectorSum(m_elem, m_elemNum, 1, ongpu);
	else
		return vectorSum(m_elem, Cnum, Cnum + 1, ongpu);
}
void Matrix::save(const std::string& fname){
	FILE *fp = fopen(fname.c_str(), "w");
	assert(fp != NULL);
	double* elem = m_elem;
	if(ongpu){
		elem = (double*)malloc(m_elemNum * sizeof(double));
		elemCopy(elem, m_elem, m_elemNum * sizeof(double), false, ongpu);
	}
	fwrite(elem, sizeof(double), m_elemNum, fp);
	fclose(fp);
	if(ongpu)
		free(elem);
}

void Matrix::load(const std::string& fname){
	FILE *fp = fopen(fname.c_str(), "r");
	assert(fp != NULL);
	double* elem = m_elem;
	if(ongpu)
		elem = (double*)malloc(m_elemNum * sizeof(double));
	fread(elem, sizeof(double), m_elemNum, fp);
	fclose(fp);
	if(ongpu){
		elemCopy(m_elem, elem, m_elemNum * sizeof(double), ongpu, false);
		free(elem);
	}
}

double& Matrix::operator[](size_t idx){
	assert(idx < m_elemNum);
	return m_elem[idx];
}

double* Matrix::getElem()const{
	return m_elem;
}

double* Matrix::getHostElem(){
	if(ongpu){
		m_elem = (double*)mvCPU(m_elem, m_elemNum * sizeof(double), ongpu);
	}
	return m_elem;
}

double& Matrix::at(size_t r, size_t c){
	assert(r < Rnum);
	assert(c < Cnum);
	if(diag){
		assert(r == c && r < m_elemNum);
		return m_elem[r];
	}
	else
		return m_elem[r * Cnum + c];
}

Matrix takeExp(double a, const Matrix& mat){
	std::vector<Matrix> rets = mat.diagonalize();
	Matrix UT(rets[1]);
	UT.transpose();
	vectorExp(a, rets[0].getElem(), rets[0].row(), rets[0].isOngpu());
	return UT * (rets[0] * rets[1]);
}

};	/* namespace uni10 */
