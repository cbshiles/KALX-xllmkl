// vslSS.cpp - Summary Statistics
// Copyright (c) KALX, LLC. All rights reserved. No warranty is made.
#include "xllmkl.h"
#include "vslSS.h"

using namespace xll;

namespace xll {
	struct SSTask : vsl::SSTask<> {
		std::map<MKL_INT,FPX> map; // results of tasks
		SSTask(MKL_INT nvar, MKL_INT nobs, const double* data, const double* weights = 0, const MKL_INT* indices = 0)
			: vsl::SSTask<>(nvar, nobs, data, weights, indices)
		{ }
	};
}

static Auto<OpenX> xao_debug([/*&_CrtDbgBreak*/](void) -> int { /*_CrtDbgBreak = 6747;*/ return 1; });

// EditTask enums
XLL_ENUM_DOC(VSL_SS_ED_DIMEN, VSL_SS_ED_DIMEN, CATEGORY, _T("Address of a variable that holds the task dimension."), _T("Required. Positive integer value."))
XLL_ENUM_DOC(VSL_SS_ED_OBSERV_N, VSL_SS_ED_OBSERV_N, CATEGORY, _T("Address of a variable that holds the number of observations."), _T("Required. Positive integer value."))
XLL_ENUM_DOC(VSL_SS_ED_OBSERV, VSL_SS_ED_OBSERV, CATEGORY, _T("Address of the observation matrix."), _T("Required. Provide the matrix containing your observations."))
XLL_ENUM_DOC(VSL_SS_ED_OBSERV_STORAGE, VSL_SS_ED_OBSERV_STORAGE, CATEGORY, _T("Address of a variable that holds the storage format for the observation matrix."), _T("Required. Provide a storage format supported by the library whenever you pass a matrix of observations.1"))
XLL_ENUM_DOC(VSL_SS_ED_INDC, VSL_SS_ED_INDC, CATEGORY, _T("Address of the array of indices."), _T("Optional. Provide this array if you need to process individual components of the random vector. Set entry i of the array to one to include the ith coordinate in the analysis. Set entry i of the array to zero to exclude the ith coordinate from the analysis."))
XLL_ENUM_DOC(VSL_SS_ED_WEIGHTS, VSL_SS_ED_WEIGHTS, CATEGORY, _T("Address of the array of observation weights."), _T("Optional. If the observations have weights different from the default weight (one), set entries of the array to non-negative floating point values."))
XLL_ENUM_DOC(VSL_SS_ED_MEAN, VSL_SS_ED_MEAN, CATEGORY, _T("Address of the array of means."), _T("Optional. Set entries of the array to meaningful values (typically zero) if you intend to compute a progressive estimate. Otherwise, do not initialize the array."))
XLL_ENUM_DOC(VSL_SS_ED_2R_MOM, VSL_SS_ED_2R_MOM, CATEGORY, _T("Address of an array of raw moments of the second order."), _T("Optional. Set entries of the array to meaningful values (typically zero) if you intend to compute a progressive estimate. Otherwise, do not initialize the array."))
XLL_ENUM_DOC(VSL_SS_ED_3R_MOM, VSL_SS_ED_3R_MOM, CATEGORY, _T("Address of an array of raw moments of the third order."), _T("Optional. Set entries of the array to meaningful values (typically zero) if you intend to compute a progressive estimate. Otherwise, do not initialize the array."))
XLL_ENUM_DOC(VSL_SS_ED_4R_MOM, VSL_SS_ED_4R_MOM, CATEGORY, _T("Address of an array of raw moments of the fourth order."), _T("Optional. Set entries of the array to meaningful values (typically zero) if you intend to compute a progressive estimate. Otherwise, do not initialize the array."))
XLL_ENUM_DOC(VSL_SS_ED_2C_MOM, VSL_SS_ED_2C_MOM, CATEGORY, _T("Address of an array of central moments of the second order."), _T("Optional. Set entries of the array to meaningful values (typically zero) if you intend to compute a progressive estimate. Otherwise, do not initialize the array. Make sure you also provide arrays for raw moments of the first and second order."))
XLL_ENUM_DOC(VSL_SS_ED_3C_MOM, VSL_SS_ED_3C_MOM, CATEGORY, _T("Address of an array of central moments of the third order."), _T("Optional. Set entries of the array to meaningful values (typically zero) if you intend to compute a progressive estimate. Otherwise, do not initialize the array. Make sure you also provide arrays for raw moments of the first, second, and third order."))
XLL_ENUM_DOC(VSL_SS_ED_4C_MOM, VSL_SS_ED_4C_MOM, CATEGORY, _T("Address of an array of central moments of the fourth order."), _T("Optional. Set entries of the array to meaningful values (typically zero) if you intend to compute a progressive estimate. Otherwise, do not initialize the array. Make sure you also provide arrays for raw moments of the first, second, third, and fourth order."))
XLL_ENUM_DOC(VSL_SS_ED_KURTOSIS, VSL_SS_ED_KURTOSIS, CATEGORY, _T("Address of the array of kurtosis estimates."), _T("Optional. Set entries of the array to meaningful values (typically zero) if you intend to compute a progressive estimate. Otherwise, do not initialize the array. Make sure you also provide arrays for raw moments of the first, second, third, and fourth order."))
XLL_ENUM_DOC(VSL_SS_ED_SKEWNESS, VSL_SS_ED_SKEWNESS, CATEGORY, _T("Address of the array of skewness estimates."), _T("Optional. Set entries of the array to meaningful values (typically zero) if you intend to compute a progressive estimate. Otherwise, do not initialize the array. Make sure you also provide arrays for raw moments of the first, second, and third order."))
XLL_ENUM_DOC(VSL_SS_ED_MIN, VSL_SS_ED_MIN, CATEGORY, _T("Address of the array of minimum estimates."), _T("Optional. Set entries of array to meaningful values, such as the values of the first observation."))
XLL_ENUM_DOC(VSL_SS_ED_MAX, VSL_SS_ED_MAX, CATEGORY, _T("Address of the array of maximum estimates."), _T("Optional. Set entries of array to meaningful values, such as the values of the first observation."))
XLL_ENUM_DOC(VSL_SS_ED_VARIATION, VSL_SS_ED_VARIATION, CATEGORY, _T("Address of the array of variation coefficients."), _T("Optional. Set entries of the array to meaningful values (typically zero) if you intend to compute a progressive estimate. Otherwise, do not initialize the array. Make sure you also provide arrays for raw moments of the first and second order."))
XLL_ENUM_DOC(VSL_SS_ED_COV, VSL_SS_ED_COV, CATEGORY, _T("Address of a covariance matrix."), _T("Optional. Set entries of the array to meaningful values (typically zero) if you intend to compute a progressive estimate. Make sure you also provide an array for the mean."))
XLL_ENUM_DOC(VSL_SS_ED_COV_STORAGE, VSL_SS_ED_COV_STORAGE, CATEGORY, _T("Address of the variable that holds the storage format for a covariance matrix."), _T("Required. Provide a storage format supported by the library whenever you intend to compute the covariance matrix.2"))
XLL_ENUM_DOC(VSL_SS_ED_COR, VSL_SS_ED_COR, CATEGORY, _T("Address of a correlation matrix."), _T("Optional. Set entries of the array to meaningful values (typically zero) if you intend to compute a progressive estimate. If you initialize the matrix in non-trivial way, make sure that the main diagonal contains variance values. Also, provide an array for the mean."))
XLL_ENUM_DOC(VSL_SS_ED_COR_STORAGE, VSL_SS_ED_COR_STORAGE, CATEGORY, _T("Address of the variable that holds the correlation storage format for a correlation matrix."), _T("Required. Provide a storage format supported by the library whenever you intend to compute the correlation matrix.2"))
XLL_ENUM_DOC(VSL_SS_ED_ACCUM_WEIGHT, VSL_SS_ED_ACCUM_WEIGHT, CATEGORY, _T("Address of the array of size 2 that holds the accumulated weight (sum of weights) in the first position and the sum of weights squared in the second position."), _T("Optional. Set the entries of the matrix to meaningful values (typically zero) if you intend to do progressive processing of the dataset or need the sum of weights and sum of squared weights assigned to observations."))
XLL_ENUM_DOC(VSL_SS_ED_QUANT_ORDER_N, VSL_SS_ED_QUANT_ORDER_N, CATEGORY, _T("Address of the variable that holds the number of quantile orders."), _T("Required. Positive integer value. Provide the number of quantile orders whenever you compute quantiles."))
XLL_ENUM_DOC(VSL_SS_ED_QUANT_ORDER, VSL_SS_ED_QUANT_ORDER, CATEGORY, _T("Address of the array of quantile orders."), _T("Required. Set entries of array to values from the interval (0,1). Provide this parameter whenever you compute quantiles."))
XLL_ENUM_DOC(VSL_SS_ED_QUANT_QUANTILES, VSL_SS_ED_QUANT_QUANTILES, CATEGORY, _T("Address of the array of quantiles."), _T("None."))
XLL_ENUM_DOC(VSL_SS_ED_ORDER_STATS, VSL_SS_ED_ORDER_STATS, CATEGORY, _T("Address of the array of order statistics."), _T("None."))
XLL_ENUM_DOC(VSL_SS_ED_GROUP_INDC, VSL_SS_ED_GROUP_INDC, CATEGORY, _T("Address of the array of group indices used in computation of a pooled covariance matrix."), _T("Required. Set entry i to integer value k if the observation belongs to group k. Values of k take values in the range [0, g-1], where g is the number of groups."))
XLL_ENUM_DOC(VSL_SS_ED_POOLED_COV_STORAGE, VSL_SS_ED_POOLED_COV_STORAGE, CATEGORY, _T("Address of a variable that holds the storage format for a pooled covariance matrix."), _T("Required. Provide a storage format supported by the library whenever you intend to compute pooled covariance.2"))
XLL_ENUM_DOC(VSL_SS_ED_POOLED_MEAN, VSL_SS_ED_POOLED_MEAN, CATEGORY, _T("Address of an array of pooled means."), _T("None."))
XLL_ENUM_DOC(VSL_SS_ED_POOLED_COV, VSL_SS_ED_POOLED_COV, CATEGORY, _T("Address of pooled covariance matrices."), _T("None."))
XLL_ENUM_DOC(VSL_SS_ED_GROUP_COV_INDC, VSL_SS_ED_GROUP_COV_INDC, CATEGORY, _T("Address of an array of indices for which covariance/means should be computed."), _T("Optional. Set the kth entry of the array to 1 if you need group covariance and mean for group k; otherwise set it to zero."))
XLL_ENUM_DOC(VSL_SS_ED_GROUP_MEAN, VSL_SS_ED_GROUP_MEAN, CATEGORY, _T("Address of an array of group means."), _T("None."))
XLL_ENUM_DOC(VSL_SS_ED_GROUP_COV_STORAGE, VSL_SS_ED_GROUP_COV_STORAGE, CATEGORY, _T("Address of a variable that holds the storage format for a group covariance matrix."), _T("Required. Provide a storage format supported by the library whenever you intend to get group covariance.2"))
XLL_ENUM_DOC(VSL_SS_ED_GROUP_COV, VSL_SS_ED_GROUP_COV, CATEGORY, _T("Address of group covariance matrices."), _T("None."))
XLL_ENUM_DOC(VSL_SS_ED_ROBUST_COV_STORAGE, VSL_SS_ED_ROBUST_COV_STORAGE, CATEGORY, _T("Address of a variable that holds the storage format for a robust covariance matrix."), _T("Required. Provide a storage format supported by the library whenever you compute robust covariance2."))
XLL_ENUM_DOC(VSL_SS_ED_ROBUST_COV_PARAMS_N, VSL_SS_ED_ROBUST_COV_PARAMS_N, CATEGORY, _T("Address of a variable that holds the number of algorithmic parameters of the method for robust covariance estimation."), _T("Required. Set to the number of TBS parameters,VSL_SS_TBS_PARAMS_N."))
XLL_ENUM_DOC(VSL_SS_ED_ROBUST_COV_PARAMS, VSL_SS_ED_ROBUST_COV_PARAMS, CATEGORY, _T("Address of an array of parameters of the method for robust estimation of a covariance."), _T("Required. Set the entries of the array according to the description invslSSEditRobustCovariance."))
XLL_ENUM_DOC(VSL_SS_ED_ROBUST_MEAN, VSL_SS_ED_ROBUST_MEAN, CATEGORY, _T("Address of an array of robust means."), _T("None."))
XLL_ENUM_DOC(VSL_SS_ED_ROBUST_COV, VSL_SS_ED_ROBUST_COV, CATEGORY, _T("Address of a robust covariance matrix."), _T("None."))
XLL_ENUM_DOC(VSL_SS_ED_OUTLIERS_PARAMS_N, VSL_SS_ED_OUTLIERS_PARAMS_N, CATEGORY, _T("Address of a variable that holds the number of parameters of the outlier detection method."), _T("Required. Set to the number of outlier detection parameters,VSL_SS_BACON_PARAMS_N."))
XLL_ENUM_DOC(VSL_SS_ED_OUTLIERS_PARAMS, VSL_SS_ED_OUTLIERS_PARAMS, CATEGORY, _T("Address of an array of algorithmic parameters for the outlier detection method."), _T("Required. Set the entries of the array according to the description inEditOutliersDetection."))
XLL_ENUM_DOC(VSL_SS_ED_OUTLIERS_WEIGHT, VSL_SS_ED_OUTLIERS_WEIGHT, CATEGORY, _T("Address of an array of weights assigned to observations by the outlier detection method."), _T("None."))
XLL_ENUM_DOC(VSL_SS_ED_ORDER_STATS_STORAGE, VSL_SS_ED_ORDER_STATS_STORAGE, CATEGORY, _T("Address of a variable that holds the storage format of an order statistics matrix."), _T("Required. Provide a storage format supported by the library whenever you compute a matrix of order statistics.1"))
XLL_ENUM_DOC(VSL_SS_ED_PARTIAL_COV_IDX, VSL_SS_ED_PARTIAL_COV_IDX, CATEGORY, _T("Address of an array that encodes subcomponents of a random vector."), _T("Required. Set the entries of the array according to the description invslSSEditPartialCovCor."))
XLL_ENUM_DOC(VSL_SS_ED_PARTIAL_COV, VSL_SS_ED_PARTIAL_COV, CATEGORY, _T("Address of a partial covariance matrix."), _T("None."))
XLL_ENUM_DOC(VSL_SS_ED_PARTIAL_COV_STORAGE, VSL_SS_ED_PARTIAL_COV_STORAGE, CATEGORY, _T("Address of a variable that holds the storage format of a partial covariance matrix."), _T("Required. Provide a storage format supported by the library whenever you compute the partial covariance.2"))
XLL_ENUM_DOC(VSL_SS_ED_PARTIAL_COR, VSL_SS_ED_PARTIAL_COR, CATEGORY, _T("Address of a partial correlation matrix."), _T("None."))
XLL_ENUM_DOC(VSL_SS_ED_PARTIAL_COR_STORAGE, VSL_SS_ED_PARTIAL_COR_STORAGE, CATEGORY, _T("Address of a variable that holds the storage format for a partial correlation matrix."), _T("Required. Provide a storage format supported by the library whenever you compute the partial correlation.2"))
XLL_ENUM_DOC(VSL_SS_ED_MI_PARAMS_N, VSL_SS_ED_MI_PARAMS_N, CATEGORY, _T("Address of a variable that holds the number of algorithmic parameters for the Multiple Imputation method."), _T("Required. Set to the number of MI parameters,VSL_SS_MI_PARAMS_SIZE."))
XLL_ENUM_DOC(VSL_SS_ED_MI_PARAMS, VSL_SS_ED_MI_PARAMS, CATEGORY, _T("Address of an array of algorithmic parameters for the Multiple Imputation method."), _T("Required. Set entries of the array according to the description inEditMissingValues."))
XLL_ENUM_DOC(VSL_SS_ED_MI_INIT_ESTIMATES_N, VSL_SS_ED_MI_INIT_ESTIMATES_N, CATEGORY, _T("Address of a variable that holds the number of initial estimates for the Multiple Imputation method."), _T("Optional. Set to p+p*(p+1)/2, wherep is the task dimension."))
XLL_ENUM_DOC(VSL_SS_ED_MI_INIT_ESTIMATES, VSL_SS_ED_MI_INIT_ESTIMATES, CATEGORY, _T("Address of an array of initial estimates for the Multiple Imputation method."), _T("Optional. Set the values of the array according to the description in 'Basic Components of the Multiple Imputation Function in Summary Statistics Library' in the Intel® MKL Summary Statistics Library Application Notes document on theIntel® MKL web page."))
XLL_ENUM_DOC(VSL_SS_ED_MI_SIMUL_VALS_N, VSL_SS_ED_MI_SIMUL_VALS_N, CATEGORY, _T("Address of a variable that holds the number of simulated values in the Multiple Imputation method."), _T("Optional. Positive integer indicating the number of missing points in the observation matrix."))
XLL_ENUM_DOC(VSL_SS_ED_MI_SIMUL_VALS, VSL_SS_ED_MI_SIMUL_VALS, CATEGORY, _T("Address of an array of simulated values in the Multiple Imputation method."), _T("None."))
XLL_ENUM_DOC(VSL_SS_ED_MI_ESTIMATES_N, VSL_SS_ED_MI_ESTIMATES_N, CATEGORY, _T("Address of a variable that holds the number of estimates obtained as a result of the Multiple Imputation method."), _T("Optional. Positive integer number defined according to the description in 'Basic Components of the Multiple Imputation Function in Summary Statistics Library' in theIntel® MKL Summary Statistics Library Application Notes document on the Intel® MKL web page."))
XLL_ENUM_DOC(VSL_SS_ED_MI_ESTIMATES, VSL_SS_ED_MI_ESTIMATES, CATEGORY, _T("Address of an array of estimates obtained as a result of the Multiple Imputation method."), _T("None."))
XLL_ENUM_DOC(VSL_SS_ED_MI_PRIOR_N, VSL_SS_ED_MI_PRIOR_N, CATEGORY, _T("Address of a variable that holds the number of prior parameters for the Multiple Imputation method."), _T("Optional. If you pass a user-defined array of prior parameters, set this parameter to (p2+3*p+4)/2, where pis the task dimension."))
XLL_ENUM_DOC(VSL_SS_ED_MI_PRIOR, VSL_SS_ED_MI_PRIOR, CATEGORY, _T("Address of an array of prior parameters for the Multiple Imputation method."), _T("Optional. Set entries of the array of prior parameters according to the description in 'Basic Components of the Multiple Imputation Function in Summary Statistics Library' in theIntel® MKL Summary Statistics Library Application Notes document on the Intel® MKL web page."))
XLL_ENUM_DOC(VSL_SS_ED_PARAMTR_COR, VSL_SS_ED_PARAMTR_COR, CATEGORY, _T("Address of a parameterized correlation matrix."), _T("None."))
XLL_ENUM_DOC(VSL_SS_ED_PARAMTR_COR_STORAGE, VSL_SS_ED_PARAMTR_COR_STORAGE, CATEGORY, _T("Address of a variable that holds the storage format of a parameterized correlation matrix."), _T("Required. Provide a storage format supported by the library whenever you compute the parameterized correlation matrix.2"))
XLL_ENUM_DOC(VSL_SS_ED_STREAM_QUANT_PARAMS_N, VSL_SS_ED_STREAM_QUANT_PARAMS_N, CATEGORY, _T("Address of a variable that holds the number of parameters of a quantile computation method for streaming data."), _T("Required. Set to the number of quantile computation parameters,VSL_SS_SQUANTS_ZW_PARAMS_N."))
XLL_ENUM_DOC(VSL_SS_ED_STREAM_QUANT_PARAMS, VSL_SS_ED_STREAM_QUANT_PARAMS, CATEGORY, _T("Address of an array of parameters of a quantile computation method for streaming data."), _T("Required. Set the entries of the array according to the description in'Computing Quantiles for Streaming Data' in the Intel® MKL Summary Statistics Library Application Notesdocument on the Intel® MKL web page."))
XLL_ENUM_DOC(VSL_SS_ED_STREAM_QUANT_ORDER_N, VSL_SS_ED_STREAM_QUANT_ORDER_N, CATEGORY, _T("Address of a variable that holds the number of quantile orders for streaming data."), _T("Required. Positive integer value."))
XLL_ENUM_DOC(VSL_SS_ED_STREAM_QUANT_ORDER, VSL_SS_ED_STREAM_QUANT_ORDER, CATEGORY, _T("Address of an array of quantile orders for streaming data."), _T("Required. Set entries of the array to values from the interval (0,1). Provide this parameter whenever you compute quantiles."))
XLL_ENUM_DOC(VSL_SS_ED_STREAM_QUANT_QUANTILES, VSL_SS_ED_STREAM_QUANT_QUANTILES, CATEGORY, _T("Address of an array of quantiles for streaming data."), _T("None."))
 /* undefined
XLL_ENUM_DOC(VSL_SS_ED_SUM, VSL_SS_ED_SUM, CATEGORY, _T("Address of array of sums"), _T("Optional. Set entries of the array to meaningful values (typically zero) if you intend to compute a progressive estimate. Otherwise, do not initialize the array."))
XLL_ENUM_DOC(VSL_SS_ED_2R_SUM, VSL_SS_ED_2R_SUM, CATEGORY, _T("Address of array of raw sums of 2nd order"), _T("Optional. Set entries of the array to meaningful values (typically zero) if you intend to compute a progressive estimate. Otherwise, do not initialize the array."))
XLL_ENUM_DOC(VSL_SS_ED_3R_SUM, VSL_SS_ED_3R_SUM, CATEGORY, _T("Address of array of raw sums of 3rd order"), _T("Optional. Set entries of the array to meaningful values (typically zero) if you intend to compute a progressive estimate. Otherwise, do not initialize the array."))
XLL_ENUM_DOC(VSL_SS_ED_4R_SUM, VSL_SS_ED_4R_SUM, CATEGORY, _T("Address of array of raw sums of 4th order"), _T("Optional. Set entries of the array to meaningful values (typically zero) if you intend to compute a progressive estimate. Otherwise, do not initialize the array."))
XLL_ENUM_DOC(VSL_SS_ED_2C_SUM, VSL_SS_ED_2C_SUM, CATEGORY, _T("Address of array of central sums of 2nd order"), _T("Optional. Set entries of the array to meaningful values (typically zero) if you intend to compute a progressive estimate. Otherwise, do not initialize the array."))
XLL_ENUM_DOC(VSL_SS_ED_3C_SUM, VSL_SS_ED_3C_SUM, CATEGORY, _T("Address of array of central sums of 3rd order"), _T("Optional. Set entries of the array to meaningful values (typically zero) if you intend to compute a progressive estimate. Otherwise, do not initialize the array."))
XLL_ENUM_DOC(VSL_SS_ED_4C_SUM, VSL_SS_ED_4C_SUM, CATEGORY, _T("Address of array of central sums of 4th order"), _T("Optional. Set entries of the array to meaningful values (typically zero) if you intend to compute a progressive estimate. Otherwise, do not initialize the array."))
XLL_ENUM_DOC(VSL_SS_ED_CP, VSL_SS_ED_CP, CATEGORY, _T("Address of cross-product matrix"), _T("Optional. Set entries of the array to meaningful values (typically zero) if you intend to compute a progressive estimate. Otherwise, do not initialize the array."))
XLL_ENUM_DOC(VSL_SS_ED_MDAD, VSL_SS_ED_MDAD, CATEGORY, _T("Address of array of median absolute deviations"), _T("None."))
XLL_ENUM_DOC(VSL_SS_ED_MNAD, VSL_SS_ED_MNAD, CATEGORY, _T("Address of array of mean absolute deviations"), _T("None."))
*/

