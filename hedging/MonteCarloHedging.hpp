#pragma once
#include "Hedging.hpp"

class MonteCarloHedging : public Hedging
{
private:
	const unsigned long sample_number;
	const double shift;

public:
	MonteCarloHedging(models::UnderlyingModel & underlying_model, const products::Product & product, const unsigned long sample_nb, const double shift) : 
		Hedging(underlying_model, product), sample_number(sample_nb), shift(shift) {};
	void hedge(const PnlVect * const spots, PnlVect * hedging_results) const;
	void hedge_at(const double time, const PnlMat * const past, PnlVect * hedging_results) const;
};