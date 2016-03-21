#include "PortfolioManager.hpp"
#include "pnl/pnl_finance.h"
#include <iostream>

PortfolioManager::PortfolioManager(Product &product, UnderlyingModel &model, BlackScholesModelMarket &market, Pricing &pricing_unit, Hedging &hedging_unit, 
	int rebalancing_times, int monitoring_times) :
	portfolio_(),
	rebalancing_times_(rebalancing_times),
	monitoring_times_(monitoring_times),
	product_(product),
	model_(model),
	market_(market),
	pricing_unit_(pricing_unit),
	hedging_unit_(hedging_unit)
{
	spot_ = pnl_vect_create(product.get_underlying_number() * 2);
}

double PortfolioManager::hedge()
{
	// Simulation du marché
	const PnlMat * market_path = market_.get_market_asset_paths();
	pnl_mat_get_row(spot_, market_path, 0);
	//PnlMat * past = pnl_mat_create_from_zero(monitoring_times_ + 1, model_.underlying_number());
	PnlMat * past = pnl_mat_create_from_zero(monitoring_times_ + 1, 2*model_.underlying_number());
	pnl_mat_set_row(past, spot_, 0);

	// Initialisation du portefeuille
	double product_price = 0;
	double ic = 0;
	pricing_unit_.price(spot_, product_price, ic);
	double p0 = product_price;
	std::cout << "p0 : " << p0 << " - ic : " << ic << std::endl;

	
	PnlVect *deltas = pnl_vect_create_from_zero(2*model_.underlying_number());
	hedging_unit_.hedge(spot_, deltas);
	PnlVect *prices = pnl_vect_create_from_zero(2*model_.underlying_number());
	pnl_mat_get_row(prices, market_path, 0);

	portfolio_.initialisation(p0, deltas, prices);
	
	// Rebalancement du portefeuille
	double step = product_.get_maturity() / rebalancing_times_;
	// Pour changer les vol et les corrélations dynamiquement pendant le rebalancement si backtest
	PnlVect * volatilities = pnl_vect_create(2 * model_.underlying_number());
	PnlMat * correlations = pnl_mat_create(2 * model_.underlying_number(), 2 * model_.underlying_number());

	for (int i = 1; i < rebalancing_times_; ++i)
	{
		// Rebalancement au temps i * step
		manage_market_path_for_pricing_at_time_T(market_path, past, i * step);
		// Recalcul la volatilité
		market_.get_volatilities_and_correlations(volatilities, correlations, i);
		// Modification de la vol et des corrélations avant pricing et hedging
		model_.set_volatilities_and_correlations(volatilities, correlations);

		hedging_unit_.hedge_at(i * step, past, deltas);

		pnl_mat_get_row(prices, market_path, i);
		portfolio_.rebalancing(deltas, prices, model_.interest_rate(), step);
		double price, ic;
		pricing_unit_.price_at(i * step, past, price, ic);
		std::cout << i << " | " << portfolio_.compute_value(prices) << " - " << price << std::endl;
	}
	// Valeur finale du portefeuille
	pnl_mat_get_row(prices, market_path, rebalancing_times_);
	double portfolio_value = portfolio_.compute_final_value(prices, model_.interest_rate(), step);
	// Payoff à maturité
	manage_market_path_for_pricing_at_time_T(market_path, past, rebalancing_times_ * step);

	// On modifie past pour passer seulement les trajectoires des indices à product.get_payoff
	PnlMat * generated_foreign_assets_paths = pnl_mat_create(past->m, past->n / 2);
	double timestep = product_.get_maturity() / monitoring_times_;
	get_generated_foreign_asset_paths(past, generated_foreign_assets_paths, model_.foreign_interest_rates(), timestep);
	double payoff = product_.get_payoff(generated_foreign_assets_paths);

	// Calcul du P&L
	std::cout << "portfolio_value: " << portfolio_.compute_value(prices) << "  payoff: " << payoff << "  p0: " << p0 << std::endl;
	double pl = 100 * fabs(portfolio_value - payoff) / p0;

	// Libération de la mémoire
	pnl_mat_free(&past);
	pnl_mat_free(&generated_foreign_assets_paths);
	pnl_vect_free(&deltas);
	pnl_vect_free(&prices);
	pnl_vect_free(&volatilities);
	pnl_mat_free(&correlations);
	return pl;
}

void PortfolioManager::manage_market_path_for_pricing_at_time_T(const PnlMat *market_path, PnlMat *past, double t)
{
	int market_index = static_cast<int>(round(t * rebalancing_times_ / product_.get_maturity()));
	int past_index = static_cast<int>(floor((t - TIME_PRECISION) / (product_.get_maturity() / monitoring_times_) + 1));
	pnl_mat_resize(past, past_index + 1, past->n);
	for (int j = 0; j < 2*model_.underlying_number(); ++j)
	{
		MLET(past, past_index, j) = MGET(market_path, market_index, j);
	}
}

PortfolioManager::~PortfolioManager()
{
	pnl_vect_free(&spot_);
}