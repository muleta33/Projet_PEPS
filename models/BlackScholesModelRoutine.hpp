#pragma once
#include <vector>
#include "UnderlyingModel.hpp"
#include "BlackScholesModelInputParser.hpp"
#include "RandomGeneration.hpp"

namespace models
{

	class BlackScholesModelRoutine
	{
	public:
		BlackScholesModelRoutine(int underlying_number, int monitoring_times, double final_simulation_date, double trend, PnlVect * volatilities,
			double correlation_parameter, const generators::RandomGeneration &random_generator);
		void add_one_simulation_to_generated_asset_paths(int at_line, double timelength, const PnlVect * const last_values, PnlMat * generated_asset_paths) const;
		void fill_remainder_of_generated_asset_paths(int from_line, PnlMat * generated_asset_paths) const;
		~BlackScholesModelRoutine();

	private:
		int underlying_number_;
		double timestep_;
		double final_simulation_date_;
		double trend_;
		PnlVect * volatilities_;
		double correlation_parameter_;
		PnlMat * cholesky_matrix_corr_;
		int compute_cholesky_matrix();
		const generators::RandomGeneration &random_generator_;
		PnlVect * gaussian_vector_for_simulation_;
	};

}