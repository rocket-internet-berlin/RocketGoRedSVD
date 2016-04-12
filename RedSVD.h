/* 
 * Go integration of RedSVD.h
 * 
 * Copyright (c) 2016 Rocket Internet
 * For license, see included LICENSE file.
 */
#include "RedSVD-h"

#include <math.h>
#include <iostream>
#include <map>
#include <algorithm>
#include <stdexcept>

typedef Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> MatrixXf;
typedef Eigen::SparseMatrix<float> SpMatrixXf;
typedef Eigen::Map<MatrixXf> MapTypeXf;
typedef Eigen::Map<const MatrixXf> MapTypeConstXf;

typedef Eigen::Triplet<float> TripletXf;

class GoRedSVD 
{
protected:
  MapTypeXf *denseMap;
  MatrixXf *dense;
  SpMatrixXf *sparse;

  bool unNormalized;
  
  RedSVD::RedSVD<MapTypeXf> *redSvdDense;
  RedSVD::RedSVD<SpMatrixXf> *redSvdSparse;

  std::vector<TripletXf> *tripletList;
    
public:
  ~GoRedSVD() {
    if (sparse) {
      delete sparse;
    }
    if (denseMap) {
      delete denseMap;
    }
    if (redSvdSparse) {
      delete redSvdSparse;
    }
    if (redSvdDense) {
      delete redSvdDense;
    }
  }

  GoRedSVD() {
    denseMap = NULL;
    dense = NULL;
    sparse = NULL;
    unNormalized = false;
    redSvdDense = NULL;
    redSvdSparse = NULL;
    tripletList = NULL;
  }

  MatrixXf matrixU() {
    if (redSvdSparse) {
      return redSvdSparse->matrixU();
    }
    if (redSvdDense) {
      return redSvdDense->matrixU();
    }
    MatrixXf emptymatrix(0, 0);
    return emptymatrix;
  }

  MatrixXf matrixUNotNormalized() {
    if (redSvdSparse) {
      return redSvdSparse->matrixUNotNormalized();
    }
    if (redSvdDense) {
      return redSvdDense->matrixUNotNormalized();
    }
    MatrixXf emptymatrix(0, 0);
    return emptymatrix;
  }

  ScalarXf singularValues() {
    if (redSvdSparse) {
      return redSvdSparse->singularValues();
    }
    if (redSvdDense) {
      return redSvdDense->singularValues();
    }
    ScalarXf emptyscalar;
    return emptyscalar;
  }
  
  MatrixXf matrixV() {
    if (redSvdSparse) {
      return redSvdSparse->matrixV();
    }
    if (redSvdDense) {
      return redSvdDense->matrixV();
    }
    MatrixXf emptymatrix(0, 0);
    return emptymatrix;
  }

  MatrixXf matrixVNotNormalized() {
    if (redSvdSparse) {
      return redSvdSparse->matrixVNotNormalized();
    }
    if (redSvdDense) {
      return redSvdDense->matrixVNotNormalized();
    }
    MatrixXf emptymatrix(0, 0);
    return emptymatrix;
  }

  
  void setUnnormalized(bool value) {
    unNormalized = value;
  }
  
  bool redSVD() {
    if (denseMap) {
      redSvdDense = new RedSVD::RedSVD<MapTypeXf>(*denseMap, unNormalized);
      return true;
    }
    if (sparse) {
      redSvdSparse = new RedSVD::RedSVD<SpMatrixXf>(*sparse, unNormalized);
      return true;
    }
    return false;
  }

  bool redSVD(const int rank) {
    if (denseMap) {
      redSvdDense = new RedSVD::RedSVD<MapTypeXf>(*denseMap, (MapTypeXf::Index)rank, unNormalized);
      return true;
    }
    if (sparse) {
      redSvdSparse = new RedSVD::RedSVD<SpMatrixXf>(*sparse, (SpMatrixXf::Index)rank, unNormalized);
      return true;
    }
    return false;
  }

  void setDenseMatrix32(const int rows, const int cols, float **matrix) {
    if (denseMap) {
      delete denseMap;
    }
    denseMap = new MapTypeXf(*matrix, rows, cols);
  }

  void initializeSparseMatrix(const int rows, const int cols) {
    if (sparse) {
      delete sparse;
    }
    if (tripletList) {
      delete tripletList;
    }
    sparse = new SpMatrixXf(rows, cols);
    tripletList = new std::vector<TripletXf>();
  }

  void finalizeSparseMatrix() {
    sparse->setFromTriplets(tripletList->begin(), tripletList->end());
    delete tripletList;
  }
  
  bool addSparseMatrixRow(const int row, const int cols, const float *items) {
    if (!sparse) {
      return false;
    }
    
    for (int i = 0; i < cols; i++) {
      if (items[i] != 0.0) {
	tripletList->push_back(TripletXf(row, i, items[i]));
      }
    }
    return true;
  }

  ScalarXf getColumnNorms() {
    if (redSvdSparse) {
      return redSvdSparse->getNorms();
    } else {
      return redSvdDense->getNorms();
    }
  }
};
