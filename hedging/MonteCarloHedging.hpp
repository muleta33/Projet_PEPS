#pragma once
#include "pnl/pnl_vector.h"
#include "Product.hpp"
#include "UnderlyingModel.hpp"

class MonteCarloHedging
{
private:
	const unsigned long sample_number;
	const double shift;

public:
	MonteCarloHedging(const unsigned long sample_nb, const double shift) : sample_number(sample_nb), shift(shift) {};
	void hedge(const models::UnderlyingModel & underlying_model, const products::Product & product, const PnlVect * const spots, PnlVect * hedging_results) const;
	void hedge_at(const double time, const models::UnderlyingModel & underlying_model, const products::Product & product, const PnlMat * const past, PnlVect * hedging_results) const;
};