#include <cmath>
#include "MonteCarloPricing.hpp"
#include "MonteCarloRoutine.hpp"

using namespace products;
using namespace models;



void MonteCarloPricing::price(PnlVect * spots, double &price, double &confidence_interval) const
{
	MonteCarloRoutineAtOrigin mco(underlying_model_, product_, sample_number, spots);
	mco.price(price, confidence_interval);
}

void MonteCarloPricing::price_at(const double time, PnlMat * past, double &price, double &confidence_interval) const
{
	MonteCarloRoutineAtTimeT mct(underlying_model_, product_, sample_number, past, time);
	mct.price(price, confidence_interval);
}