#include "BlackScholesModelMarket.hpp"


class BlackScholesModelMarketSimulate : public BlackScholesModelMarket
{

public:
	BlackScholesModelMarketSimulate(const BlackScholesModelInputParser &model_parser, const ProfitAndLossInputParser &pl_parser,
		const generators::RandomGeneration &random_generator);
	const PnlMat* const get_market_asset_paths() const;
	void get_volatilities_and_correlations(PnlVect * volatilities, PnlMat * correlations, int before) const;
	
	~BlackScholesModelMarketSimulate();

private:
	PnlVect * spot_;
	PnlVect * volatilities_;
	PnlMat * correlations_;
	models::BlackScholesModelRoutine * routine_;
};