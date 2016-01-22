#pragma once
#include "pnl/pnl_matrix.h"
#include "pnl/pnl_vector.h"
#include "UnderlyingModel.hpp"
#include "Product.hpp"

class MonteCarloRoutine
{
protected:
	virtual const PnlMat * const get_generated_path() const = 0;
	virtual double get_time() const = 0;
	virtual int get_past_values_number() const = 0;
	virtual void get_current_stock_values(PnlVect * current_stock_values) const = 0;

	const models::UnderlyingModel & underlying_model;
	const products::Product & product;
	const unsigned long sample_number;
	
	MonteCarloRoutine(const models::UnderlyingModel & underlying_model, const products::Product & product, const unsigned long sample_number) :
		underlying_model(underlying_model), product(product), sample_number(sample_number)
	{}
public:
	void price(double &price, double &confidence_interval) const;
	void delta_hedge(const double shift, PnlVect * deltas) const;
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

	double get_time() const { return 0.; }
	int get_past_values_number() const { return 1; }

	void get_current_stock_values(PnlVect * current_stock_values) const
	{
		pnl_vect_clone(current_stock_values, spots);
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

	double get_time() const { return t; }
	int get_past_values_number() const { return past->m; }
	
	void get_current_stock_values(PnlVect * current_stock_values) const
	{
		pnl_mat_get_row(current_stock_values, past, past->m - 1);
	}

public:
	MonteCarloRoutineAtTimeT(const models::UnderlyingModel &underlying_model, const products::Product & product, const unsigned long sample_nb, const PnlMat * const past, const double t) :
		MonteCarloRoutine(underlying_model, product, sample_nb), past(past), t(t)
	{}
};