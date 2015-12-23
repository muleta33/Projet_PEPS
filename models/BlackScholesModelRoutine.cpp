#include <cmath>
#include "BlackScholesModelRoutine.hpp"
#include "pnl/pnl_matrix.h"
#include <iostream>

using namespace input_parsers;
using namespace generators;
using namespace models;

BlackScholesModelRoutine::BlackScholesModelRoutine(const BlackScholesModelInputParser &parser, const RandomGeneration &random_generator) :
UnderlyingModel(), random_generator_(random_generator)
{
	underlying_number_ = parser.get_underlying_number();
	interest_rate_ = parser.get_interest_rate();
	final_simulation_date_ = parser.get_final_simulation_date();
	int monitoring_times = parser.get_monitoring_times();
	timestep_ = static_cast<double>(final_simulation_date_) / monitoring_times;
	generated_asset_paths_ = pnl_mat_create(monitoring_times + 1, underlying_number_);
	gaussian_vector_for_simulation_ = pnl_vect_create(underlying_number_);
	correlation_parameter_ = parser.get_correlation_parameter();
	volatilities = parser.get_volatility();
	cholesky_matrix_corr_ = pnl_mat_create(underlying_number_, underlying_number_);
	compute_cholesky_matrix();
}

int BlackScholesModelRoutine::compute_cholesky_matrix()
{
	int i, j;
	for (i = 0; i < underlying_number_; i++)
	{
		MLET(cholesky_matrix_corr_, i, i) = 1.0;
		for (j = 0; j < i; j++)
		{
			MLET(cholesky_matrix_corr_, i, j) = correlation_parameter_;
			MLET(cholesky_matrix_corr_, j, i) = correlation_parameter_;
		}
	}
	return pnl_mat_chol(cholesky_matrix_corr_);
}


void BlackScholesModelRoutine::fill_remainder_of_generated_asset_paths(int from_line) const
{
	double sqrt_timelength = sqrt(timestep_);
	int last_line = generated_asset_paths_->m;
	PnlVect * last_values = pnl_vect_create_from_zero(underlying_number_);
	for (int line = from_line; line < last_line; line++)
	{
		pnl_mat_get_row(last_values, generated_asset_paths_, line - 1);
		add_one_simulation_to_generated_asset_paths(line, timestep_, last_values);
	}
	pnl_vect_free(&last_values);
}


void BlackScholesModelRoutine::add_one_simulation_to_generated_asset_paths(int at_line, double time_length, const PnlVect * const last_values) const
{
	random_generator_.get_one_gaussian_sample(gaussian_vector_for_simulation_);
	PnlVect * work_vector = pnl_vect_create_from_zero(underlying_number_);
	double sqrt_time_length = sqrt(time_length);
	pnl_mat_mult_vect_inplace(work_vector, cholesky_matrix_corr_, gaussian_vector_for_simulation_);
	for (int i = 0; i < underlying_number_; i++)
	{
		double vol = GET(volatilities, i);
		MLET(generated_asset_paths_, at_line, i) = GET(last_values, i) * exp((get_trend() - (vol *  vol / 2.0)) * time_length
			+ sqrt_time_length * vol * GET(work_vector, i));
	}
	pnl_vect_free(&work_vector);
}


BlackScholesModelRoutine::~BlackScholesModelRoutine()
{
	pnl_mat_free(&generated_asset_paths_);
	pnl_vect_free(&gaussian_vector_for_simulation_);
	generated_asset_paths_ = nullptr;
	gaussian_vector_for_simulation_ = nullptr;
}
