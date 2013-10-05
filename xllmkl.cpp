// xllmkl.cpp - MKL wrappers
// Copyright (c) KALX, LLC. All rights reserved. No warranty is made.
#include <cmath>
#include "xllmkl.h"

using namespace xll;

MKL_INT vsl::status;

static AddIn xai_function(
	Document("MKL")
	.Documentation("<para>"
		"Fastest and most used math library for Intel&#174; and compatible processors. "
		"</para><para>"
		"This library remains as faithful as possible to the MKL interface. "
		"Current users will recognize almost all of the functions in this library, "
		"however some accomodation needs to be made for the fact Excel is purely functional. "
		"</para><para>"
		"This implementation uses the little known Excel "
		"<externalLink>"
		"<linkText>FP</linkText>"
		"<linkUri>http://xll.codeplex.com/wikipage?title=FP</linkUri>"
		"</externalLink>"
		" data type for two dimensional arrays "
		"of floating point numbers. The pointer to the array can be handed directly to the MKL "
		"to get the highest possible performance. "
		"</para>"
	)
);

// still leaks
//static Auto<Close> xac_free_buffers([](void) { mkl_free_buffers(); return 1; });
