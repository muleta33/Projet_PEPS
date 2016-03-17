#pragma once
#include "pnl/pnl_vector.h"
#include "Product.hpp"
#include "UnderlyingModel.hpp"

class Pricing
{
protected:
	models::UnderlyingModel &underlying_model_;
	const products::Product &product_;
public:
	Pricing(models::UnderlyingModel & underlying_model, const products::Product & product) : underlying_model_(underlying_model), product_(product) {};
	virtual void price(PnlVect * spots, double &price, double &confidence_interval) const = 0;
	virtual void price_at(const double time, PnlMat * past, double &price, double &confidence_interval) const = 0;
};