#pragma once
#include "BlackScholesModelInputParser.hpp"

class BlackScholesModelParameters : public input_parsers::BlackScholesModelInputParser
{
private:
	int const underlying_number = 3;
	double const maturity = 8;
	int const monitoring_times = 16;
	double interest_rate = 0.0485;
	PnlVect * volatilities;
	PnlMat * correlation_matrix;

public:
	double get_maturity() const { return maturity; }
	int get_underlying_number() const { return underlying_number; }
	int get_monitoring_times() const { return monitoring_times; }
	PnlMat * get_correlation_matrix() const { return correlation_matrix; }
	double get_interest_rate() const { return interest_rate; }
	PnlVect * get_volatility() const { return volatilities; }

	BlackScholesModelParameters(PnlVect * vol, PnlMat * correlation)
	{
		volatilities = pnl_vect_create(underlying_number);
		pnl_vect_clone(volatilities, vol);
		correlation_matrix = pnl_mat_create(underlying_number, underlying_number);
		pnl_mat_clone(correlation_matrix, correlation);
	}

	virtual ~BlackScholesModelParameters() {};
};