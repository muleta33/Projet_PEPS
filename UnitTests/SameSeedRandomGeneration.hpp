#pragma once
#include "RandomGeneration.hpp"
#include "pnl\pnl_random.h"

namespace UnitTests {
	class SameSeedRandomGeneration : public generators::RandomGeneration
	{

	private:
		PnlRng *pnlRandomGen;

	public:
		SameSeedRandomGeneration()
		{
			pnlRandomGen = pnl_rng_create(PNL_RNG_MERSENNE);
			pnl_rng_sseed(pnlRandomGen, 1234);
		}

		~SameSeedRandomGeneration()
		{
			pnl_rng_free(&pnlRandomGen);
		}

		void get_one_gaussian_sample(PnlVect * const into) const
		{
			pnl_vect_rng_normal(into, into->size, pnlRandomGen);
		}
	};
}