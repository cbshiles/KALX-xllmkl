// vslStream.cpp - random number stream implementation
#include "vslStream.h"
#include "xllmkl.h"

using namespace xll;

vsl::Stream vsl::defaultStream(VSL_BRNG_MT19937, 0);

XLL_ENUM_DOC(VSL_BRNG_MCG31, VSL_BRNG_MCG31, CATEGORY, _T("A 31-bit multiplicative congruential generator."), _T("<codeInline>MCG(1132489760, 2<superscript>31</superscript> -1)</codeInline> from L'Ecuyer, Pierre. <legacyItalic>Tables of Linear Congruential Generators of Different Sizes and Good Lattice Structure</legacyItalic>. Mathematics of Computation, 68, 225, 249-260, 1999."))
XLL_ENUM_DOC(VSL_BRNG_R250, VSL_BRNG_R250, CATEGORY, _T("A generalized feedback shift register generator."), _T(""))
XLL_ENUM_DOC(VSL_BRNG_MRG32K3A, VSL_BRNG_MRG32K3A, CATEGORY, _T("A combined multiple recursive generator with two components of order 3."), _T(""))
XLL_ENUM_DOC(VSL_BRNG_MCG59, VSL_BRNG_MCG59, CATEGORY, _T("A 59-bit multiplicative congruential generator."), _T(""))
XLL_ENUM_DOC(VSL_BRNG_WH, VSL_BRNG_WH, CATEGORY, _T("A set of 273 Wichmann-Hill combined multiplicative congruential generators."), _T(""))
XLL_ENUM_DOC(VSL_BRNG_MT19937, VSL_BRNG_MT19937, CATEGORY, _T("A Mersenne Twister pseudorandom number generator."), _T(""))
XLL_ENUM_DOC(VSL_BRNG_MT2203, VSL_BRNG_MT2203, CATEGORY, _T("A set of 6024 Mersenne Twister pseudorandom number generators."), _T(""))
XLL_ENUM_DOC(VSL_BRNG_SFMT19937, VSL_BRNG_SFMT19937, CATEGORY, _T("A SIMD-oriented Fast Mersenne Twister pseudorandom number generator."), _T(""))
XLL_ENUM_DOC(VSL_BRNG_SOBOL, VSL_BRNG_SOBOL, CATEGORY, _T("A 32-bit Gray code-based generator producing low-discrepancy sequences for dimensions 1 through 40; user-defined dimensions are also available."), _T(""))
XLL_ENUM_DOC(VSL_BRNG_NIEDERR, VSL_BRNG_NIEDERR, CATEGORY, _T("A 32-bit Gray code-based generator producing low-discrepancy sequences for dimensions 1 through 318; user-defined dimensions are also available."), _T(""))
XLL_ENUM_DOC(VSL_BRNG_IABSTRACT, VSL_BRNG_IABSTRACT, CATEGORY, _T("An abstract random number generator for integer arrays."), _T(""))
XLL_ENUM_DOC(VSL_BRNG_DABSTRACT, VSL_BRNG_DABSTRACT, CATEGORY, _T("An abstract random number generator for double precision floating-point arrays."), _T(""))
XLL_ENUM_DOC(VSL_BRNG_SABSTRACT, VSL_BRNG_SABSTRACT, CATEGORY, _T("An abstract random number generator for single precision floating-point arrays."), _T(""))
XLL_ENUM_DOC(VSL_BRNG_NONDETERM, VSL_BRNG_NONDETERM, CATEGORY, _T("A non-deterministic random number generator."), _T(""))

static AddInX xai_vslStream(
	FunctionX(XLL_HANDLEX, _T("?xll_vslStream"), _T("VSL.Stream"))
	.Arg(XLL_LPOPERX, _T("Brng"), _T("is the type of random number generator from the VSL_BRNG_* enumeration."))
	.Arg(XLL_LONGX, _T("Seed"), _T("is the initial random number generator seed. "))
	.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp(_T("Returns a handle to a random stream."))
	.Documentation(_T(""))
);
HANDLEX WINAPI xll_vslStream(LPOPERX pBrng, LONG seed)
{
#pragma XLLEXPORT
	handlex h;

	try {
		int brng = Enum_(*pBrng);
		handle<vsl::Stream> h_(new vsl::Stream(brng, seed));
		ensure (h_);

		h = h_.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return h;
}

static AddInX xai_vslStreamStateBrng(
	FunctionX(XLL_LONGX, _T("?xll_StreamStateBrng"), _T("VSL.StreamStateBrng"))
	.Arg(XLL_HANDLEX, _T("Stream"), _T("is a handle to a stream returned by VSL.Stream. "))
	.Category(CATEGORY)
	.FunctionHelp(_T("Returns index of a basic generator used for generation of a given random stream."))
	.Documentation(_T(""))
);
LONG WINAPI xll_StreamStateBrng(HANDLEX h)
{
#pragma XLLEXPORT
	LONG brng(0);

	try {
		handle<vsl::Stream> h_(h);
		if (!h_)
			XLL_INFO("StreamStateBrng: returning default stream brng");

		brng = vslGetStreamStateBrng(h_ ? *h_ : vsl::defaultStream);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}

	return brng;
}

static AddInX xai_vdRngGaussian(
	FunctionX(XLL_FPX, _T("?xll_vdRngGaussian"), _T("VD.RngGaussian"))
	.Arg(XLL_HANDLEX, _T("Stream"), _T("is a handle to a stream."))
	.Arg(XLL_WORDX, _T("Rows"), _T("is the number of rows to generate."))
	.Arg(XLL_WORDX, _T("_Columns"), _T("is the optional number of columns to generate. Default is 1."))
	.Arg(XLL_DOUBLEX, _T("_Mean"), _T("is the optional mean. Defalult is 0."))
	.Arg(XLL_DOUBLEX, __T("_Stdev"), _T("is the optional standard deviation. Default is 1. "))
	.Volatile()
	.Category(CATEGORY)
	.FunctionHelp(_T("Return an array of normally distributed random variates."))
	.Documentation(_T(""))
);
xfp* xll_vdRngGaussian(HANDLEX s, WORD r, WORD c, double mu, double sigma)
{
#pragma XLLEXPORT
	static FPX result;

	try {
		if (r == 0)
			r = 1;
		if (c == 0)
			c = 1;
		if (sigma == 0)
			sigma = 1;

		handle<vsl::Stream> s_(s);
		if (!s_)
			XLL_INFO("RngGaussian: using defalult stream");

		result.resize(r, c);
		vdRngGaussian(VSL_RNG_METHOD_GAUSSIAN_BOXMULLER2, s_ ? *s_ : vsl::defaultStream, result.size(), result.array(), mu, sigma);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());

		return 0;
	}

	return result.get();
}