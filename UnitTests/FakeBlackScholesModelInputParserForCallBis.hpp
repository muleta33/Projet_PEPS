#pragma once
#include "BlackScholesModelInputParser.hpp"

namespace UnitTests {
	class FakeBlackScholesModelInputParserForCallBis : public input_parsers::BlackScholesModelInputParser
	{
	private:
		PnlVect * vol;

	public:
		FakeBlackScholesModelInputParserForCallBis()
		{
			vol = pnl_vect_create(1);
			LET(vol, 0) = 0.15;
		}

		~FakeBlackScholesModelInputParserForCallBis()
		{
			pnl_vect_free(&vol);
			vol = nullptr;
		}

		int get_underlying_number() const { return 1; }
		virtual int get_monitoring_times() const { return 6; }
		virtual double get_correlation_parameter() const { return 0; }
		virtual double get_interest_rate() const { return 0.05; }
		virtual double get_final_simulation_date() const { return 3; }
		virtual PnlVect * get_volatility() const { return vol; }

	};
}