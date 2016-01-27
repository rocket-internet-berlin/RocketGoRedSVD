/* 
 * Go integration of RedSVD.h
 * 
 * Copyright (c) 2016 Rocket Internet
 * For license, see included LICENSE file. 
 */
%module redsvd

%include "std_map.i"

%{
#include "RedSVD.h"
%}

// export CGO_CPPFLAGS='-I/usr/local/Cellar/eigen/3.2.6/include/eigen3/'

%rename(Wrapped_GoRedSVD) GoRedSVD;

%insert(go_wrapper) %{

type GoRedSVD interface {
    Wrapped_GoRedSVD
    SetMatrix32(int, int, map[int]map[int]float32) bool
    SetMatrix64(int, int, map[int]map[int]float64) bool
}

func (arg SwigcptrWrapped_GoRedSVD) SetMatrix32(rows int, cols int, matrix map[int]map[int]float32) bool {
  arg.InitializeSparseMatrix(rows, cols)
  for k, _ := range matrix {
    matrixRow := make([]float32, cols)
    for i := 0; i < cols; i++ {
	matrixRow[i] = matrix[k][i]
    }
    arg.AddSparseMatrixRow(k, cols, matrixRow)
  }
  arg.FinalizeSparseMatrix()
  return true
}

 
func (arg SwigcptrWrapped_GoRedSVD) SetMatrix64(rows int, cols int, matrix map[int]map[int]float64) bool {
  arg.InitializeSparseMatrix(rows, cols)
  for k, _ := range matrix {
    matrixRow := make([]float32, cols)
    for i := 0; i < cols; i++ {
	matrixRow[i] = float32(matrix[k][i])
    }
    arg.AddSparseMatrixRow(k, cols, matrixRow)
  }
  arg.FinalizeSparseMatrix()
  return true
}

func DeleteGoRedSVD(arg1 GoRedSVD) {
  DeleteWrapped_GoRedSVD(arg1)
}
 
func NewGoRedSVD() (GoRedSVD) {
  svd := NewWrapped_GoRedSVD()
  return svd.(GoRedSVD)
}
 
%}

%typemap(gotype) (MatrixXf)  "[][]float32"
%typemap(out) (MatrixXf)
%{
  {
    _goslice_ *a;
    $result.array = malloc(sizeof(_goslice_) * $1.rows());
    for (int i = 0, nRows = $1.rows(), nCols = $1.cols(); i < nRows; ++i) {
      _goslice_ *row = &((_goslice_ *)$result.array)[i];
      row->array = malloc(sizeof(float) * nCols);
      row->len = nCols;
      row->cap = nCols;
      for (int j = 0; j < nCols; ++j) {
	((float *)row->array)[j] = $1(i, j);
      }
    }
    $result.len = $1.rows();
    $result.cap = $result.len;
  }
%}

%typemap(gotype) (ScalarXf)  "[]float32"
%typemap(out) (ScalarXf)
%{
  {
    $result.array = malloc($1.rows() * $1.cols() * sizeof(float));
    $result.len = $1.rows() * $1.cols();
    $result.cap = $1.rows() * $1.cols();
    memcpy($result.array, &$1(0), $1.rows() * $1.cols() * sizeof(float));
  }
%}

%typemap(gotype) (const int rows, const int cols, float **matrix)  "[][]float32"
%typemap(in) (const int rows, const int cols, float **matrix)
%{
  {
    float *tmp;
    int maxlen = 0;

    for (int i = 0; i < $input.len; i++) {
      _goslice_ *row = &((_goslice_ *)$input.array)[i];
      if (row->len > maxlen) maxlen = row->len;
    }

    tmp = (float * )malloc(sizeof(float) * $input.len * maxlen);
    for (int i = 0; i < $input.len; i++) {
      _goslice_ *row = &((_goslice_ *)$input.array)[i];
      memcpy((void *)(tmp + (maxlen * i)), (float *)row->array, row->len * sizeof(float));
    }

    $1 = $input.len;
    $2 = maxlen;
    $3 = &tmp;
  }
%}

%typemap(gotype) (const float *)  "[]float32"
%typemap(in) (const float *matrix)
%{
  $1 = &$input.array[0];
%}

%include "RedSVD.h"

