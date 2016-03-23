#pragma once
#include "pnl/pnl_matrix.h"

namespace models {

	extern void get_generated_foreign_asset_paths(const PnlMat * generated_asset_paths, PnlMat * generated_foreign_asset_paths,
		PnlVect * foreign_interest_rates, double timestep);

	extern void historical_data_to_domestic_currency(const PnlMat * past_data, PnlMat * past_domestic_data, 
		PnlVect * foreign_interest_rates, double timestep);

	extern void foreign_to_domestic_spots(const PnlVect * foreign_spots, PnlVect * domestic_spots);

	extern void historical_to_domestic_past_data(const PnlMat * past_data, PnlMat * past_domestic_data,
		PnlVect * foreign_spots, PnlVect * foreign_interest_rates, double time, double timestep);
}