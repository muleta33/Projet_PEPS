#pragma once
#include "BlackScholesModelInputParser.hpp"
#include "Parser.hpp"

namespace input_parsers
{
	class CoreBlackScholesModelInputParser : public BlackScholesModelInputParser
	{
	private:
		const Parser & parser;
		int underlying_number;
	public:
		int get_underlying_number() const;
		int get_monitoring_times() const;
		double get_correlation_parameter() const;
		double get_interest_rate() const;
		double get_final_simulation_date() const;
		PnlVect * get_volatility() const;

		CoreBlackScholesModelInputParser(const Parser & p) : parser(p)
		{
			parser.extract("option size", underlying_number);
		}

		virtual ~CoreBlackScholesModelInputParser() {};
	};
}