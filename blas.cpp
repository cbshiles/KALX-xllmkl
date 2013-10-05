// blas.cpp - Basic Linear Algebra Stuff.
// Copyright (c) KALX, LLC. All rights reserved. No warranty is made.
#include "xllmkl.h"

using namespace xll;

static AddInX xai_blas_asum(
	FunctionX(XLL_DOUBLEX, _T("?xll_blas_asum"), _T("BLAS.ASUM"))
	.Arg(XLL_FPX, _T("Vector"), _T("is a one dimensional array of floating point numbers."))
	.Arg(XLL_LONGX, _T("_Incr"), _T("is the optional increment for Vector. Default is 1. "))
	.Category(CATEGORY)
	.FunctionHelp(_T("Computes the sum of magnitudes of the vector elements."))
	.Documentation(
		_T("The <codeInline>BLAS.ASUM</codeInline> routine computes the sum of the magnitudes of elements of <codeInline>Vector</codeInline>: ")
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

		res = DASUM(&n, px_->array, &incr);
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

int xll_test_blas()
{
	try {
		xll_test_asum();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return 1;
}
static Auto<OpenAfterX> xao_test_blas(xll_test_blas);

#endif // _DEBUG