#pragma once
#include <vector>
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "UnderlyingModel.hpp"
#include "BlackScholesModelInputParser.hpp"
#include "RandomGeneration.hpp"

namespace models
{

	class BlackScholesModelRoutine
	{
	public:
		BlackScholesModelRoutine(int underlying_number, int monitoring_times, double maturity, const PnlVect * trend, const PnlVect * volatilities,
			double correlation_parameter, const generators::RandomGeneration &random_generator);
		BlackScholesModelRoutine(int underlying_number, int monitoring_times, double maturity, const PnlVect * trend, const PnlVect * volatilities,
			const PnlMat * correlation_matrix, const generators::RandomGeneration &random_generator);
		void add_one_simulation_to_generated_asset_paths(int at_line, double timelength, const PnlVect * const last_values, PnlMat * generated_asset_paths) const;
		void fill_remainder_of_generated_asset_paths(int from_line, PnlMat * generated_asset_paths) const;

		void set_volatilities_and_correlations(PnlVect * volatilities, PnlMat * correlations)
		{
			volatilities_ = pnl_vect_copy(volatilities);
			compute_cholesky_matrix(correlations);
		}

		~BlackScholesModelRoutine();


	private:
		int underlying_number_;
		double timestep_;
		double maturity_;
		PnlVect * trend_;
		PnlVect * volatilities_;
		PnlMat * cholesky_matrix_corr_;
		int compute_cholesky_matrix(double correlation_parameter);
		int compute_cholesky_matrix(const PnlMat * correlation_matrix);
		const generators::RandomGeneration &random_generator_;
		PnlVect * gaussian_vector_for_simulation_;
		
	};

}