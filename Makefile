ifndef GOPATH
$(error GOPATH is not set)
endif
ifndef CGO_CPPFLAGS
$(warning CGO_CPPFLAGS is not set, you probably need to set it)
endif

SWIG ?= swig
GO ?= go

all:
	${SWIG} -go -intgosize 64 -cgo -c++ RedSVD.i
	cp RedSVD-h RedSVD.h RedSVD_wrap.cxx redsvd.go redsvd_test.go ${GOPATH}/src/redsvd
	cd ${GOPATH}/src/redsvd ; ${GO} get -t ./...
	cd ${GOPATH}/src/redsvd ; ${GO} build
	cd ${GOPATH}/src/redsvd ; ${GO} test
