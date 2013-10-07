// xllSS.h - Excel specific Summary Statistics routines
#pragma once
#include "vslSS.h"
#include "xllmkl.h"

// keep track of results in an FP
namespace xll {

	class SSTask : public vsl::SSTask<> {
		SSTask(const SSTask&);
		SSTask& operator=(const SSTask&);
		std::map<MKL_INT,FPX> map; // Map VSL_SS_ED_* Edit requests to results
	public:
		SSTask(MKL_INT nvar, MKL_INT nobs, const double* data, const double* weights = 0, const MKL_INT* indices = 0)
			: vsl::SSTask<>(nvar, nobs, data, weights, indices)
		{ }
		~SSTask()
		{
//			mkl_free_buffers(); // MKL likes to hang on to memory
		}

		// call as many times as needed
		MKL_INT Compute(MKL_INT param, MKL_INT method)
		{
			MKL_INT d = dimen();
			MKL_INT64 params(0);

			switch (param) {
			case VSL_SS_ED_KURTOSIS:
				ensure (VSL_STATUS_OK == add(VSL_SS_ED_4C_MOM, 1, d));
				ensure (VSL_STATUS_OK == add(VSL_SS_ED_2C_MOM, 1, d));
			case VSL_SS_ED_4R_MOM:
			case VSL_SS_ED_4C_MOM:
			{
				ensure (VSL_STATUS_OK == add(param, 1, d));
				ensure (VSL_STATUS_OK == add(VSL_SS_ED_4R_MOM, 1, d));
				ensure (VSL_STATUS_OK == add(VSL_SS_ED_3R_MOM, 1, d));
				ensure (VSL_STATUS_OK == add(VSL_SS_ED_2R_MOM, 1, d));
				ensure (VSL_STATUS_OK == add(VSL_SS_ED_MEAN, 1, d));

				params |= mask(param);

				break;
			}
			case VSL_SS_ED_SKEWNESS:
				ensure (VSL_STATUS_OK == add(VSL_SS_ED_3C_MOM, 1, d));
				ensure (VSL_STATUS_OK == add(VSL_SS_ED_2C_MOM, 1, d));
			case VSL_SS_ED_3R_MOM:
			case VSL_SS_ED_3C_MOM:
			{
				ensure (VSL_STATUS_OK == add(param, 1, d));
				ensure (VSL_STATUS_OK == add(VSL_SS_ED_3R_MOM, 1, d));
				ensure (VSL_STATUS_OK == add(VSL_SS_ED_2R_MOM, 1, d));
				ensure (VSL_STATUS_OK == add(VSL_SS_ED_MEAN, 1, d));

				params |= mask(param);

				break;
			}
			case VSL_SS_ED_VARIATION: 
				ensure (VSL_STATUS_OK == add(VSL_SS_ED_2C_MOM, 1, d));
			case VSL_SS_ED_2R_MOM:
			case VSL_SS_ED_2C_MOM:
			{
				ensure (VSL_STATUS_OK == add(param, 1, d));
				ensure (VSL_STATUS_OK == add(VSL_SS_ED_2R_MOM, 1, d));
				ensure (VSL_STATUS_OK == add(VSL_SS_ED_MEAN, 1, d));

				params |= mask(param);

				break;
			}
			case VSL_SS_ED_MIN:
			{
				ensure (VSL_STATUS_OK == add(param, 1, d, (std::numeric_limits<double>::max)()));

				params |= mask(param);

				break;
			}
			case VSL_SS_ED_MAX:
			{
				ensure (VSL_STATUS_OK == add(param, 1, d, -(std::numeric_limits<double>::max)()));

				params |= mask(param);

				break;
			}
			case VSL_SS_ED_MEAN:
			{
				ensure (VSL_STATUS_OK == add(param, 1, d));

				params |= mask(param);

				break;
			}
			case VSL_SS_ED_COV:
			case VSL_SS_ED_COR:
			{
				ensure (VSL_STATUS_OK == add(param, d, d));
				ensure (VSL_STATUS_OK == add(VSL_SS_ED_MEAN, 1, d));

				params |= mask(param);

				break;
			}
			default:
				XLL_WARNING("VSL.TaskEdit: unrecognized parameter");
			}

			ensure (vsl::status == VSL_STATUS_OK);

			if (method) // only call TaskEdit if 0
				vsl::status = vsl::SSTask<>::Compute(params, method);

			return vsl::status;
		}

		// use same param as in Compute call
		const traits<XLOPERX>::xfp* Result(MKL_INT param)
		{
			auto i = map.find(param);

			return i == map.end() ? 0 : i->second.get();
		}

	private:
		// add FP to map to hold results
		MKL_INT add(MKL_INT pi, MKL_INT r, MKL_INT c, double d = std::numeric_limits<double>::quiet_NaN()) 
		{
			if (map.find(pi) == map.end()) {
				map[pi] = FPX(static_cast<xword>(r), static_cast<xword>(c));
				if (!_isnan(d))
					map[pi] = d;
				vsl::status = Edit(pi, map[pi].array());
			}

			return vsl::status;
		}

