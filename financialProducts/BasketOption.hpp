#pragma once
#include "Product.hpp"
#include "BasketOptionInputParameters.hpp"

class BasketOption : public products::Product
{
private:
	double strike_;
	PnlVect * lambda_;

public:
	BasketOption() : Product(), strike_(0) {};
	BasketOption(const input_parameters::BasketOptionInputParameters &input_parameters) : Product(input_parameters.get_maturity(), 
		input_parameters.get_underlying_number(), input_parameters.get_currencies())
	{
		strike_ = input_parameters.get_strike();
		lambda_ = input_parameters.get_underlying_coefficients();
	}
	BasketOption(double maturity, const input_parameters::BasketOptionInputParameters &input_parameters) : 
		Product(maturity, input_parameters.get_underlying_number(), input_parameters.get_currencies())
	{
		strike_ = input_parameters.get_strike();
		lambda_ = input_parameters.get_underlying_coefficients();
	}
	~BasketOption() { pnl_vect_free(&lambda_); };

	double get_payoff(const PnlMat * const underlying_paths) const
	{
		double sum = 0.0;
		PnlVect final = pnl_vect_wrap_mat_row(underlying_paths, underlying_paths->m - 1);
		sum = pnl_vect_scalar_prod(lambda_, &final);
		return fmax(sum - strike_, 0.0);
	}

};