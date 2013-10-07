// vslSS.h - Vector Statistical Library Summary Statistics.
// Copyright (c) KALX, LLC. All rights reserved. No warranty is made.
#pragma once
#include <vector>
#include <mkl_vsl.h>

namespace vsl {
/*
	template<class T>
	struct traits {
	};

	template<>
	struct traits<double> {
		static int (*vslSSNewTask)(VSLSSTaskPtr* , MKL_INT* , MKL_INT* , MKL_INT* , double [], double [], MKL_INT []) = vsldSSNewTask;
	};
*/
	extern MKL_INT status; // global last status

	template<class T = double, MKL_INT storage = VSL_SS_MATRIX_STORAGE_ROWS>
	class SSTask {
		SSTask(const SSTask&);
		SSTask operator=(const SSTask&);

		MKL_INT nvar_, nobs_, storage_;
//		std::vector<T> weights_;
//		std::vector<MKL_INT> indices_;
		VSLSSTaskPtr p_;
	public:
		SSTask(MKL_INT nvar, MKL_INT nobs, const T* data, const T* weights = 0, const MKL_INT* indices = 0)
			: nvar_(nvar), nobs_(nobs), storage_(storage)//, weights_(weights, weights + nvar), indices_(indices, indices + nvar)
		{
			status = vsldSSNewTask(&p_, &nvar_, &nobs_, &storage_, const_cast<double*>(data), const_cast<double*>(weights), const_cast<MKL_INT*>(indices));
//			status = traits<T>::vslSSNewTask(&p_, &nvar, &nobs, &storage_, const_cast<double*>(data), const_cast<double*>(weights), const_cast<MKL_INT*>(indices));

			if (status != VSL_STATUS_OK)
				throw std::runtime_error("vsldSSNewTask: failed");
		}
/*		SSTask(SSTask&& t)
		{
			p_ = t.p_;
			t.p_ = 0;
		}
		SSTask& operator=(SSTask&& t)
		{
			if (this != &t) {
				p_ = t.p_;
				t.p_ = 0;
			}

			return *this;
		}
*/		virtual ~SSTask()
		{
			if (p_)
				vslSSDeleteTask(&p_);
		}

		operator VSLSSTaskPtr()
		{
			return p_;
		}

		MKL_INT dimen() const
		{
			return nvar_;
		}

		// Modifies address of an input/output parameter in the task descriptor.
		int Edit(MKL_INT param, const MKL_INT* data)
		{
			return status = vsliSSEditTask(p_, param, data);
		}
		int Edit(MKL_INT param, const double* data)
		{
			return status = vsldSSEditTask(p_, param, data);
		}
		// Computes Summary Statistics estimates.
		int Compute(MKL_INT64 params, const MKL_INT method)
		{
			MKL_INT status(VSL_STATUS_OK);

			status = vsldSSCompute(p_, params, method);

			return status;
		}
	};

} // namespace vsl