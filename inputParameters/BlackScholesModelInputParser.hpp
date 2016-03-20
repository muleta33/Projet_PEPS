#pragma once
#include "BlackScholesModelInputParameters.hpp"
#include "Parser.hpp"

namespace input_parameters
{
	class BlackScholesModelInputParser : public BlackScholesModelInputParameters
	{
	private:
		const Parser & parser;
		int underlying_number;
	public:
		int get_underlying_number() const;
		int get_monitoring_times() const;
		PnlMat * get_correlation_matrix() const;
		double get_interest_rate() const;
		double get_maturity() const;
		PnlVect * get_volatility() const;
		PnlVect * get_foreign_interest_rates() const;

		BlackScholesModelInputParser(const Parser & p) : parser(p)
		{
			parser.extract("option size", underlying_number);
		}

		virtual ~BlackScholesModelInputParser() {};
	};
}