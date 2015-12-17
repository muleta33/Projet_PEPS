#pragma once
#include "pnl/pnl_matrix.h"
#include "pnl/pnl_vector.h"
#include "UnderlyingModel.hpp"
#include "Product.hpp"

class MonteCarloRoutine
{
protected:
	virtual const PnlMat * const get_generated_path() const = 0;
	const models::UnderlyingModel & underlying_model;
	const products::Product & product;
	const unsigned long sample_number;
	MonteCarloRoutine(const models::UnderlyingModel & underlying_model, const products::Product & product, const unsigned long sample_number) :
		underlying_model(underlying_model), product(product), sample_number(sample_number)
	{}
public:
	void price(double &price, double &confidence_interval) const;
};

///////////////////////////

class MonteCarloRoutineAtOrigin : public MonteCarloRoutine
{
private:
	const PnlVect * const spots;
protected:
	const PnlMat * const get_generated_path() const
	{
		return underlying_model.simulate_asset_paths_from_start(spots);
	}
public:
	MonteCarloRoutineAtOrigin(const models::UnderlyingModel &underlying_model, const products::Product & product, const unsigned long sample_nb, const PnlVect * const spots) :
		MonteCarloRoutine(underlying_model, product, sample_nb), spots(spots)
	{}

};


////////////////////////////

class MonteCarloRoutineAtTimeT : public MonteCarloRoutine
{
private:
	const PnlMat * const past;
	const double t;
protected:
	const PnlMat * const get_generated_path() const
	{
		return underlying_model.simulate_asset_paths_from_time(t, past);
	}
public:
	MonteCarloRoutineAtTimeT(const models::UnderlyingModel &underlying_model, const products::Product & product, const unsigned long sample_nb, const PnlMat * const past, const double t) :
		MonteCarloRoutine(underlying_model, product, sample_nb), past(past), t(t)
	{}
};