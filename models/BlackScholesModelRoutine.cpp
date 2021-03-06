#include <cmath>
#include "BlackScholesModelRoutine.hpp"
#include <iostream>

using namespace input_parameters;
using namespace generators;

namespace models {
	BlackScholesModelRoutine::BlackScholesModelRoutine(int underlying_number, int monitoring_times, double maturity, const PnlVect * trend, const PnlVect * volatilities,
		double correlation_parameter, const generators::RandomGeneration &random_generator) :
		underlying_number_(underlying_number), maturity_(maturity), random_generator_(random_generator)
	{
		trend_ = pnl_vect_copy(trend);
		volatilities_ = pnl_vect_copy(volatilities);
		//gaussian_vector_for_simulation_ = pnl_vect_create(underlying_number_);
		gaussian_vector_for_simulation_ = pnl_vect_create(2 * underlying_number_);
		timestep_ = maturity_ / monitoring_times;
		//cholesky_matrix_corr_ = pnl_mat_create(underlying_number_, underlying_number_);
		cholesky_matrix_corr_ = pnl_mat_create(2 * underlying_number_, 2 * underlying_number_);
		compute_cholesky_matrix(correlation_parameter);
	}

	BlackScholesModelRoutine::BlackScholesModelRoutine(int underlying_number, int monitoring_times, double maturity, const PnlVect * trend, const PnlVect * volatilities,
		const PnlMat * correlation_matrix, const generators::RandomGeneration &random_generator) :
		underlying_number_(underlying_number), maturity_(maturity), random_generator_(random_generator)
	{
		trend_ = pnl_vect_copy(trend);
		volatilities_ = pnl_vect_copy(volatilities);
		//gaussian_vector_for_simulation_ = pnl_vect_create(underlying_number_);
		gaussian_vector_for_simulation_ = pnl_vect_create(2 * underlying_number_);
		timestep_ = maturity_ / monitoring_times;
		//cholesky_matrix_corr_ = pnl_mat_create(underlying_number_, underlying_number_);
		cholesky_matrix_corr_ = pnl_mat_create(2 * underlying_number_, 2 * underlying_number_);
		compute_cholesky_matrix(correlation_matrix);
	}

	int BlackScholesModelRoutine::compute_cholesky_matrix(double correlation_parameter)
	{
		int i, j;
		//for (i = 0; i < underlying_number_; i++)
		for (i = 0; i < 2 * underlying_number_; i++)
		{
			MLET(cholesky_matrix_corr_, i, i) = 1.0;
			for (j = 0; j < i; j++)
			{
				MLET(cholesky_matrix_corr_, i, j) = correlation_parameter;
				MLET(cholesky_matrix_corr_, j, i) = correlation_parameter;
			}
		}
		return pnl_mat_chol(cholesky_matrix_corr_);
	}

	int BlackScholesModelRoutine::compute_cholesky_matrix(const PnlMat * correlation_matrix)
	{
		pnl_mat_clone(cholesky_matrix_corr_, correlation_matrix);
		return pnl_mat_chol(cholesky_matrix_corr_);
	}


	void BlackScholesModelRoutine::fill_remainder_of_generated_asset_paths(int from_line, PnlMat * generated_asset_paths) const
	{
		double sqrt_timelength = sqrt(timestep_);
		int last_line = generated_asset_paths->m;
		//PnlVect * last_values = pnl_vect_create_from_zero(underlying_number_);
		PnlVect * last_values = pnl_vect_create_from_zero(2 * underlying_number_);
		for (int line = from_line; line < last_line; line++)
		{
			pnl_mat_get_row(last_values, generated_asset_paths, line - 1);
			add_one_simulation_to_generated_asset_paths(line, timestep_, last_values, generated_asset_paths);
		}
		pnl_vect_free(&last_values);
	}


	void BlackScholesModelRoutine::add_one_simulation_to_generated_asset_paths(int at_line, double time_length, const PnlVect * const last_values, PnlMat * generated_asset_paths) const
	{
		random_generator_.get_one_gaussian_sample(gaussian_vector_for_simulation_);
		//PnlVect * work_vector = pnl_vect_create_from_zero(underlying_number_);
		PnlVect * work_vector = pnl_vect_create_from_zero(2 * underlying_number_);
		double sqrt_time_length = sqrt(time_length);
		pnl_mat_mult_vect_inplace(work_vector, cholesky_matrix_corr_, gaussian_vector_for_simulation_);
		//for (int i = 0; i < underlying_number_; i++)
		for (int i = 0; i < 2 * underlying_number_; i++)
		{
			double vol = GET(volatilities_, i);
			MLET(generated_asset_paths, at_line, i) = GET(last_values, i) * exp((GET(trend_, i) - (vol *  vol / 2.0)) * time_length
				+ sqrt_time_length * vol * GET(work_vector, i));
		}
		pnl_vect_free(&work_vector);
	}


	BlackScholesModelRoutine::~BlackScholesModelRoutine()
	{
		pnl_vect_free(&gaussian_vector_for_simulation_);
		pnl_mat_free(&cholesky_matrix_corr_);
	}
}