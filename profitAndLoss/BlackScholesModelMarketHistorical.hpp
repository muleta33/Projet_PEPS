#pragma once
#include "pnl/pnl_matrix.h"
#include "BlackScholesModelMarket.hpp"
#include "BlackScholesModelInputHistorical.hpp"


class BlackScholesModelMarketHistorical : public BlackScholesModelMarket
{

public:
	BlackScholesModelMarketHistorical(const PnlMat * market_data, int underlying_number, int number_volatility_dates);
	const PnlMat * const get_market_asset_paths() const;
	void get_volatilities_and_correlations(PnlVect * volatilities, PnlMat * correlations, int before) const;

	~BlackScholesModelMarketHistorical();

private:
	int underlying_number_;
	int number_volatility_dates_;
};