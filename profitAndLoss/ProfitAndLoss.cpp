// ProfitAndLoss.cpp : Defines the entry point for the console application.
//
#pragma once
#include "stdafx.h"
#include <iostream>
#include "pnl\pnl_matrix.h"
#include "FakeBlackScholesModelInputParserProfitAndLoss.hpp"
#include "BlackScholesModelMarket.hpp"
#include "BlackScholesModelRiskNeutral.hpp"
#include "PnlRandomGeneration.hpp"
#include "FakeEurostralMutualFundInputParser.hpp"
#include "PNLRandomGeneration.hpp"
#include "PortfolioManager.hpp"

using namespace products;
using namespace generators;
using namespace models;

int _tmain(int argc, _TCHAR* argv[])
{
	const FakeEurostralMutualFundInputParser fake_fund_parser;
	EurostralMutualFund fund(fake_fund_parser);
	const PnlRandomGeneration random_generator;
	const FakeBlackScholesModelInputParserProfitAndLoss fake_model_parser;
	BlackScholesModelRiskNeutral model(fake_model_parser, random_generator);
	BlackScholesModelMarket market(fake_model_parser, random_generator);
	const int number_of_samples = 10000;
	PnlVect *spot = pnl_vect_create_from_scalar(model.underlying_number(), 10);

	PortfolioManager portfolio_manager(fund, model, market, fake_model_parser.get_rebalancing_times(), fake_model_parser.get_monitoring_times(), 
		fake_model_parser.get_fd_step(), number_of_samples, spot);
	double profit_and_loss = portfolio_manager.hedge();
	std::cout << "Profit & loss: " << profit_and_loss << std::endl;
	return 0;
}
