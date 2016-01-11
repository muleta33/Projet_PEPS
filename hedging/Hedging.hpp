#pragma once
#include "pnl/pnl_vector.h"
#include "Product.hpp"
#include "UnderlyingModel.hpp"

class Hedging
{
protected:
	const models::UnderlyingModel &underlying_model_;
	const products::Product &product_;
public:
	Hedging(const models::UnderlyingModel & underlying_model, const products::Product & product) : underlying_model_(underlying_model), product_(product) {};
	virtual void hedge(const PnlVect * const spots, PnlVect * hedging_results) const = 0;
	virtual void hedge_at(const double time, const PnlMat * const past, PnlVect * hedging_results) const = 0;
};