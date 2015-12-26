#pragma once
#include <vector>
#include "BlackScholesModelMarket.hpp"

using namespace models;

BlackScholesModelMarket::BlackScholesModelMarket(const BlackScholesModelInputParserProfitAndLoss &parser, const generators::RandomGeneration &random_generator)
{
	routine = new BlackScholesModelRoutine(parser.get_underlying_number(), parser.get_rebalancing_times(), parser.get_final_simulation_date(), parser.get_trend(), parser.get_volatility(),
		parser.get_correlation_parameter(), random_generator);
	generated_market_asset_paths_ = pnl_mat_create(parser.get_rebalancing_times() + 1, parser.get_underlying_number());
	timestep_ = parser.get_final_simulation_date() / parser.get_rebalancing_times();
}

const PnlMat* const BlackScholesModelMarket::simulate_market_asset_paths(const PnlVect * const spot) const
{
	pnl_mat_set_row(generated_market_asset_paths_, spot, 0);
	routine->fill_remainder_of_generated_asset_paths(1, generated_market_asset_paths_);
	return generated_market_asset_paths_;
}

BlackScholesModelMarket::~BlackScholesModelMarket()
{
	pnl_mat_free(&generated_market_asset_paths_);
	generated_market_asset_paths_ = nullptr;
	delete(routine);
}