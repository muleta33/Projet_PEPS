#pragma once
#include <vector>
#include "BlackScholesModelRiskNeutral.hpp"

using namespace models;

BlackScholesModelRiskNeutral::BlackScholesModelRiskNeutral(const input_parsers::BlackScholesModelInputParser &parser, const generators::RandomGeneration &random_generator) 
{
	interest_rate_ = parser.get_interest_rate();

	underlying_number_ = parser.get_underlying_number();

	// à changer le vecteur des taux d'interets étrangers
	interest_rates_ = pnl_vect_create_from_scalar(underlying_number_, interest_rate_);
	//TODO
	
	//PnlVect * trend = pnl_vect_create_from_scalar(underlying_number_, interest_rate_);
	PnlVect * trend = pnl_vect_create_from_scalar(2*underlying_number_, interest_rate_);

	routine = new BlackScholesModelRoutine(underlying_number_, parser.get_monitoring_times(), parser.get_maturity(), trend, parser.get_volatility(),
		parser.get_correlation_matrix(), random_generator);

	//generated_asset_paths_ = pnl_mat_create(parser.get_monitoring_times() + 1, parser.get_underlying_number());
	generated_asset_paths_ = pnl_mat_create(parser.get_monitoring_times() + 1, 2*parser.get_underlying_number());
	timestep_ = parser.get_maturity() / parser.get_monitoring_times();
	pnl_vect_free(&trend);
}

//TODO : noms à changer?
const PnlMat* const BlackScholesModelRiskNeutral::simulate_asset_paths_from_start(const PnlVect * const spot) const
{
	pnl_mat_set_row(generated_asset_paths_, spot, 0);
	routine->fill_remainder_of_generated_asset_paths(1, generated_asset_paths_);

	PnlMat * generated_foreign_asset_paths = pnl_mat_new();
	get_generated_foreign_asset_paths(generated_asset_paths_, generated_foreign_asset_paths);

	return generated_foreign_asset_paths;
}

//TODO : idem
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

	PnlMat * generated_foreign_asset_paths = pnl_mat_new();
	get_generated_foreign_asset_paths(generated_asset_paths_, generated_foreign_asset_paths);

	return generated_foreign_asset_paths;

}

void BlackScholesModelRiskNeutral::get_generated_foreign_asset_paths(const PnlMat * const generated_asset_paths, PnlMat * generated_foreing_asset_paths) const
{
	if (generated_foreing_asset_paths == NULL)
		generated_foreing_asset_paths = pnl_mat_new();
	pnl_mat_resize(generated_foreing_asset_paths, generated_asset_paths->m, generated_asset_paths->n / 2);
	for (int i = 0; i < generated_foreing_asset_paths->m; i++)
	{
		for (int j = 0; j < generated_foreing_asset_paths->n; j++)
		{
			MLET(generated_foreing_asset_paths, i, j) = MGET(generated_asset_paths, i, j) / MGET(generated_asset_paths, i, (j + generated_asset_paths->n / 2))
				*exp(GET(interest_rates_, j) * i* timestep_);
		}
	}

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
	generated_asset_paths_ = nullptr;
	delete(routine);
}