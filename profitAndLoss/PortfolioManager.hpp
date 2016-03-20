#pragma once
#include "Portfolio.hpp"
#include "EurostralMutualFund.hpp"
#include "UnderlyingModel.hpp"
#include "BlackScholesModelMarket.hpp"
#include "BlackScholesModelUtilities.hpp"
#include "Pricing.hpp"
#include "Hedging.hpp"

using namespace models;
using namespace products;

class PortfolioManager
{
public:
	PortfolioManager(Product &product, UnderlyingModel &model, BlackScholesModelMarket &market, Pricing &pricing_unit, Hedging &hedging_unit, 
		int rebalancing_times, int monitoring_times);
	double hedge();
	~PortfolioManager();

private:
	Portfolio portfolio_;
	int rebalancing_times_;
	int monitoring_times_;
	double fd_step_;
	PnlVect *spot_;
	Product &product_;
	UnderlyingModel &model_;
	BlackScholesModelMarket &market_;
	Pricing &pricing_unit_;
	Hedging &hedging_unit_;
	const double TIME_PRECISION = 10e-5;
	int number_of_samples_;

	void manage_market_path_for_pricing_at_time_T(const PnlMat *market_path, PnlMat *past, double t);
};
