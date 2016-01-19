#pragma once
#include "BlackScholesModelInputParser.hpp"

class BlackScholesModelParameters : public input_parsers::BlackScholesModelInputParser
{
private:
	int const underlying_number = 3;
	double const maturity = 8;
	int const monitoring_times = 16;
	double correlation_parameter;
	double interest_rate;
	PnlVect * volatilities;

public:
	double get_maturity() const { return maturity; }
	int get_underlying_number() const { return underlying_number; }
	int get_monitoring_times() const { return monitoring_times; }
	double get_correlation_parameter() const { return correlation_parameter; }
	double get_interest_rate() const { return interest_rate; }
	PnlVect * get_volatility() const { return volatilities; }

	BlackScholesModelParameters(PnlVect *vol, double correlation)
	{
		volatilities = pnl_vect_create(underlying_number);
		pnl_vect_clone(volatilities, vol);
		correlation_parameter = correlation;
	}

	virtual ~BlackScholesModelParameters() {};
};