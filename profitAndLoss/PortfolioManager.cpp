#include "PortfolioManager.hpp"
#include "pnl/pnl_finance.h"
#include <iostream>

PortfolioManager::PortfolioManager(Product &product, UnderlyingModel &model, BlackScholesModelMarket &market, int rebalancing_times,
	int monitoring_times, double fd_step, int number_of_samples, const PnlVect *spot) :
	portfolio_(),
	rebalancing_times_(rebalancing_times),
	monitoring_times_(monitoring_times),
	number_of_samples_(number_of_samples),
	fd_step_(fd_step),
	product_(product),
	model_(model),
	market_(market)
{
	spot_ = pnl_vect_copy(spot);
}

double PortfolioManager::hedge()
{
	// Simulation du marché
	const PnlMat * market_path = market_.simulate_market_asset_paths(spot_);
	PnlMat * past = pnl_mat_create_from_zero(monitoring_times_ + 1, model_.underlying_number());
	pnl_mat_set_row(past, spot_, 0);

	// Initialisation du portefeuille
	double product_price = 0;
	double ic = 0;
	MonteCarloRoutineAtOrigin mc(model_, product_, number_of_samples_, spot_);
	mc.price(product_price, ic);
	double p0 = product_price;
	std::cout << "p0 : " << p0 << " - ic : " << ic << std::endl;

	PnlVect *deltas = pnl_vect_create_from_zero(model_.underlying_number());
	mc.delta_hedge(fd_step_, deltas);

	PnlVect *prices = pnl_vect_create_from_zero(model_.underlying_number());
	pnl_mat_get_row(prices, market_path, 0);

	portfolio_.initialisation(p0, deltas, prices);
	// Rebalancement du portefeuille
	double step = product_.get_maturity() / rebalancing_times_;

	for (int i = 1; i < rebalancing_times_; ++i)
	{
		// Rebalancement au temps i * step
		manage_market_path_for_pricing_at_time_T(market_path, past, i * step);
		MonteCarloRoutineAtTimeT mcAtTimeT(model_, product_, number_of_samples_, past, i * step);
		mcAtTimeT.delta_hedge(fd_step_, deltas);
		pnl_mat_get_row(prices, market_path, i);
		portfolio_.rebalancing(deltas, prices, model_.interest_rate(), step);
		double price, ic;
		mcAtTimeT.price(price, ic);
		std::cout << i << " | " << portfolio_.compute_value(prices) << " - " << price << std::endl;
	}
	// Valeur finale du portefeuille
	pnl_mat_get_row(prices, market_path, rebalancing_times_);
	double portfolio_value = portfolio_.compute_final_value(prices, model_.interest_rate(), step);
	// Payoff à maturité
	manage_market_path_for_pricing_at_time_T(market_path, past, rebalancing_times_ * step);
	double payoff = product_.get_payoff(past);
	// Calcul du P&L
	std::cout << "portfolio_value: " << portfolio_.compute_value(prices) << "  payoff: " << payoff << "  p0: " << p0 << std::endl;
	double pl = 100 * fabs(portfolio_value - payoff) / p0;

	// Libération de la mémoire
	pnl_mat_free(&past);
	pnl_vect_free(&deltas);
	pnl_vect_free(&prices);
	
	return pl;
}

void PortfolioManager::manage_market_path_for_pricing_at_time_T(const PnlMat *market_path, PnlMat *past, double t)
{
	int market_index = static_cast<int>(round(t * rebalancing_times_ / product_.get_maturity()));
	int past_index = static_cast<int>(floor((t - TIME_PRECISION) / (product_.get_maturity() / monitoring_times_) + 1));
	pnl_mat_resize(past, past_index + 1, past->n);
	for (int j = 0; j < model_.underlying_number(); ++j)
	{
		MLET(past, past_index, j) = MGET(market_path, market_index, j);
	}
}

PortfolioManager::~PortfolioManager()
{
	pnl_vect_free(&spot_);
}