#pragma once 
#include "pnl/pnl_matrix.h"
#include "RandomGeneration.hpp"

namespace models
{

	class UnderlyingModel
	{
	protected:
		int underlying_number_;
		double interest_rate_;
		PnlVect * foreign_interest_rates_;
		PnlVect * volatilities_;
		PnlMat * correlations_;

	public:
		UnderlyingModel() {};
		int underlying_number() const { return underlying_number_; }
		double interest_rate() const { return interest_rate_; }

		virtual void set_volatilities_and_correlations(const PnlVect * volatilities, const PnlMat * correlations) { 
			pnl_vect_clone(volatilities_, volatilities); 
			pnl_mat_clone(correlations_, correlations);
		}

		PnlVect * foreign_interest_rates() const { return foreign_interest_rates_; }
		virtual const PnlMat* const simulate_asset_paths_from_time(const double from_time, const PnlMat * const past_values) const = 0;
		virtual const PnlMat* const simulate_asset_paths_from_start(const PnlVect * const spot) const = 0;
		virtual void get_shifted_asset_paths(const PnlMat * const asset_path, int underlying_to_shift, double shift, double from_time, int past_values_number, PnlMat * shifted_asset_path) const = 0;
		virtual ~UnderlyingModel() {};
	};

}