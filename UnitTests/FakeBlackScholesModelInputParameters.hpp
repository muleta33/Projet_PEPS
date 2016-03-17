#pragma once
#include "BlackScholesModelInputParameters.hpp"

namespace UnitTests {
	class FakeBlackScholesModelInputParameters : public input_parameters::BlackScholesModelInputParameters
	{
	private:
		PnlVect * vol;
		PnlMat * correlation;

	public:
		FakeBlackScholesModelInputParameters()
		{
			vol = pnl_vect_create(6);
			LET(vol, 0) = 0.1;
			LET(vol, 1) = 0.3;
			LET(vol, 2) = 0.15;
			LET(vol, 3) = 0;
			LET(vol, 4) = 0;
			LET(vol, 5) = 0;
			correlation = pnl_mat_create_from_double(6, 6, 0);
			for (int i = 0; i < 6; ++i)
				MLET(correlation, i, i) = 1;
			MLET(correlation, 0, 1) = 0.3;
			MLET(correlation, 0, 2) = 0.3;
			MLET(correlation, 1, 0) = 0.3;
			MLET(correlation, 1, 2) = 0.3;
			MLET(correlation, 2, 0) = 0.3;
			MLET(correlation, 2, 1) = 0.3;
		}

		~FakeBlackScholesModelInputParameters()
		{
			pnl_vect_free(&vol);
			vol = nullptr;
		}

		int get_underlying_number() const { return 3; }
		virtual int get_monitoring_times() const { return 8; }
		virtual PnlMat * get_correlation_matrix() const { return correlation; }
		virtual double get_interest_rate() const { return 0.05; }
		virtual double get_maturity() const { return 4; }
		virtual PnlVect * get_volatility() const { return vol; }
		virtual PnlVect * get_foreign_interest_rates() const { return pnl_vect_create_from_double(3, 0.05); }
	};
}