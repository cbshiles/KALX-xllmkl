// vsl_ss_enum.cpp - VSL_SS_* enumerations for Compute
// Copyright (c) KALX, LLC. All rights reserved. No warranty is made.
#include "xllmkl.h"

using namespace xll;

#if 0
// No longer needed.
// Compute parameter enums
XLL_ENUM_DOC(VSL_SS_MEAN, VSL_SS_MEAN, CATEGORY, _T("Computes the array of means."), _T("Supports block observations."))
//XLL_ENUM_DOC(VSL_SS_SUM, VSL_SS_SUM, CATEGORY, _T("Computes the array of sums."), _T("Supports block observations."))
XLL_ENUM_DOC(VSL_SS_2R_MOM, VSL_SS_2R_MOM, CATEGORY, _T("Computes the array of the 2nd order raw moments."), _T("Supports block observations."))
//XLL_ENUM_DOC(VSL_SS_2R_SUM, VSL_SS_2R_SUM, CATEGORY, _T("Computes the array of raw sums of the 2nd order."), _T("Supports block observations."))
XLL_ENUM_DOC(VSL_SS_3R_MOM, VSL_SS_3R_MOM, CATEGORY, _T("Computes the array of the 3rd order raw moments."), _T("Supports block observations."))
//XLL_ENUM_DOC(VSL_SS_3R_SUM, VSL_SS_3R_SUM, CATEGORY, _T("Computes the array of raw sums of the 3rd order."), _T("Supports block observations."))
XLL_ENUM_DOC(VSL_SS_4R_MOM, VSL_SS_4R_MOM, CATEGORY, _T("Computes the array of the 4th order raw moments."), _T("Supports block observations."))
//XLL_ENUM_DOC(VSL_SS_4R_SUM, VSL_SS_4R_SUM, CATEGORY, _T("Computes the array of raw sums of the 4th order."), _T("Supports block observations."))
XLL_ENUM_DOC(VSL_SS_2C_MOM, VSL_SS_2C_MOM, CATEGORY, _T("Computes the array of the 2nd order central moments."), _T("Supports block observations."))
//XLL_ENUM_DOC(VSL_SS_2C_SUM, VSL_SS_2C_SUM, CATEGORY, _T("Computes the array of central sums of the 2nd order."), _T("Supports block observations."))
XLL_ENUM_DOC(VSL_SS_3C_MOM, VSL_SS_3C_MOM, CATEGORY, _T("Computes the array of the 3rd order central moments."), _T("Supports block observations."))
//XLL_ENUM_DOC(VSL_SS_3C_SUM, VSL_SS_3C_SUM, CATEGORY, _T("Computes the array of central sums of the 3rd order."), _T("Supports block observations."))
XLL_ENUM_DOC(VSL_SS_4C_MOM, VSL_SS_4C_MOM, CATEGORY, _T("Computes the array of the 4th order central moments."), _T("Supports block observations."))
//XLL_ENUM_DOC(VSL_SS_4C_SUM, VSL_SS_4C_SUM, CATEGORY, _T("Computes the array of central sums of the 4th order."), _T("Supports block observations."))
XLL_ENUM_DOC(VSL_SS_KURTOSIS, VSL_SS_KURTOSIS, CATEGORY, _T("Computes the array of kurtosis values."), _T("Supports block observations."))
XLL_ENUM_DOC(VSL_SS_SKEWNESS, VSL_SS_SKEWNESS, CATEGORY, _T("Computes the array of skewness values."), _T("Supports block observations."))
XLL_ENUM_DOC(VSL_SS_MIN, VSL_SS_MIN, CATEGORY, _T("Computes the array of minimum values."), _T("Supports block observations."))
XLL_ENUM_DOC(VSL_SS_MAX, VSL_SS_MAX, CATEGORY, _T("Computes the array of maximum values."), _T("Supports block observations."))
XLL_ENUM_DOC(VSL_SS_VARIATION, VSL_SS_VARIATION, CATEGORY, _T("Computes the array of variation coefficients."), _T("Supports block observations."))
XLL_ENUM_DOC(VSL_SS_COV, VSL_SS_COV, CATEGORY, _T("Computes a covariance matrix."), _T("Supports block observations."))
XLL_ENUM_DOC(VSL_SS_COR, VSL_SS_COR, CATEGORY, _T("Computes a correlation matrix. The main diagonal of the correlation matrix holds variances of the random vector components."), _T("Supports block observations."))
//XLL_ENUM_DOC(VSL_SS_CP, VSL_SS_CP, CATEGORY, _T("Computes a cross-product matrix."), _T("Supports block observations."))
XLL_ENUM_DOC(VSL_SS_POOLED_COV, VSL_SS_POOLED_COV, CATEGORY, _T("Computes a pooled covariance matrix."), _T("Does not support block observations."))
XLL_ENUM_DOC(VSL_SS_GROUP_COV, VSL_SS_GROUP_COV, CATEGORY, _T("Computes group covariance matrices."), _T("Does not support block observations."))
XLL_ENUM_DOC(VSL_SS_QUANTS, VSL_SS_QUANTS, CATEGORY, _T("Computes quantiles."), _T("Does not support block observations."))
XLL_ENUM_DOC(VSL_SS_ORDER_STATS, VSL_SS_ORDER_STATS, CATEGORY, _T("Computes order statistics."), _T("Does not support block observations."))
XLL_ENUM_DOC(VSL_SS_ROBUST_COV, VSL_SS_ROBUST_COV, CATEGORY, _T("Computes a robust covariance matrix."), _T("Does not support block observations."))
XLL_ENUM_DOC(VSL_SS_OUTLIERS, VSL_SS_OUTLIERS, CATEGORY, _T("Detects outliers in the dataset."), _T("Does not support block observations."))
XLL_ENUM_DOC(VSL_SS_PARTIAL_COV, VSL_SS_PARTIAL_COV, CATEGORY, _T("Computes a partial covariance matrix."), _T("Does not support block observations."))
XLL_ENUM_DOC(VSL_SS_PARTIAL_COR, VSL_SS_PARTIAL_COR, CATEGORY, _T("Computes a partial correlation matrix."), _T("Does not support block observations."))
XLL_ENUM_DOC(VSL_SS_MISSING_VALS, VSL_SS_MISSING_VALS, CATEGORY, _T("Supports missing values in datasets."), _T("Does not support block observations."))
XLL_ENUM_DOC(VSL_SS_PARAMTR_COR, VSL_SS_PARAMTR_COR, CATEGORY, _T("Computes a parameterized correlation matrix."), _T("Does not support block observations."))
XLL_ENUM_DOC(VSL_SS_STREAM_QUANTS, VSL_SS_STREAM_QUANTS, CATEGORY, _T("Computes quantiles for streaming data."), _T("Supports block observations."))
//XLL_ENUM_DOC(VSL_SS_MDAD, VSL_SS_MDAD, CATEGORY, _T("Computes median absolute deviation."), _T("Does not support block observations."))
//XLL_ENUM_DOC(VSL_SS_MNAD, VSL_SS_MNAD, CATEGORY, _T("Computes mean absolute deviation."), _T("Does not support block observations."))

