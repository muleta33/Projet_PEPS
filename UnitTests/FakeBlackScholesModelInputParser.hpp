#pragma once
#include "BlackScholesModelInputParser.hpp"

namespace UnitTests {
	class FakeBlackScholesModelInputParser : public input_parsers::BlackScholesModelInputParser
	{
	private:
		PnlVect * vol;
		PnlMat * correlation;

	public:
		FakeBlackScholesModelInputParser()
		{
			vol = pnl_vect_create(5);
			LET(vol, 0) = 0.1;
			LET(vol, 1) = 0.3;
			LET(vol, 2) = 0.15;
			LET(vol, 3) = 0.25;
			LET(vol, 4) = 0.2;
			correlation = pnl_mat_create_from_double(5, 5, 0.3);
			for (int i = 0; i < 5; ++i)
				MLET(correlation, i, i) = 1;
		}

		~FakeBlackScholesModelInputParser()
		{
			pnl_vect_free(&vol);
			vol = nullptr;
		}

		int get_underlying_number() const { return 5; }
		virtual int get_monitoring_times() const { return 8; }
		virtual PnlMat * get_correlation_matrix() const { return correlation; }
		virtual double get_interest_rate() const { return 0.05; }
		virtual double get_maturity() const { return 4; }
		virtual PnlVect * get_volatility() const { return vol; }

	};
}