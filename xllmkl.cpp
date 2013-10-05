// xllmkl.cpp - MKL wrappers
#include <cmath>
#include "xllmkl.h"

using namespace xll;

MKL_INT vsl::status;

static AddIn xai_function(
	Document(CATEGORY)
	.Documentation("<para>"
		"Fastest and most used math library for Intel and compatible processors. "
		"</para><para>"
		"This library remains as faithful as possible to the MKL interface. "
		"Current users will recognize almost all of the functions in this library, "
		"however some accomodation needs to be made for the fact Excel is purely functional. "
		"</para><para>"
		"This implementation uses the little know FP(link) data type for two dimensional arrays "
		"of floating point numbers. The pointer to the array can be handed directly to MKL. "
		"No need to copy arrays, which makes a significant performance difference for large arrays. "
		"</para>"
	)
);

static Auto<Close> xac_free_buffers([](void) { mkl_free_buffers(); return 1; });
