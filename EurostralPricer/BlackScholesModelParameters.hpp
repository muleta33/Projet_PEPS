#pragma once
#include "BlackScholesModelInputParameters.hpp"
#include "InputParametersUtilities.hpp"

using namespace input_parameters;

class BlackScholesModelParameters : public input_parameters::BlackScholesModelInputParameters
{
private:
	int const underlying_number = 3;
	double const maturity = 8;
	int const monitoring_times = 16;
	double interest_rate = 0.0485;
	PnlVect * foreign_interest_rates = pnl_vect_create_from_scalar(3, 0.0485);
	PnlVect * volatilities;
	PnlMat * correlation_matrix;

public:
	double get_maturity() const { return maturity; }
	int get_underlying_number() const { return underlying_number; }
	int get_monitoring_times() const { return monitoring_times; }
	PnlMat * get_correlation_matrix() const { return correlation_matrix; }
	double get_interest_rate() const { return interest_rate; }
	PnlVect * get_volatility() const { return volatilities; }
	PnlVect * get_foreign_interest_rates() const { return foreign_interest_rates; }

	BlackScholesModelParameters(PnlMat * past_data)
	{
		PnlMat * past_returns = pnl_mat_create(past_data->m - 1, past_data->n);
		compute_returns(past_returns, past_data);
		volatilities = pnl_vect_create(2 * underlying_number);
		compute_volatilities(volatilities, past_returns);
		correlation_matrix = pnl_mat_create(2 * underlying_number, 2 * underlying_number);
		compute_correlation_matrix(correlation_matrix, past_returns, volatilities);
		pnl_mat_free(&past_returns);
	}

	~BlackScholesModelParameters() 
	{
		pnl_vect_free(&volatilities);
		pnl_mat_free(&correlation_matrix);
		pnl_vect_free(&foreign_interest_rates);
	};
};