// ComputeTask enums
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

// Compute method
XLL_ENUM_DOC(VSL_SS_METHOD_FAST, VSL_SS_METHOD_FAST, CATEGORY, _T("Fast method for calculation of the estimates:"), _T("raw/central moments/sums, skewness, kurtosis, variation, variance-covariance/correlation/cross-product matrix"))
XLL_ENUM_DOC(VSL_SS_METHOD_FAST_USER_MEAN, VSL_SS_METHOD_FAST_USER_MEAN, CATEGORY, _T("Fast method for calculation of the estimates given user-defined mean:"), _T("central moments/sums of 2-4 order, skewness, kurtosis, variation, variance-covariance/correlation/cross-product matrix, mean absolute deviation"))
XLL_ENUM_DOC(VSL_SS_METHOD_1PASS, VSL_SS_METHOD_1PASS, CATEGORY, _T("One-pass method for calculation of estimates:"), _T("raw/central moments/sums, skewness, kurtosis, variation, variance-covariance/correlation/cross-product matrix"))
XLL_ENUM_DOC(VSL_SS_METHOD_TBS, VSL_SS_METHOD_TBS, CATEGORY, _T("TBS method for robust estimation of covariance and mean"), _T(""))
XLL_ENUM_DOC(VSL_SS_METHOD_BACON, VSL_SS_METHOD_BACON, CATEGORY, _T("BACON method for detection of multivariate outliers"), _T(""))
XLL_ENUM_DOC(VSL_SS_METHOD_MI, VSL_SS_METHOD_MI, CATEGORY, _T("Multiple imputation method for support of missing values"), _T(""))
XLL_ENUM_DOC(VSL_SS_METHOD_SD, VSL_SS_METHOD_SD, CATEGORY, _T("Spectral decomposition method for parameterization of a correlation matrix"), _T(""))
XLL_ENUM_DOC(VSL_SS_METHOD_SQUANTS_ZW, VSL_SS_METHOD_SQUANTS_ZW, CATEGORY, _T("Zhang-Wang (ZW) method for quantile estimation for streaming data"), _T(""))
XLL_ENUM_DOC(VSL_SS_METHOD_SQUANTS_ZW_FAST, VSL_SS_METHOD_SQUANTS_ZW_FAST, CATEGORY, _T("Fast ZW method for quantile estimation for streaming data"), _T(""))


