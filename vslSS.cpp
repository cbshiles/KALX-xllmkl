// vslSS.cpp - Summary Statistics
// Copyright (c) KALX, LLC. All rights reserved. No warranty is made.
#include "xllmkl.h"

using namespace xll;

//static Auto<OpenX> xao_debug([](void) -> int { _crtBreakAlloc = 7245; return 1; });

#ifdef _DEBUG
static AddInX xai_vlsSSTask(
	FunctionX(XLL_HANDLEX, _T("?xll_vslSSTask"), _T("VSL.SSTask"))
	.Arg(XLL_FPX, _T("Data"), _T("is an array of numeric data."))
	.Arg(XLL_LPOPERX, _T("_Weights"), _T("is an optional array of weights for each column of observations."))
	.Arg(XLL_LPOPERX, _T("_Indices"), _T("is an optional mask to use for each column of observations. "))
	.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp(_T("Return a handle to an MKL Summary Statistics task."))
	.Documentation(_T(""))
);
HANDLEX WINAPI xll_vslSSTask(const  xfp* pdata, const LPOPERX pWeights, const LPOPERX pIndices)
{
#pragma XLLEXPORT
	handlex h;

	try {
		std::vector<double> weights;
		std::vector<MKL_INT> indices;

		if (pWeights->xltype == xltypeMulti) {
			ensure (pWeights->size() == pdata->columns);
			std::copy(pWeights->begin(), pWeights->end(), std::back_inserter(weights));
		}
		else {
			ensure (pWeights->xltype == xltypeMissing);
		}

		if (pIndices->xltype == xltypeMulti) {
			ensure (pIndices->size() == pdata->columns);
			std::transform(pIndices->begin(), pIndices->end(), std::back_inserter(indices), [](const OPERX& x) { return static_cast<MKL_INT>(x.val.num); });
		}
		else { 
			ensure (pWeights->xltype == xltypeMissing);
		}

		// pdata->array goes out of scope and causes memory errors!!!
		handle<xll::SSTask> h_(new xll::SSTask(pdata->columns, pdata->rows, pdata->array, weights.size() ? &weights[0] : 0, indices.size() ? &indices[0] : 0));
		ensure (h_);

		h = h_.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return h;
}

static AddInX xai_vlsSSTaskEdit(
	FunctionX(XLL_HANDLEX, _T("?xll_vslSSTaskEdit"), _T("VSL.SSTaskEdit"))
	.Arg(XLL_HANDLEX, _T("Handle"), _T("is a handle returned by VSL.SSTask."))
	.Arg(XLL_FPX, _T("Param"), _T("is an array of values from the VSL_SS_ED_* enumeration."))
	.Arg(XLL_LONGX, _T("_Storage"), _T("is an optional storage type from the VSL_SS_MATRIX_STORAGE_* enumeration. "))
	.Category(CATEGORY)
	.FunctionHelp(_T("Modifies address of an input/output parameter in the task Handle."))
	.Documentation(_T(""))
);
HANDLEX WINAPI xll_vslSSTaskEdit(HANDLEX h, const xfp* param)
{
#pragma XLLEXPORT
	try {
		handle<xll::SSTask> h_(h);
		ensure (h_);

		for (xword i = 0; i < size(*param); ++i) {
			int pi = static_cast<MKL_INT>(index(*param, i));

			ensure (VSL_STATUS_OK == h_->Compute(pi, 0));
		}
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return h;
}

static AddInX xai_vlsSSTaskCompute(
	FunctionX(XLL_HANDLEX, _T("?xll_vslSSTaskCompute"), _T("VSL.SSTaskCompute"))
	.Arg(XLL_HANDLEX, _T("Handle"), _T("is a handle returned by VSL.SSTask."))
	.Arg(XLL_LONGX, _T("Estimate"), _T("is a bitmask of values from the VSL_SS_* enumeration."))
	.Arg(XLL_LONGX, _T("Method"), _T("is the method from the VSL_SS_METHOD_* enumeration. "))
	.Category(CATEGORY)
	.FunctionHelp(_T("Computes Summary Statistics estimates."))
	.Documentation(_T(""))
);
HANDLEX WINAPI xll_vslSSTaskCompute(HANDLEX h, LONG param, LONG method)
{
#pragma XLLEXPORT
	try {
		MKL_INT status;
		handle<xll::SSTask> h_(h);
		ensure (h_);

		status = h_->Compute(param, method);
		ensure (status == VSL_STATUS_OK);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return h;
}

#endif // _DEBUG

static AddInX xai_vlsSSTaskResult(
	FunctionX(XLL_FPX, _T("?xll_vslSSTaskResult"), _T("VSL.SSTaskResult"))
	.Arg(XLL_HANDLEX, _T("Handle"), _T("is a handle returned by VSL.SSTask."))
	.Arg(XLL_LONGX, _T("Param"), _T("is a value from the VSL_SS_ED_* enumeration."))
	.Category(CATEGORY)
	.FunctionHelp(_T("Returns the result of a computation."))
	.Documentation(_T(""))
	.Alias(_T("VSL.SSResult"))
);
const xfp* WINAPI xll_vslSSTaskResult(HANDLEX h, LONG param)
{
#pragma XLLEXPORT
	const xfp* p(0);

	try {
		handle<xll::SSTask> h_(h);
		ensure (h_);

		p = h_->Result(param);

	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return p;
}

static AddInX xai_vlsSSCompute(
	FunctionX(XLL_HANDLEX, _T("?xll_vslSSCompute"), _T("VSL.SSCompute"))
	.Arg(XLL_FPX, _T("Data"), _T("is an array of data."))
	.Arg(XLL_FPX, _T("Param"), _T("is an array of values from the VSL_SS_ED_* enumeration."))
	.Arg(XLL_LONGX, _T("Method"), _T("is a value from the VSL_SS_METHOD_* enumeration. "))
//	.Arg(XLL_FPX, _T("_Storage"), _T("is an optional storage type from the VSL_SS_MATRIX_STORAGE_* enumeration. "))
	.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp(_T("Returns a handle that can be queried by VSL.SSResult."))
	.Documentation(_T(""))
);
HANDLEX WINAPI xll_vslSSCompute(const xfp* data, const xfp* param, LONG method)
{
#pragma XLLEXPORT
	handlex h;

	try {
		handle<xll::SSTask> h_(new xll::SSTask(data->columns, data->rows, data->array));
		ensure (h_);

		if (!method)
			method = VSL_SS_METHOD_FAST;


		for (xword i = 0; i < size(*param); ++i) {
			int pi = static_cast<MKL_INT>(index(*param, i));

			ensure (VSL_STATUS_OK == h_->Compute(pi, method));
		}

		h = h_.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return h;
}
