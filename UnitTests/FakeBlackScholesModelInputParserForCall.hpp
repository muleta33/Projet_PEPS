#pragma once
#include "BlackScholesModelInputParser.hpp"

namespace UnitTests {
	class FakeBlackScholesModelInputParserForCall : public input_parsers::BlackScholesModelInputParser
	{
	private:
		PnlVect * vol;
		PnlMat * correlation;

	public:
		FakeBlackScholesModelInputParserForCall()
		{
			vol = pnl_vect_create(2);
			LET(vol, 0) = 0.15;
			LET(vol, 1) = 0;
			correlation = pnl_mat_create(2, 2);
			MLET(correlation, 0, 0) = 1;
			MLET(correlation, 1, 1) = 1;
			MLET(correlation, 0, 1) = 0;
			MLET(correlation, 1, 0) = 0;
		}

		~FakeBlackScholesModelInputParserForCall()
		{
			pnl_vect_free(&vol);
			vol = nullptr;
			pnl_mat_free(&correlation);
			correlation = nullptr;
		}

		int get_underlying_number() const { return 1; }
		virtual int get_monitoring_times() const { return 8; }
		virtual PnlMat * get_correlation_matrix() const { return correlation; }
		virtual double get_interest_rate() const { return 0.05; }
		virtual double get_maturity() const { return 4; }
		virtual PnlVect * get_volatility() const { return vol; }

	};
}