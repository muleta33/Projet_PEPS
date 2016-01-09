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
	//const CoreEurostralMutualFundInputParser fund_parser(parser);
	//EurostralMutualFund product(fund_parser);
	const CoreBasketOptionInputParser basket_parser(parser);
	BasketOption product(basket_parser);
	const PnlRandomGeneration random_generator;
	BlackScholesModelRiskNeutral model(model_parser, random_generator);
	BlackScholesModelMarket market(model_parser, profit_and_loss_parser, random_generator);

	PortfolioManager portfolio_manager(product, model, market, profit_and_loss_parser.get_rebalancing_times(), model_parser.get_monitoring_times(), 
		profit_and_loss_parser.get_fd_step(), profit_and_loss_parser.get_sample_number(), profit_and_loss_parser.get_spot());
	double profit_and_loss = portfolio_manager.hedge();
	std::cout << "Profit & loss: " << profit_and_loss << std::endl;
	return 0;
}
