// EurostralPricer.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "EurostralPricer.h"
#include "EurostralMutualFundParameters.hpp"
#include "BlackScholesModelParameters.hpp"
#include "PNLRandomGeneration.hpp"

// This is the constructor of a class that has been exported.
// see EurostralPricer.h for the class definition
EurostralPricer::EurostralPricer(double *vol, double correlation, int sample_number)
{
	PnlVect * volatilities = pnl_vect_create_from_ptr(3, vol);
	const EurostralMutualFundInputParameters fund_parameters;
	fund = new products::EurostralMutualFund(fund_parameters);
	const generators::PnlRandomGeneration random_generator;
	const BlackScholesModelParameters model_parameters(volatilities, correlation);
	model = new models::BlackScholesModelRiskNeutral(model_parameters, random_generator);
	pricer = new MonteCarloPricing(*model, *fund, sample_number);
	hedger = new MonteCarloHedging(*model, *fund, sample_number, 0.1);
	pnl_vect_free(&volatilities);
}

EurostralPricer::~EurostralPricer() {
	delete fund;
	delete model;
	delete pricer;
	delete hedger;
}

void EurostralPricer::price(double * spots, double &price, double &confidence_interval) {
	PnlVect *S = pnl_vect_create_from_ptr(3, spots);
	pricer->price(S, price, confidence_interval);
}

void EurostralPricer::price_at(const double time, PnlMat * past, double &price, double &confidence_interval) {
	pricer->price_at(time, past, price, confidence_interval);
}

void EurostralPricer::hedge(const PnlVect * const spots, PnlVect * hedging_results) {
	hedger->hedge(spots, hedging_results);
}

void EurostralPricer::hedge_at(const double time, const PnlMat * const past, PnlVect * hedging_results) {
	hedger->hedge_at(time, past, hedging_results);
}
