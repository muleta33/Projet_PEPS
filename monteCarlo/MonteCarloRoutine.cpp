#include <cmath>
#include "MonteCarloRoutine.hpp"

void MonteCarloRoutine::price(double &price, double &confidence_interval) const
{
	double runningSum = 0;
	double runningSquaredSum = 0;
	double payoff;
	for (unsigned long i = 0; i < sample_number; i++)
	{
		const PnlMat * const generated_path = get_generated_path();

		payoff = product.get_payoff(generated_path);
		runningSum += payoff;
		runningSquaredSum += payoff * payoff;
	}
	double interest_rate = underlying_model.interest_rate();
	double maturity = product.get_maturity();
	// Pb pour pricing en t : Mnacho
	//price = exp(-1 * interest_rate * maturity) * runningSum / sample_number;
	price = exp(-1 * interest_rate * (maturity - get_time())) * runningSum / sample_number;
	double variance = exp(-2 * interest_rate * (maturity - get_time())) * runningSquaredSum / sample_number - price * price;
	confidence_interval = 1.96 * sqrt(variance / sample_number);
}

void MonteCarloRoutine::delta_hedge(const double shift, PnlVect * deltas) const
{
	PnlVect * payoff_differences_sum = pnl_vect_create_from_zero(product.get_underlying_number());
	const PnlMat * generated_path = NULL;
	PnlMat * shifted_asset_path = pnl_mat_new();

	for (unsigned long i = 0; i < sample_number; ++i)
	{
		generated_path = get_generated_path();

		for (int underlying = 0; underlying < product.get_underlying_number(); ++underlying)
		{
			underlying_model.get_shifted_asset_paths(generated_path, underlying, -shift, get_time(), get_past_values_number(), shifted_asset_path); 
			double payoff_down = product.get_payoff(shifted_asset_path); 
			
			underlying_model.get_shifted_asset_paths(generated_path, underlying, shift, get_time(), get_past_values_number(), shifted_asset_path);
			double payoff_up = product.get_payoff(shifted_asset_path);

			LET(payoff_differences_sum, underlying) = GET(payoff_differences_sum, underlying) + payoff_up - payoff_down;
		}
	}
	double interest_rate = underlying_model.interest_rate();
	double maturity = product.get_maturity();

	double actualisation = exp(-interest_rate * (maturity - get_time()));

	PnlVect * spots = pnl_vect_create_from_zero(generated_path->n);
	pnl_mat_get_row(spots, generated_path, 0);
	for (int underlying = 0; underlying < product.get_underlying_number(); ++underlying)
	{
		double factor = 1 / (sample_number * 2 * shift * GET(spots, underlying));
		LET(deltas, underlying) = actualisation * factor * GET(payoff_differences_sum, underlying);
	}

	pnl_vect_free(&spots);
	pnl_mat_free(&shifted_asset_path);
	pnl_vect_free(&payoff_differences_sum);
}

