#pragma once
#include "BlackScholesModelInputParser.hpp"

namespace UnitTests {
	class FakeBlackScholesModelInputParserForCall : public input_parsers::BlackScholesModelInputParser
	{
	private:
		PnlVect * vol;

	public:
		FakeBlackScholesModelInputParserForCall()
		{
			vol = pnl_vect_create(1);
			LET(vol, 0) = 0.15;
		}

		~FakeBlackScholesModelInputParserForCall()
		{
			pnl_vect_free(&vol);
			vol = nullptr;
		}

		int get_underlying_number() const { return 1; }
		virtual int get_monitoring_times() const { return 8; }
		virtual double get_correlation_parameter() const { return 0; }
		virtual double get_interest_rate() const { return 0.05; }
		virtual double get_maturity() const { return 4; }
		virtual PnlVect * get_volatility() const { return vol; }

	};
}