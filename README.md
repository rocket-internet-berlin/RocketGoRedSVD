# GoRedSVD

GoRedSVD is some Swig-generated Go glue wrapping around [Nicolas Tessore](https://github.com/ntessore)'s
[RedSVD-h](https://github.com/ntessore/redsvd-h), which is based on
[Daisuke Okanohara](https://github.com/hillbig)'s redsvd implementation.

## What is RedSVD?

> redsvd is a library for solving several matrix decompositions including
> singular value decomposition (SVD), principal component analysis (PCA),
> and eigen value decomposition. redsvd can handle very large matrix
> efficiently, and optimized for a truncated SVD of sparse matrices.

(quoted from [redsvd project page](https://code.google.com/p/redsvd/))

GoRedSVD implements only the SVD part, but it should be reasonably easy to
adapt SymEigen and PCA parts also.

## Build requirements

- Swig 3.0.6 or newer (tested on 3.0.7)
- Eigen 3 library (tested on 3.2.6)

It should also be possible to [link in](http://eigen.tuxfamily.org/dox/TopicUsingIntelMKL.html) Intel®'s Math Kernel Library, which
should enable multicore processing for the SVD.

## Building

Build process is roughly this:

1. Build files using `swig`
2. Copy the resulting files into gopath
3. Provide `cgo` with the location of Eigen's include path
4. Build with `go build`.
5. You can run tests with `go test`.

```
swig -go -intgosize 64 -cgo -c++ src/RedSVD.i
mkdir -p $GOPATH/src/redsvd
cp RedSVD-h RedSVD.h RedSVD_wrap.cxx redsvd.go redsvd_test.go $GOPATH/src/redsvd

cd $GOPATH/src/redsvd
export CGO_CPPFLAGS='-I/usr/local/Cellar/eigen/3.2.6/include/eigen3/'
go build
```

Or you can use the included Makefile.

## API

- `NewGoRedSVD() GoRedSVD`: creates new GoRedSVD instance
- `SetMatrix32(cols int, rows int, matrix map[int]map[int]float32)`: sets a sparse matrix
- `SetMatrix64(cols int, rows int, matrix map[int]map[int]float64)`: float64 version of previous (simply typecasted to float32 currently)
- `SetDenseMatrix32(matrix [][]float32)`: sets a dense matrix
- `RedSVD() bool`: performs SVD
- `RedSVD(rank int) bool`: performs SVD with specified rank
- `MatrixU() [][]float32`: returns U
- `MatrixV() [][]float32`: returns V
- `SetUnnormalized(unnormalized bool)`: whether to also calculate
  un-normalized versions of the U and V matrices
  - `MatrixUNotNormalized() [][]float32`: returns U, not normalized
  - `MatrixVNotNormalized() [][]float32`: returns V, not normalized
- `SingularValues() []float32`: returns singular values
- `DeleteGoRedSVD(arg GoRedSVD)`: frees the GoRedSVD instance

## License

Copyright (c) 2016 Rocket Internet

Please see LICENSE file in repository (GPLv3). The RedSVD-h header
is licensed separately using BSD license (see RedVSD-h file for
license details).

## Sample code

```go
package main

import (
       "redsvd"
       "fmt"
       "math/rand"
)

func main() {
     svd := redsvd.NewGoRedSVD()

     fmt.Printf("Generating random matrix\n")

     sparseMatrix32 := make(map[int]map[int]float32)
     sparseMatrix64 := make(map[int]map[int]float64)
     for row := 0; row < 10; row++ {
         sparseMatrix32[row] = make(map[int]float32)
		 sparseMatrix64[row] = make(map[int]float64)
		 for col := 0; col < 10; col++ {
			 sparseMatrix32[row][col] = rand.Float32()
			 sparseMatrix64[row][col] = rand.Float64()
		 }
     }

     fmt.Printf("sparseMatrix32: %v\n", sparseMatrix32)
     fmt.Printf("sparseMatrix64: %v\n", sparseMatrix64)

     svd.SetMatrix32(10, 10, sparseMatrix32)

     svd.RedSVD()

     u := svd.MatrixU()
     fmt.Printf("float32 matrixU %v\n", u)

     v := svd.MatrixV()
     fmt.Printf("float32 matrixV %v\n", v)

     singularValues := svd.SingularValues()
     fmt.Printf("float 32 singularValues %v\n", singularValues)

     svd.SetMatrix64(10, 10, sparseMatrix64)
     svd.RedSVD()

     u = svd.MatrixU()
     fmt.Printf("float64 matrixU %v\n", u)

     v = svd.MatrixV()
     fmt.Printf("float64 matrixV %v\n", v)

     singularValues = svd.SingularValues()
     fmt.Printf("float64 singularValues %v\n", singularValues)

     redsvd.DeleteGoRedSVD(svd)
}
```

