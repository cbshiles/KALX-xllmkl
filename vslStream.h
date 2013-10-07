// vslStream.h - stream of random numbers
#pragma once
#include <stdexcept>
#include <mkl_vsl.h>

namespace vsl {

	extern int status; // global last status

	class Stream {
		VSLStreamStatePtr p_;
	public:
		Stream(int brng, unsigned int seed)
		{
			status = vslNewStream(&p_, brng, seed);
			
			if (status != VSL_STATUS_OK)
				throw std::runtime_error("vslNewStream: failed");
		}
		Stream(int brng, int n, const unsigned int* seed)
		{
			status = vslNewStreamEx(&p_, brng, n, seed);
			
			if (status != VSL_STATUS_OK)
				throw std::runtime_error("vslNewStream: failed");
		}
		Stream(const Stream& s)
		{
			status = vslCopyStream(&p_, s.p_);
			
			if (status != VSL_STATUS_OK)
				throw std::runtime_error("vslCopyStream: failed");
		}
		Stream(Stream&& s)
		{
			p_ = s.p_;

			s.p_ = 0;
		}

		Stream& operator=(const Stream& s)
		{
			if (this != &s) {
				status = vslCopyStream(&p_, s.p_);
			
				if (status != VSL_STATUS_OK)
					throw std::runtime_error("vslCopyStream: failed");
			}

			return *this;
		}
		Stream& operator=(Stream&& s)
		{
			if (this != &s) {
				p_ = s.p_;
				s.p_ = 0;
			}

			return *this;
		}
		~Stream()
		{
			status = vslDeleteStream(&p_);
		}

		operator VSLStreamStatePtr()
		{
			return p_;
		}
	};

	extern Stream defaultStream; // common default random number stream

} // namespace vsl