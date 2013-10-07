// blas.cpp - Basic Linear Algebra Stuff using the Fortran 95 argument signatures.
// Copyright (c) KALX, LLC. All rights reserved. No warranty is made.
#include <mkl_cblas.h>
#include "xllmkl.h"

using namespace xll;

//
// Level 1
//

static AddInX xai_blas_asum(
	FunctionX(XLL_DOUBLEX, _T("?xll_blas_asum"), _T("BLAS.ASUM"))
	.Arg(XLL_FPX, _T("x"), _T("is a one dimensional array of floating point numbers."))
	.Arg(XLL_LONGX, _T("_incr"), _T("is the optional increment for Vector. Default is 1. "))
	.Category(CATEGORY)
	.FunctionHelp(_T("Computes the sum of magnitudes of the vector elements."))
	.Documentation(
		_T("The <codeInline>BLAS.ASUM</codeInline> routine computes the sum of the magnitudes of elements of <codeInline>x</codeInline>: ")
		_T("<math>res = |x(0)| + ... + |x(n - 1)|</math>, where <math>x</math> is a vector with a number of elements that equals <math>n</math>.")
	)
);
double WINAPI xll_blas_asum(xfp* px, MKL_INT incr)
{
#pragma XLLEXPORT
	double res(std::numeric_limits<double>::quiet_NaN());

	try {
		xfp* px_ = fetch(px);
		MKL_INT n = size(*px_);

		if (incr == 0)
			incr = 1;

		res = cblas_dasum(n, px_->array, incr);
	}
	catch (...) {
		XLL_ERROR("BLAS.ASUM: unhandled exception");
	}

	return res;
}
#ifdef _DEBUG

#define EX(e) ExcelX(xlfEvaluate, OPERX(_T(e)))

void xll_test_asum()
{
	OPERX o;
	
	o = EX("=BLAS.ASUM({1,-2,4})");
	ensure (o == 1 + 2 + 4);

	o = EX("=BLAS.ASUM({1,-2,4}, 2)");
	ensure (o == 1 + 4);

	o = EX("=BLAS.ASUM({1,-2,4}, -2)");
	ensure (o == 1 + 4); // surprise!
}

#endif // _DEBUG
//
// Level 2
//

static AddInX xai_blas_gemv(
	FunctionX(XLL_FPX, _T("?xll_blas_gemv"), _T("BLAS.GEMV"))
	.Arg(XLL_FPX, _T("a"), _T("is a matrix."))
	.Arg(XLL_FPX, _T("x"), _T("is a vector."))
	.Arg(XLL_DOUBLEX, _T("_alpha"), _T("is an optional number. Default is 1."))
	.Category(CATEGORY)
	.FunctionHelp(_T("Computes a matrix-vector product using a general matrix."))
	.Documentation(
		_T("The <codeInline>BLAS.GEMV</codeInline> routine performs a matrix-vector operation defined as ")
		_T("<math>alpha*a*x</math>. ")
	)
);
xfp* WINAPI xll_blas_gemv(xfp* a, xfp* x, double alpha)
{
#pragma XLLEXPORT
	static FPX y;

	try {
		static char trans('T');
		static int one(1);
		int m = a->rows;
		int n = a->columns;

		ensure (size(*x) == n);
		y.resize(a->rows, 1);
		y = 0;

		if (alpha == 0)
			alpha = 1;

		cblas_dgemv(CblasRowMajor, CblasNoTrans, m, n, alpha, a->array, n, x->array, one, 0, y.array(), 1);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return y.get();
}

#ifdef _DEBUG

void xll_test_gemv()
{
	OPERX o;

	o = EX("=BLAS.GEMV({1,2,3;4,5,6}, {7,8,9})");
	ensure (o.size() == 2);
	ensure (o[0] == 1*7 + 2*8 + 3*9);
	ensure (o[1] == 4*7 + 5*8 + 6*9);
}

#endif // _DEBUG


//
// Level 3
//

static AddInX xai_blas_gemm(
	FunctionX(XLL_FPX, _T("?xll_blas_gemm"), _T("BLAS.GEMM"))
	.Arg(XLL_FPX, _T("a"), _T("is a matrix."))
	.Arg(XLL_FPX, _T("b"), _T("is a matrix."))
	.Arg(XLL_DOUBLEX, _T("_alpha"), _T("is an optional number. Default is 1."))
	.Category(CATEGORY)
	.FunctionHelp(_T("Computes a matrix-matrix product using a general matrix."))
	.Documentation(
		_T("The <codeInline>BLAS.GEMM</codeInline> routine performs a matrix-matrix operation defined as ")
		_T("<math>alpha*a*b</math>. ")
	)
);
xfp* WINAPI xll_blas_gemm(xfp* a, xfp* b, double alpha)
{
#pragma XLLEXPORT
	static FPX c;

	try {
		static char trans('T');
		int m = a->rows;
		int n = b->columns;
		int k = a->columns;

		ensure (k == b->rows);

		c.resize(a->rows, b->columns);
		c = 0;

		if (alpha == 0)
			alpha = 1;

		cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, m, n, k, alpha, a->array, k, b->array, n, 0, c.array(), m);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return c.get();
}

#ifdef _DEBUG

void xll_test_gemm()
{
	OPERX o;

	// 2 x 3 . 3 x 2
	o = EX("=BLAS.GEMM({1,2,3 ; 4,5,6}, {7,8 ; 9,10 ; 11,12})");
	ensure (o.rows() == 2);
	ensure (o.columns() == 2);
	ensure (o(0,0) == 1*7 + 2*9 + 3*11);
	ensure (o(1,1) == 4*8 + 5*10 + 6*12);
}

#endif // _DEBUG

#ifdef _DEBUG

int xll_test_blas()
{
	try {
		xll_test_asum();
		xll_test_gemv();
		xll_test_gemm();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return 1;
}
static Auto<OpenAfterX> xao_test_blas(xll_test_blas);

#endif // _DEBUG