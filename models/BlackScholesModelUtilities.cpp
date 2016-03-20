#include "BlackScholesModelUtilities.hpp"
#include <math.h>

namespace models {

	void get_generated_foreign_asset_paths(const PnlMat * generated_asset_paths, PnlMat * generated_foreign_asset_paths,
		PnlVect * foreign_interest_rates, double timestep) {

		if (generated_foreign_asset_paths == NULL)
			generated_foreign_asset_paths = pnl_mat_new();
		pnl_mat_resize(generated_foreign_asset_paths, generated_asset_paths->m, generated_asset_paths->n / 2);
		for (int i = 0; i < generated_foreign_asset_paths->m; i++)
		{
			for (int j = 0; j < generated_foreign_asset_paths->n; j++)
			{
				MLET(generated_foreign_asset_paths, i, j) = MGET(generated_asset_paths, i, j) / MGET(generated_asset_paths, i, (j + generated_asset_paths->n / 2))
					* exp(GET(foreign_interest_rates, j) * i * timestep);
			}
		}
	}

	void historical_data_to_domestic_currency(const PnlMat * past_data, PnlMat * past_domestic_data, PnlVect * foreign_interest_rates, double timestep) {
		if (past_domestic_data == NULL)
			past_domestic_data = pnl_mat_new();
		pnl_mat_resize(past_domestic_data, past_data->m, past_data->n);
		for (int i = 0; i < past_domestic_data->m; i++)
		{
			for (int j = 0; j < past_domestic_data->n/2; j++)
			{
				MLET(past_domestic_data, i, j) = MGET(past_data, i, j) * MGET(past_data, i, (j + past_data->n / 2));
				MLET(past_domestic_data, i, j + past_data->n / 2) = MGET(past_data, i, j + past_data->n / 2) * exp(GET(foreign_interest_rates, j)*i*timestep);
			}
		}


	}
}