// ProfitAndLoss.cpp : Defines the entry point for the console application.
//
#pragma once
#include "stdafx.h"
#include <iostream>
#include "pnl\pnl_matrix.h"
#include "Parser.hpp"
#include "BlackScholesModelMarket.hpp"
#include "BlackScholesModelRiskNeutral.hpp"
#include "CoreEurostralMutualFundInputParser.hpp"
#include "CoreBasketOptionInputParser.hpp"
#include "CoreBlackScholesModelInputParser.hpp"
#include "CoreProfitAndLossInputParser.hpp"
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
using namespace input_parsers;

int main(int argc, char* argv[])
{
	const char * input_file = argv[1];
	Parser parser(input_file);
	const CoreBlackScholesModelInputParser model_parser(parser);
	const CoreProfitAndLossInputParser profit_and_loss_parser(parser);
	const CoreEurostralMutualFundInputParser fund_parser(parser);
	EurostralMutualFund product(fund_parser);
	//const CoreBasketOptionInputParser basket_parser(parser);
	//BasketOption product(basket_parser);
	const PnlRandomGeneration random_generator;
	BlackScholesModelRiskNeutral model(model_parser, random_generator);
	BlackScholesModelMarket market(model_parser, profit_and_loss_parser, random_generator);
	MonteCarloPricing pricing_unit(model, product, profit_and_loss_parser.get_sample_number());
	MonteCarloHedging hedging_unit(model, product, profit_and_loss_parser.get_sample_number(), profit_and_loss_parser.get_fd_step());
	//PricingExactCall pricing_unit(model, product);
	//HedgingExactCall hedging_unit(model, product);

	PortfolioManager portfolio_manager(product, model, market, pricing_unit, hedging_unit, profit_and_loss_parser.get_rebalancing_times(), model_parser.get_monitoring_times(), profit_and_loss_parser.get_spot());
	double profit_and_loss = portfolio_manager.hedge();
	std::cout << profit_and_loss << std::endl;
	return 0;
}
