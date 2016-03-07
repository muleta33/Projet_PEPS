#pragma once
#include <vector>
#include "BlackScholesModelMarket.hpp"

using namespace models;

BlackScholesModelMarket::BlackScholesModelMarket(const BlackScholesModelInputParser &model_parser, const ProfitAndLossInputParser &pl_parser, 
	const generators::RandomGeneration &random_generator)
{
	routine = new BlackScholesModelRoutine(model_parser.get_underlying_number(), pl_parser.get_rebalancing_times(), model_parser.get_maturity(), 
		pl_parser.get_trend(), model_parser.get_volatility(), model_parser.get_correlation_matrix(), random_generator);
	generated_market_asset_paths_ = pnl_mat_create(pl_parser.get_rebalancing_times() + 1, 2*model_parser.get_underlying_number());
	timestep_ = model_parser.get_maturity() / pl_parser.get_rebalancing_times();
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