static AddInX xai_vlsSSTask(
	FunctionX(XLL_HANDLEX, _T("?xll_vslSSTask"), _T("VSL.SSTask"))
	.Arg(XLL_FPX, _T("Data"), _T("is an array of numeric data."))
	.Arg(XLL_LPOPERX, _T("_Weights"), _T("is an optional array of weights for each column of observations."))
	.Arg(XLL_LPOPERX, _T("_Mask"), _T("is an optional mask to use for each column of observations. "))
	.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp(_T("Return a handle to an MKL Summary Statistics task."))
	.Documentation(_T(""))
);
HANDLEX WINAPI xll_vslSSTask(const  xfp* pdata, const LPOPERX pWeights, const LPOPERX pMask)
{
#pragma XLLEXPORT
	handlex h;

	try {
		double* weights(0);
		MKL_INT* mask(0);

		if (pWeights->xltype == xltypeMulti) {
			ensure (pWeights->size() == pdata->columns);

			std::unique_ptr<double> w(new double[pdata->columns]);
			
			const OPERX& Weights(*pWeights);
			for (xword i = 0; i < pdata->columns; ++i) {
				ensure (Weights[i].xltype == xltypeNum);
				w.get()[i] = Weights[i].val.num;
			}
			
			weights = w.get();
		}
		else {
			ensure (pWeights->xltype == xltypeMissing);
		}

		if (pMask->xltype == xltypeMulti) {
			ensure (pMask->size() == pdata->columns);
			
			std::unique_ptr<MKL_INT> m(new MKL_INT[pdata->columns]);
			
			const OPERX& Mask(*pMask);
			for (xword i = 0; i < pdata->columns; ++i) {
				m.get()[i] = (Mask[i] != 0);
			}
			
			mask = m.get();
		} // yikes!!! delete gets called on m???
		else { 
			ensure (pWeights->xltype == xltypeMissing);
		}

		handle<xll::SSTask> h_(new xll::SSTask(pdata->columns, pdata->rows, pdata->array, weights, mask));
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
	.Category(CATEGORY)
	.FunctionHelp(_T("Modifies address of an input/output parameter in the task Handle."))
	.Documentation(_T(""))
);
HANDLEX WINAPI xll_vslSSTaskEdit(HANDLEX h, const xfp* param)
{
#pragma XLLEXPORT
	try {
		MKL_INT status(VSL_STATUS_OK);
		handle<xll::SSTask> h_(h);
		ensure (h_);

		for (xword i = 0; i < size(*param); ++i) {
			int pi = static_cast<MKL_INT>(index(*param, i));

			switch (pi) {
			case VSL_SS_ED_MEAN:
			case VSL_SS_ED_2R_MOM:
			case VSL_SS_ED_3R_MOM:
			case VSL_SS_ED_4R_MOM:
			case VSL_SS_ED_2C_MOM:
			case VSL_SS_ED_3C_MOM:
			case VSL_SS_ED_4C_MOM:
			case VSL_SS_ED_KURTOSIS:
			case VSL_SS_ED_SKEWNESS:
			case VSL_SS_ED_MIN:
			case VSL_SS_ED_MAX:
			case VSL_SS_ED_VARIATION: 
			{
				h_->map[pi] = FPX(1, static_cast<xword>(h_->dimen()));
				status = h_->Edit(pi, h_->map[pi].array());

				break;
			}
			case VSL_SS_ED_COV:
			{
				h_->map[pi] = FPX(static_cast<xword>(h_->dimen()), static_cast<xword>(h_->dimen()));
				status = h_->Edit(pi, h_->map[pi].array());

				break;
			}
			default:
				XLL_WARNING("VSL.TaskEdit: unrecognized parameter");
			}

			ensure (status == VSL_STATUS_OK);
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

static AddInX xai_vlsSSTaskResult(
	FunctionX(XLL_FPX, _T("?xll_vslSSTaskResult"), _T("VSL.SSTaskResult"))
	.Arg(XLL_HANDLEX, _T("Handle"), _T("is a handle returned by VSL.SSTask."))
	.Arg(XLL_LONGX, _T("Param"), _T("is a value from the VSL_SS_ED_* enumeration."))
	.Category(CATEGORY)
	.FunctionHelp(_T("Returns the result of a computation."))
	.Documentation(_T(""))
);
const xfp* WINAPI xll_vslSSTaskResult(HANDLEX h, LONG param)
{
#pragma XLLEXPORT
	xfp* p(0);

	try {
		handle<xll::SSTask> h_(h);
		ensure (h_);

		auto i = h_->map.find(param);
		ensure (i != h_->map.end());

		p = i->second.get();

	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return p;
}
