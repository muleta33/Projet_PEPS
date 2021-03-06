#pragma once
#include "BlackScholesModelInputParameters.hpp"

namespace UnitTests {
	class FakeBlackScholesModelInputParametersForCallBis : public input_parameters::BlackScholesModelInputParameters
	{
	private:
		PnlVect * vol;
		PnlMat * correlation;

	public:
		FakeBlackScholesModelInputParametersForCallBis()
		{
			vol = pnl_vect_create(2);
			LET(vol, 0) = 0.15;
			LET(vol, 1) = 0;
			correlation = pnl_mat_create_from_scalar(2, 2,0);
			MLET(correlation, 0, 0) = 1;
			MLET(correlation,1, 1) = 1;
		}

		~FakeBlackScholesModelInputParametersForCallBis()
		{
			pnl_vect_free(&vol);
			vol = nullptr;
			pnl_mat_free(&correlation);
			correlation = nullptr;
		}

		int get_underlying_number() const { return 1; }
		virtual int get_monitoring_times() const { return 6; }
		virtual PnlMat * get_correlation_matrix() const { return correlation; }
		virtual double get_interest_rate() const { return 0.05; }
		virtual double get_maturity() const { return 3; }
		virtual PnlVect * get_volatility() const { return vol; }
		virtual PnlVect * get_foreign_interest_rates() const { return pnl_vect_create_from_double(1, 0.05); }

	};
}