#endif // 0

// Compute method enums
XLL_ENUM_DOC(VSL_SS_METHOD_FAST, VSL_SS_METHOD_FAST, CATEGORY, _T("Fast method for calculation of the estimates:"), _T("raw/central moments/sums, skewness, kurtosis, variation, variance-covariance/correlation/cross-product matrix"))
XLL_ENUM_DOC(VSL_SS_METHOD_FAST_USER_MEAN, VSL_SS_METHOD_FAST_USER_MEAN, CATEGORY, _T("Fast method for calculation of the estimates given user-defined mean:"), _T("central moments/sums of 2-4 order, skewness, kurtosis, variation, variance-covariance/correlation/cross-product matrix, mean absolute deviation"))
XLL_ENUM_DOC(VSL_SS_METHOD_1PASS, VSL_SS_METHOD_1PASS, CATEGORY, _T("One-pass method for calculation of estimates:"), _T("raw/central moments/sums, skewness, kurtosis, variation, variance-covariance/correlation/cross-product matrix"))
XLL_ENUM_DOC(VSL_SS_METHOD_TBS, VSL_SS_METHOD_TBS, CATEGORY, _T("TBS method for robust estimation of covariance and mean"), _T(""))
XLL_ENUM_DOC(VSL_SS_METHOD_BACON, VSL_SS_METHOD_BACON, CATEGORY, _T("BACON method for detection of multivariate outliers"), _T(""))
XLL_ENUM_DOC(VSL_SS_METHOD_MI, VSL_SS_METHOD_MI, CATEGORY, _T("Multiple imputation method for support of missing values"), _T(""))
XLL_ENUM_DOC(VSL_SS_METHOD_SD, VSL_SS_METHOD_SD, CATEGORY, _T("Spectral decomposition method for parameterization of a correlation matrix"), _T(""))
XLL_ENUM_DOC(VSL_SS_METHOD_SQUANTS_ZW, VSL_SS_METHOD_SQUANTS_ZW, CATEGORY, _T("Zhang-Wang (ZW) method for quantile estimation for streaming data"), _T(""))
XLL_ENUM_DOC(VSL_SS_METHOD_SQUANTS_ZW_FAST, VSL_SS_METHOD_SQUANTS_ZW_FAST, CATEGORY, _T("Fast ZW method for quantile estimation for streaming data"), _T(""))
