#pragma once
#include <vector>
#include "BlackScholesModelMarketSimulate.hpp"

using namespace models;

BlackScholesModelMarketSimulate::BlackScholesModelMarketSimulate(const BlackScholesModelInputParser &model_parser, const ProfitAndLossInputParser &pl_parser,
	const generators::RandomGeneration &random_generator)
{
	routine_ = new BlackScholesModelRoutine(model_parser.get_underlying_number(), pl_parser.get_rebalancing_times(), model_parser.get_maturity(), 
		pl_parser.get_trend(), model_parser.get_volatility(), model_parser.get_correlation_matrix(), random_generator);
	market_asset_paths_ = pnl_mat_create(pl_parser.get_rebalancing_times() + 1, 2*model_parser.get_underlying_number());
	spot_ = pnl_vect_new();
	pnl_vect_clone(spot_, pl_parser.get_spot());
	volatilities_ = pnl_vect_new();
	pnl_vect_clone(volatilities_, model_parser.get_volatility());
	correlations_ = pnl_mat_new();
	pnl_mat_clone(correlations_, model_parser.get_correlation_matrix());
}

const PnlMat* const BlackScholesModelMarketSimulate::get_market_asset_paths() const
{
	pnl_mat_set_row(market_asset_paths_, spot_, 0);
	routine_->fill_remainder_of_generated_asset_paths(1, market_asset_paths_);
	return market_asset_paths_;
}

void BlackScholesModelMarketSimulate::get_volatilities_and_correlations(PnlVect * volatilities, PnlMat * correlations, int before) const
{
	volatilities = pnl_vect_copy(volatilities_);
	correlations = pnl_mat_copy(correlations_);
}

BlackScholesModelMarketSimulate::~BlackScholesModelMarketSimulate()
{
	pnl_vect_free(&spot_);
	spot_ = nullptr;
	pnl_vect_free(&volatilities_);
	volatilities_ = nullptr;
	pnl_mat_free(&correlations_);
	correlations_ = nullptr;
	pnl_mat_free(&market_asset_paths_);
	market_asset_paths_ = nullptr;
	delete(routine_);
}