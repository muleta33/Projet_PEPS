#pragma once 
#include "pnl/pnl_matrix.h"
#include "RandomGeneration.hpp"
#include "BlackScholesModelRoutine.hpp"
#include "BlackScholesModelInputParserProfitAndLoss.hpp"


class BlackScholesModelMarket
{

public:
	BlackScholesModelMarket(const BlackScholesModelInputParserProfitAndLoss &parser, const generators::RandomGeneration &random_generator);
	const PnlMat* const simulate_market_asset_paths(const PnlVect * const spot) const;
	~BlackScholesModelMarket();

private:
	PnlMat * generated_market_asset_paths_;
	const double TIME_PRECISION = 10.e-5;
	double timestep_;
	models::BlackScholesModelRoutine* routine;
};