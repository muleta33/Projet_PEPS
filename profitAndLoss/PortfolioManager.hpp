#pragma once
#include "Portfolio.hpp"
#include "EurostralMutualFund.hpp"
#include "MonteCarloRoutine.hpp"
#include "UnderlyingModel.hpp"
#include "BlackScholesModelMarket.hpp"

using namespace models;
using namespace products;

class PortfolioManager
{
public:
	PortfolioManager(EurostralMutualFund &product, UnderlyingModel &model, BlackScholesModelMarket &market, int rebalancing_times, 
		int monitoring_times, double fd_step, int number_of_samples, const PnlVect *spot);
	double hedge();
	~PortfolioManager();

private:
	Portfolio *portfolio_;
	int rebalancing_times_;
	int monitoring_times_;
	double fd_step_;
	PnlVect *spot_;
	EurostralMutualFund &product_;
	UnderlyingModel &model_;
	BlackScholesModelMarket &market_;
	MonteCarloRoutine *mc_;
	const double TIME_PRECISION = 10e-5;
	int number_of_samples_;

	void manage_market_path_for_pricing_at_time_T(const PnlMat *market_path, PnlMat *past, double t);
};
