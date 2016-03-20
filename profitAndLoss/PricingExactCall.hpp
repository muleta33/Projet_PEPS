#pragma once
#include "Pricing.hpp"
#include "pnl\pnl_finance.h"

class PricingExactCall : public Pricing
{
public:
	PricingExactCall(models::UnderlyingModel & underlying_model, const products::Product & product) :
		Pricing(underlying_model, product) {};
	void price(PnlVect * spots, double &price, double &confidence_interval) const;
	void price_at(const double time, PnlMat * past, double &price, double &confidence_interval) const;
};

void PricingExactCall::price(PnlVect * spots, double &price, double &confidence_interval) const
{
	double p = 0;
	double delta = 0;
	pnl_cf_call_bs(GET(spots, 0), 100, 1, 0.0485, 0, 0.2, &p, &delta);
	price = p;
	confidence_interval = 0;
}

void PricingExactCall::price_at(const double time, PnlMat * past, double &price, double &confidence_interval) const
{
	double p = 0;
	double delta = 0;
	pnl_cf_call_bs(MGET(past, past->m - 1, 0), 100, 1 - time, 0.0485, 0, 0.2, &p, &delta);
	price = p;
	confidence_interval = 0;
}