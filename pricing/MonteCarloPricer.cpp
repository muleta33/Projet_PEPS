#include <cmath>
#include "MonteCarloPricer.hpp"
#include "MonteCarloRoutine.hpp"

using namespace products;
using namespace models;



void MonteCarloPricer::price(const UnderlyingModel & underlying_model, const products::Product & product, PnlVect * spots, double &price, double &confidence_interval) const
{
	MonteCarloRoutineAtOrigin mco(underlying_model, product, sample_number, spots);
	mco.price(price, confidence_interval);
}

void MonteCarloPricer::price_at(const double time, const models::UnderlyingModel & underlying_model, const products::Product & product, PnlMat * past, double &price, double &confidence_interval) const
{
	MonteCarloRoutineAtTimeT mct(underlying_model, product, sample_number, past, time);
	mct.price(price, confidence_interval);
}