		// map Edit to Compute tasks
		static MKL_INT64 mask(MKL_INT param) {
			static struct params_ { MKL_INT64 mask; } params[] = {
				{0}, // not used
				{0}, //#define VSL_SS_ED_DIMEN                                 1
				{0}, //#define VSL_SS_ED_OBSERV_N                              2
				{0}, //#define VSL_SS_ED_OBSERV                                3
				{0}, //#define VSL_SS_ED_OBSERV_STORAGE                        4
				{0}, //#define VSL_SS_ED_INDC                                  5
				{0}, //#define VSL_SS_ED_WEIGHTS                               6
				{VSL_SS_MEAN}, //#define VSL_SS_ED_MEAN                                  7
				{VSL_SS_2R_MOM}, //#define VSL_SS_ED_2R_MOM                                8
				{VSL_SS_3R_MOM}, //#define VSL_SS_ED_3R_MOM                                9
				{VSL_SS_4R_MOM}, //#define VSL_SS_ED_4R_MOM                               10
				{VSL_SS_2C_MOM}, //#define VSL_SS_ED_2C_MOM                               11
				{VSL_SS_3C_MOM}, //define VSL_SS_ED_3C_MOM                               12
				{VSL_SS_4C_MOM}, //#define VSL_SS_ED_4C_MOM                               13
				{VSL_SS_KURTOSIS}, //#define VSL_SS_ED_KURTOSIS                             14
				{VSL_SS_SKEWNESS}, //#define VSL_SS_ED_SKEWNESS                             15
				{VSL_SS_MIN}, //#define VSL_SS_ED_MIN                                  16
				{VSL_SS_MAX}, //#define VSL_SS_ED_MAX                                  17
				{VSL_SS_VARIATION}, //#define VSL_SS_ED_VARIATION                            18
				{VSL_SS_COV}, //#define VSL_SS_ED_COV                                  19
				{0}, //#define VSL_SS_ED_COV_STORAGE                          20
				{VSL_SS_MEAN|VSL_SS_COR}, //#define VSL_SS_ED_COR                                  21
				{0}, //#define VSL_SS_ED_COR_STORAGE                          22
				{0}, //#define VSL_SS_ED_ACCUM_WEIGHT                         23
				{0}, //#define VSL_SS_ED_QUANT_ORDER_N                        24
				{0}, //#define VSL_SS_ED_QUANT_ORDER                          25
				{0}, //#define VSL_SS_ED_QUANT_QUANTILES                      26
				{0}, //#define VSL_SS_ED_ORDER_STATS                          27
				{0}, //#define VSL_SS_ED_GROUP_INDC                           28
				{0}, //#define VSL_SS_ED_POOLED_COV_STORAGE                   29
				{0}, //#define VSL_SS_ED_POOLED_MEAN                          30
				{0}, //#define VSL_SS_ED_POOLED_COV                           31
				{0}, //#define VSL_SS_ED_GROUP_COV_INDC                       32
				{0}, //#define VSL_SS_ED_GROUP_MEAN                           33
				{0}, //#define VSL_SS_ED_GROUP_COV_STORAGE                    34
				{0}, //#define VSL_SS_ED_GROUP_COV                            35
				{0}, //#define VSL_SS_ED_ROBUST_COV_STORAGE                   36
				{0}, //#define VSL_SS_ED_ROBUST_COV_PARAMS_N                  37
				{0}, //#define VSL_SS_ED_ROBUST_COV_PARAMS                    38
				{0}, //#define VSL_SS_ED_ROBUST_MEAN                          39
				{0}, //#define VSL_SS_ED_ROBUST_COV                           40
				{0}, //#define VSL_SS_ED_OUTLIERS_PARAMS_N                    41
				{0}, //#define VSL_SS_ED_OUTLIERS_PARAMS                      42
				{0}, //#define VSL_SS_ED_OUTLIERS_WEIGHT                      43
				{0}, //#define VSL_SS_ED_ORDER_STATS_STORAGE                  44
				{0}, //#define VSL_SS_ED_PARTIAL_COV_IDX                      45
				{0}, //#define VSL_SS_ED_PARTIAL_COV                          46
				{0}, //#define VSL_SS_ED_PARTIAL_COV_STORAGE                  47
				{0}, //#define VSL_SS_ED_PARTIAL_COR                          48
				{0}, //#define VSL_SS_ED_PARTIAL_COR_STORAGE                  49
				{0}, //#define VSL_SS_ED_MI_PARAMS_N                          50
				{0}, //#define VSL_SS_ED_MI_PARAMS                            51
				{0}, //#define VSL_SS_ED_MI_INIT_ESTIMATES_N                  52
				{0}, //#define VSL_SS_ED_MI_INIT_ESTIMATES                    53
				{0}, //#define VSL_SS_ED_MI_SIMUL_VALS_N                      54
				{0}, //#define VSL_SS_ED_MI_SIMUL_VALS                        55
				{0}, //#define VSL_SS_ED_MI_ESTIMATES_N                       56
				{0}, //#define VSL_SS_ED_MI_ESTIMATES                         57
				{0}, //#define VSL_SS_ED_MI_PRIOR_N                           58
				{0}, //#define VSL_SS_ED_MI_PRIOR                             59
				{0}, //#define VSL_SS_ED_PARAMTR_COR                          60
				{0}, //#define VSL_SS_ED_PARAMTR_COR_STORAGE                  61
				{0}, //#define VSL_SS_ED_STREAM_QUANT_PARAMS_N                62
				{0}, //#define VSL_SS_ED_STREAM_QUANT_PARAMS                  63
				{0}, //#define VSL_SS_ED_STREAM_QUANT_ORDER_N                 64
				{0}, //#define VSL_SS_ED_STREAM_QUANT_ORDER                   65
				{0}, //#define VSL_SS_ED_STREAM_QUANT_QUANTILES               66
			};

			return params[param].mask;
		}
	};
}

