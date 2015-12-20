#pragma once

#include <vector>
#include "UnderlyingModel.hpp"
#include "BlackScholesModelInputParser.hpp"

namespace models
{

	class BlackScholesModel : public UnderlyingModel
	{
	public:
		BlackScholesModel(const input_parsers::BlackScholesModelInputParser &parser, const generators::RandomGeneration &random_generator);
		const PnlMat* const simulate_asset_paths_from_time(const double from_time, const PnlMat * const past_values) const;
		const PnlMat* const simulate_asset_paths_from_start(const PnlVect * const spot) const;
		void get_shifted_asset_paths(const PnlMat * const asset_path, int underlying_to_shift, double shift, double from_time, int past_values_number, PnlMat * shifted_asset_path) const;
		~BlackScholesModel();


	private:

		double final_simulation_date_;
		double timestep_;
		PnlVect * volatilities;
		double correlation_parameter_;
		PnlMat * cholesky_matrix_corr_;
		int compute_cholesky_matrix();
		const double TIME_PRECISION = 10.e-5;

		const generators::RandomGeneration &random_generator_;

		PnlMat * generated_asset_paths_;
		PnlVect * gaussian_vector_for_simulation_;

		void add_one_simulation_to_generated_asset_paths(int at_line, double timelength, const PnlVect * const last_values) const;
		void fill_remainder_of_generated_asset_paths(int from_line) const;
	};

}