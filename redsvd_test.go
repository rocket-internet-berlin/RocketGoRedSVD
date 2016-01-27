/* 
 * Go integration of RedSVD.h - tests
 * 
 * Copyright (c) 2016 Rocket Internet
 * For license, see included LICENSE file.
 */
package redsvd

import (
	"testing"
	"github.com/stretchr/testify/assert"
	"math/rand"
)

func TestSparseMatrix64(t *testing.T) {
	svd := NewGoRedSVD()

	assert.NotNil(t, svd)
     
	sparseMatrix64 := make(map[int]map[int]float64)
	for row := 0; row < 100; row++ {
		sparseMatrix64[row] = make(map[int]float64)
		for col := 0; col < 100; col++ {
			if (rand.Float64() < 0.2) {
				sparseMatrix64[row][col] = rand.ExpFloat64()
			}		 
		}
	}
	assert.Equal(t, 100, len(sparseMatrix64))

	svd.SetMatrix64(100, 100, sparseMatrix64)

	svd.RedSVD(10)

	u := svd.MatrixU()
	assert.NotNil(t, u)
	assert.Equal(t, 100, len(u))
	assert.Equal(t, 10, len(u[0]))
	
	v := svd.MatrixV()
	assert.NotNil(t, v)
	assert.Equal(t, 100, len(v))
	assert.Equal(t, 10, len(v[0]))

	singularValues := svd.SingularValues()
	assert.NotNil(t, singularValues)
	assert.Equal(t, 10, len(singularValues))

	svd.SetUnnormalized(true)
	svd.RedSVD(10)

	u = svd.MatrixUNotNormalized()
	assert.NotNil(t, u)
	assert.Equal(t, 100, len(u))
	assert.Equal(t, 10, len(u[0]))
	
	v = svd.MatrixVNotNormalized()
	assert.NotNil(t, v)
	assert.Equal(t, 100, len(v))
	assert.Equal(t, 10, len(v[0]))

	singularValues = svd.SingularValues()
	assert.NotNil(t, singularValues)
	assert.Equal(t, 10, len(singularValues))

	DeleteGoRedSVD(svd)
}

func TestSparseMatrix32(t *testing.T) {
	svd := NewGoRedSVD()

	assert.NotNil(t, svd)
     
	sparseMatrix32 := make(map[int]map[int]float32)
	for row := 0; row < 100; row++ {
		sparseMatrix32[row] = make(map[int]float32)
		for col := 0; col < 100; col++ {
			if (rand.Float64() < 0.2) {
				sparseMatrix32[row][col] = float32(rand.ExpFloat64())
			}		 
		}
	}
	assert.Equal(t, 100, len(sparseMatrix32))

	svd.SetMatrix32(100, 100, sparseMatrix32)

	svd.RedSVD(10)

	u := svd.MatrixU()
	assert.NotNil(t, u)
	assert.Equal(t, 100, len(u))
	assert.Equal(t, 10, len(u[0]))
	
	v := svd.MatrixV()
	assert.NotNil(t, v)
	assert.Equal(t, 100, len(v))
	assert.Equal(t, 10, len(v[0]))

	singularValues := svd.SingularValues()
	assert.NotNil(t, singularValues)
	assert.Equal(t, 10, len(singularValues))

	svd.SetUnnormalized(true)
	svd.RedSVD(10)

	u = svd.MatrixUNotNormalized()
	assert.NotNil(t, u)
	assert.Equal(t, 100, len(u))
	assert.Equal(t, 10, len(u[0]))
	
	v = svd.MatrixVNotNormalized()
	assert.NotNil(t, v)
	assert.Equal(t, 100, len(v))
	assert.Equal(t, 10, len(v[0]))

	singularValues = svd.SingularValues()
	assert.NotNil(t, singularValues)
	assert.Equal(t, 10, len(singularValues))

	DeleteGoRedSVD(svd)
}

