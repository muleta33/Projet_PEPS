#pragma once 
#include "pnl/pnl_matrix.h"
#include "RandomGeneration.hpp"
#include "BlackScholesModelRoutine.hpp"
#include "BlackScholesModelInputParser.hpp"
#include "ProfitAndLossInputParser.hpp"

using namespace input_parameters;

class BlackScholesModelMarket
{

public:
	virtual const PnlMat * const get_market_asset_paths() const = 0;
	virtual void get_volatilities_and_correlations(PnlVect * volatilities, PnlMat * correlations, int before) const = 0;
	virtual ~BlackScholesModelMarket() {};

protected:
	PnlMat * market_asset_paths_;
};