// vsl_ss_matrix_enum.cpp - VSL_SS_MATRIX_* enumerations
// Copyright (c) KALX, LLC. All rights reserved. No warranty is made.
#include "xllmkl.h"

using namespace xll;

// Matrix storage
XLL_ENUM_DOC(VSL_SS_MATRIX_STORAGE_ROWS, VSL_SS_MATRIX_STORAGE_ROWS, CATEGORY, _T("Row major order."), _T("Observations of the random vector are stored in rows, that is, i-th row of the matrix of observations contains values of i-th component of the random vector. "))
XLL_ENUM_DOC(VSL_SS_MATRIX_STORAGE_COLS, VSL_SS_MATRIX_STORAGE_COLS, CATEGORY, _T("Column major order."), _T("Observations of the random vector are stored in columns that is, i-th column of the matrix of observations contains values of i-th component of the random vector. "))
XLL_ENUM_DOC(VSL_SS_MATRIX_STORAGE_FULL, VSL_SS_MATRIX_STORAGE_FULL, CATEGORY, _T("Whole matrix is stored."), _T(""))
XLL_ENUM_DOC(VSL_SS_MATRIX_STORAGE_L_PACKED, VSL_SS_MATRIX_STORAGE_L_PACKED, CATEGORY, _T("Lower triangular."), _T("The lower triangular matrix is stored as 1-dimensional array. "))
XLL_ENUM_DOC(VSL_SS_MATRIX_STORAGE_U_PACKED, VSL_SS_MATRIX_STORAGE_U_PACKED, CATEGORY, _T("Upper triangular."), _T("The upper triangular matrix is stored as 1-dimensional array. "))
