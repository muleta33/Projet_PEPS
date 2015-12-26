#include "Portfolio.hpp"


void Portfolio::initialisation(double optionPrice, const PnlVect *deltas, const PnlVect *prices)
{
	if (deltas_->size != deltas->size)
		pnl_vect_resize(deltas_, deltas->size);
	pnl_vect_clone(deltas_, deltas);
	risk_free_investment_ = optionPrice - pnl_vect_scalar_prod(deltas, prices);
}

void Portfolio::rebalancing(const PnlVect *deltas, const PnlVect *prices, double interestRate, double timeStep)
{
	pnl_vect_minus_vect(deltas_, deltas);
	risk_free_investment_ = risk_free_investment_ * exp(interestRate * timeStep) + pnl_vect_scalar_prod(deltas_, prices);
	pnl_vect_clone(deltas_, deltas);
}

double Portfolio::compute_value(const PnlVect *prices)
{
	return risk_free_investment_ + pnl_vect_scalar_prod(deltas_, prices);
}

double Portfolio::compute_final_value(const PnlVect *prices, double interestRate, double timeStep)
{
	return risk_free_investment_ * exp(interestRate * timeStep) + pnl_vect_scalar_prod(deltas_, prices);
}
