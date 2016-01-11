#pragma once
#include "Hedging.hpp"
#include "pnl\pnl_finance.h"

class HedgingExactCall : public Hedging
{
public:
	HedgingExactCall(const models::UnderlyingModel & underlying_model, const products::Product & product) :
		Hedging(underlying_model, product) {};
	void hedge(const PnlVect * const spots, PnlVect * hedging_results) const;
	void hedge_at(const double time, const PnlMat * const past, PnlVect * hedging_results) const;
};

void HedgingExactCall::hedge(const PnlVect * const spots, PnlVect * hedging_results) const
{
	double p = 0;
	double delta = 0;
	pnl_cf_call_bs(GET(spots, 0), 100, 1, 0.485, 0, 0.2, &p, &delta);
	LET(hedging_results, 0) = delta;
}

void HedgingExactCall::hedge_at(const double time, const PnlMat * const past, PnlVect * hedging_results) const
{
	double p = 0;
	double delta = 0;
	pnl_cf_call_bs(MGET(past, 1, 0), 100, 1 - time, 0.485, 0, 0.2, &p, &delta);
	LET(hedging_results, 0) = delta;
}