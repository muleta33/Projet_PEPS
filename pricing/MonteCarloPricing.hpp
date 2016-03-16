#pragma once
#include "Pricing.hpp"

class MonteCarloPricing : public Pricing
{
private:
	const unsigned long sample_number;

public:
	MonteCarloPricing(models::UnderlyingModel & underlying_model, const products::Product & product, const unsigned long sample_nb) : 
		Pricing(underlying_model, product), sample_number(sample_nb) {};
	void price(PnlVect * spots, double &price, double &confidence_interval) const;
	void price_at(const double time, PnlMat * past, double &price, double &confidence_interval) const;
};