// xllmkl.cpp - MKL wrappers
#include <cmath>
#include "xllmkl.h"

using namespace xll;

#ifdef _DEBUG
static AddIn xai_function(
	Document(CATEGORY)
	.Documentation("Fastest and most used math library for Intel and compatible processors. ")
);
#endif // _DEBUG
