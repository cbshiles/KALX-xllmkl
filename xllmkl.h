// xllmkl.h - Wrappers for the Intel Math Kernel Library
// Copyright (c) KALX, LLC. All rights reserved. No warranty is made.
// Uncomment the following line to use features for Excel2007 and above.
#pragma once
#include "vslSS.h"
//#define EXCEL12
#include "xll/xll.h"

#ifndef CATEGORY
#define CATEGORY _T("MKL")
#endif

typedef xll::traits<XLOPERX>::xcstr xcstr; // pointer to const string
typedef xll::traits<XLOPERX>::xword xword; // use for OPER and FP indices
typedef xll::traits<XLOPERX>::xfp xfp;

extern int vsl::status;

namespace xll {

	// convert enum strings to int
	inline MKL_INT Enum_(const OPERX& e)
	{
		OPERX e_(e);

		if (e_.xltype == xltypeStr)
			e_ = ExcelX(xlfEvaluate, ExcelX(xlfConcatenate, OPERX(_T("=")), e_, OPERX(_T("()"))));
		else if (e_.xltype == xltypeMissing)
			e_ = 0;

		ensure (e_.xltype == xltypeNum);

		return static_cast<MKL_INT>(e_.val.num);
	}

	// get pointer to array if in memory
	inline xfp* fetch(xfp* pa)
	{
		xfp* pa_(0);

		if (size(*pa) == 1) {
			xll::handle<FPX> a(pa->array[0]);
			ensure (a);

			pa_ = reinterpret_cast<xfp*>(a.ptr());
		}
		else {
			pa_ = pa;
		}
		
		return pa_;
	}

} // namespace xll