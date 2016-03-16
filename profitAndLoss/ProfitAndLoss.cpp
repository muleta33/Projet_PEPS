// ProfitAndLoss.cpp : Defines the entry point for the console application.
//
#pragma once
#include "stdafx.h"
#include <iostream>
#include "pnl\pnl_matrix.h"
#include "Parser.hpp"
#include "BlackScholesModelMarket.hpp"
#include "BlackScholesModelMarketSimulate.hpp"
#include "BlackScholesModelMarketHistorical.hpp"
#include "BlackScholesModelRiskNeutral.hpp"
#include "EurostralMutualFundInputParser.hpp"
#include "BasketOptionInputParser.hpp"
#include "BlackScholesModelInputParser.hpp"
#include "BlackScholesModelInputHistorical.hpp"
#include "HistoricalDataParser.hpp"
#include "ProfitAndLossInputParser.hpp"
#include "PNLRandomGeneration.hpp"
#include "PortfolioManager.hpp"
#include "BasketOption.hpp"
#include "MonteCarloPricing.hpp"
#include "MonteCarloHedging.hpp"
#include "PricingExactCall.hpp"
#include "HedgingExactCall.hpp"

using namespace products;
using namespace generators;
using namespace models;
using namespace input_parameters;

int main(int argc, char* argv[])
{
	// Eurostral pricing avec fichier dat
	/* const char * input_file = argv[1];
	Parser parser(input_file);
	const BlackScholesModelInputParser model_input_parameters(parser);
	const ProfitAndLossInputParser profit_and_loss_parser(parser);
	const EurostralMutualFundInputParser fund_parser(parser);
	EurostralMutualFund product(fund_parser);
	const PnlRandomGeneration random_generator;
	BlackScholesModelRiskNeutral model(model_input_parameters, random_generator);
	BlackScholesModelMarketSimulate market(model_input_parameters, profit_and_loss_parser, random_generator);
	MonteCarloPricing pricing_unit(model, product, profit_and_loss_parser.get_sample_number());
	MonteCarloHedging hedging_unit(model, product, profit_and_loss_parser.get_sample_number(), profit_and_loss_parser.get_fd_step()); */

	// Call/Basket pricing avec fichier dat
	/* const char * input_file = argv[1];
	Parser parser(input_file);
	const BlackScholesModelInputParser model_input_parameters(parser);
	const ProfitAndLossInputParser profit_and_loss_parser(parser);
	const BasketOptionInputParser basket_parser(parser);
	BasketOption product(basket_parser);
	const PnlRandomGeneration random_generator;
	BlackScholesModelRiskNeutral model(model_input_parameters, random_generator);
	BlackScholesModelMarketSimulate market(model_input_parameters, profit_and_loss_parser, random_generator);
	MonteCarloPricing pricing_unit(model, product, profit_and_loss_parser.get_sample_number());
	MonteCarloHedging hedging_unit(model, product, profit_and_loss_parser.get_sample_number(), profit_and_loss_parser.get_fd_step()); */

	// Call pricing exact
	/* const char * input_file = argv[1];
	Parser parser(input_file);
	const BlackScholesModelInputParser model_input_parameters(parser);
	const ProfitAndLossInputParser profit_and_loss_parser(parser);
	const BasketOptionInputParser basket_parser(parser);
	BasketOption product(basket_parser);
	const PnlRandomGeneration random_generator;
	BlackScholesModelRiskNeutral model(model_input_parameters, random_generator);
	BlackScholesModelMarketSimulate market(model_input_parameters, profit_and_loss_parser, random_generator);
	PricingExactCall pricing_unit(model, product);
	HedgingExactCall hedging_unit(model, product); */

	const char * config_file = argv[1];
	//const char * config_file = "EurostralMutualFund_month_change_historical.dat";
	Parser parser(config_file);
	// Construction du parser des données de marché
	const char * data_file = argv[2];
	//const char * data_file = "historical_data.csv";
	int underlying_number;
	int rebalancing_times;
	int number_past_data = 20;
	double maturity;
	PnlVect * foreign_interest_rates;
	parser.extract("maturity", maturity);
	parser.extract("option size", underlying_number);
	parser.extract("rebalancing times", rebalancing_times);
	parser.extract("foreign interest rates", foreign_interest_rates, underlying_number);
	double timestep = maturity / rebalancing_times;
	const HistoricalDataParser data_parser(data_file, rebalancing_times + number_past_data, underlying_number);
	// Modification des données passées pour que ça colle avec le modèle - on suppose que ce sont des données journalières
	PnlMat * past_data_domestic_currency = pnl_mat_create(number_past_data, underlying_number * 2);
	models::historical_data_to_domestic_currency(data_parser.get_past_data(number_past_data), past_data_domestic_currency, foreign_interest_rates, 1/360);
	const BlackScholesModelInputHistorical model_input_parameters(parser, past_data_domestic_currency);
	const ProfitAndLossInputParser profit_and_loss_parser(parser);
	const EurostralMutualFundInputParser fund_parser(parser);
	EurostralMutualFund product(fund_parser);
	const PnlRandomGeneration random_generator;
	BlackScholesModelRiskNeutral model(model_input_parameters, random_generator);
	// Modification des données du marché pour que ça colle avec le modèle
	PnlMat * market_data_domestic_currency = pnl_mat_create(rebalancing_times + number_past_data + 1, underlying_number * 2);
	models::historical_data_to_domestic_currency(data_parser.get_all_market_data(), market_data_domestic_currency, foreign_interest_rates, timestep);
	BlackScholesModelMarketHistorical market(market_data_domestic_currency, underlying_number, number_past_data);
	MonteCarloPricing pricing_unit(model, product, profit_and_loss_parser.get_sample_number());
	MonteCarloHedging hedging_unit(model, product, profit_and_loss_parser.get_sample_number(), profit_and_loss_parser.get_fd_step());

	PortfolioManager portfolio_manager(product, model, market, pricing_unit, hedging_unit, profit_and_loss_parser.get_rebalancing_times(),
		model_input_parameters.get_monitoring_times());
	double profit_and_loss = portfolio_manager.hedge();
	std::cout << profit_and_loss << std::endl;
	return 0;
}

