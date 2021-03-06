#pragma once
#include <vector>
#include "BlackScholesModelRiskNeutral.hpp"
#include "BlackScholesModelUtilities.hpp"

using namespace models;

BlackScholesModelRiskNeutral::BlackScholesModelRiskNeutral(const input_parameters::BlackScholesModelInputParameters &input_parameters, const generators::RandomGeneration &random_generator)
{
	interest_rate_ = input_parameters.get_interest_rate();

	underlying_number_ = input_parameters.get_underlying_number();

	// taux de change étrangers
	foreign_interest_rates_ = pnl_vect_new();
	pnl_vect_clone(foreign_interest_rates_, input_parameters.get_foreign_interest_rates());
	
	//PnlVect * trend = pnl_vect_create_from_scalar(underlying_number_, interest_rate_);
	PnlVect * trend = pnl_vect_create_from_scalar(2*underlying_number_, interest_rate_);

	volatilities_ = pnl_vect_new();
	pnl_vect_clone(volatilities_, input_parameters.get_volatility());

	correlations_ = pnl_mat_new();
	pnl_mat_clone(correlations_, input_parameters.get_correlation_matrix());

	routine = new BlackScholesModelRoutine(underlying_number_, input_parameters.get_monitoring_times(), input_parameters.get_maturity(), trend, volatilities_,
		input_parameters.get_correlation_matrix(), random_generator);

	//generated_asset_paths_ = pnl_mat_create(parser.get_monitoring_times() + 1, parser.get_underlying_number());
	generated_asset_paths_ = pnl_mat_create(input_parameters.get_monitoring_times() + 1, 2 * input_parameters.get_underlying_number());
	generated_foreign_asset_paths_ = pnl_mat_create(input_parameters.get_monitoring_times() + 1, input_parameters.get_underlying_number());
	timestep_ = input_parameters.get_maturity() / input_parameters.get_monitoring_times();

	pnl_vect_free(&trend);
}


const PnlMat* const BlackScholesModelRiskNeutral::simulate_asset_paths_from_start(const PnlVect * const spot) const
{
	pnl_mat_set_row(generated_asset_paths_, spot, 0);
	routine->fill_remainder_of_generated_asset_paths(1, generated_asset_paths_);


	get_generated_foreign_asset_paths(generated_asset_paths_, generated_foreign_asset_paths_, foreign_interest_rates_, timestep_);

	return generated_foreign_asset_paths_;
}


const PnlMat* const BlackScholesModelRiskNeutral::simulate_asset_paths_from_time(const double from_time, const PnlMat * const past_values) const
{
	pnl_mat_set_subblock(generated_asset_paths_, past_values, 0, 0);
	int number_of_values = past_values->m;
	double timespan_to_monitoring = (number_of_values - 1) * timestep_ - from_time;
	bool is_at_monitoring_time = timespan_to_monitoring < TIME_PRECISION;
	if (!is_at_monitoring_time)
	{
		//PnlVect * last_values = pnl_vect_create_from_zero(underlying_number_);
		PnlVect * last_values = pnl_vect_create_from_zero(2*underlying_number_);
		pnl_mat_get_row(last_values, past_values, number_of_values - 1);
		routine->add_one_simulation_to_generated_asset_paths(number_of_values - 1, timespan_to_monitoring, last_values, generated_asset_paths_);
		pnl_vect_free(&last_values);
	}
	routine->fill_remainder_of_generated_asset_paths(number_of_values, generated_asset_paths_);

	get_generated_foreign_asset_paths(generated_asset_paths_, generated_foreign_asset_paths_, foreign_interest_rates_, timestep_);

	return generated_foreign_asset_paths_;

}

void BlackScholesModelRiskNeutral::get_shifted_asset_paths(const PnlMat * const asset_path, int underlying_to_shift, double shift, double from_time, int past_values_number, PnlMat * shifted_asset_path) const
{
	if (shifted_asset_path == NULL)
		shifted_asset_path = pnl_mat_new();
	pnl_mat_clone(shifted_asset_path, asset_path);
	double timespan_to_monitoring = (past_values_number - 1) * timestep_ - from_time;
	bool is_at_monitoring_time = timespan_to_monitoring < TIME_PRECISION;
	int begin_shift_time_index = 0;
	if (is_at_monitoring_time)
		begin_shift_time_index = past_values_number;
	else
		begin_shift_time_index = past_values_number - 1;
	for (int i = begin_shift_time_index; i < asset_path->m; ++i)
	{
		MLET(shifted_asset_path, i, underlying_to_shift) = MGET(asset_path, i, underlying_to_shift) * (1. + shift);
	}
}

BlackScholesModelRiskNeutral::~BlackScholesModelRiskNeutral()
{
	pnl_mat_free(&generated_asset_paths_);
	pnl_mat_free(&generated_foreign_asset_paths_);
	pnl_vect_free(&volatilities_);
	pnl_mat_free(&correlations_);
	pnl_vect_free(&foreign_interest_rates_);
	generated_asset_paths_ = nullptr;
	generated_foreign_asset_paths_ = nullptr;
	volatilities_ = nullptr;
	correlations_ = nullptr;
	foreign_interest_rates_ = nullptr;
	delete(routine);
}