#pragma once
#include "BlackScholesModelMarketHistorical.hpp"
#include "InputParametersUtilities.hpp"

using namespace models;

BlackScholesModelMarketHistorical::BlackScholesModelMarketHistorical(const PnlMat * market_data, int underlying_number, int number_volatility_dates) :
	underlying_number_(underlying_number), number_volatility_dates_(number_volatility_dates)
{
	market_asset_paths_ = pnl_mat_create(market_data->m, market_data->n);
	pnl_mat_clone(market_asset_paths_, market_data);
}

const PnlMat* const BlackScholesModelMarketHistorical::get_market_asset_paths() const
{
	PnlMat * market_data = pnl_mat_create(market_asset_paths_->m - number_volatility_dates_, 2 * underlying_number_);
	pnl_mat_extract_subblock(market_data, market_asset_paths_, number_volatility_dates_, market_asset_paths_->m, 0, 2 * underlying_number_);
	return market_data;
}

void BlackScholesModelMarketHistorical::get_volatilities_and_correlations(PnlVect * volatilities, PnlMat * correlations, int before) const {
	PnlMat * past_data = pnl_mat_create(number_volatility_dates_, 2 * underlying_number_);
	pnl_mat_extract_subblock(past_data, market_asset_paths_, before, number_volatility_dates_, 0, 2 * underlying_number_);
	PnlMat * past_returns = pnl_mat_create(number_volatility_dates_ - 1, 2 * underlying_number_);
	compute_returns(past_returns, past_data);
	compute_volatilities(volatilities, past_returns);
	compute_correlation_matrix(correlations, past_returns, volatilities);
	pnl_mat_free(&past_data);
	pnl_mat_free(&past_returns);
}

BlackScholesModelMarketHistorical::~BlackScholesModelMarketHistorical()
{
	pnl_mat_free(&market_asset_paths_);
	market_asset_paths_ = nullptr;
}