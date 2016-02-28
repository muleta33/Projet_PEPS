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

	public:
		UnderlyingModel() {};
		int underlying_number() const { return underlying_number_; }
		double interest_rate() const { return interest_rate_; }
		virtual const PnlMat* const simulate_asset_paths_from_time(const double from_time, const PnlMat * const past_values) const = 0;
		virtual const PnlMat* const simulate_asset_paths_from_start(const PnlVect * const spot) const = 0;
		virtual void get_generated_foreign_asset_paths(const PnlMat * const generated_asset_paths, PnlMat * generated_foreign_asset_paths) const = 0;
		virtual void get_shifted_asset_paths(const PnlMat * const asset_path, int underlying_to_shift, double shift, double from_time, int past_values_number, PnlMat * shifted_asset_path) const = 0;
		virtual ~UnderlyingModel() {};
	};

}