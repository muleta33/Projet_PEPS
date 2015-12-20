#include <cmath>
#include "MonteCarloHedging.hpp"
#include "MonteCarloRoutine.hpp"

using namespace products;
using namespace models;

void MonteCarloHedging::hedge(const UnderlyingModel & underlying_model, const products::Product & product, const PnlVect * const spots, PnlVect * hedging_results) const
{
	MonteCarloRoutineAtOrigin mco(underlying_model, product, sample_number, spots);
	mco.delta_hedge(shift, hedging_results);
}

void MonteCarloHedging::hedge_at(const double time, const models::UnderlyingModel & underlying_model, const products::Product & product, const PnlMat * const past, PnlVect * hedging_results) const
{
	MonteCarloRoutineAtTimeT mct(underlying_model, product, sample_number, past, time);
	mct.delta_hedge(shift, hedging_results);
}