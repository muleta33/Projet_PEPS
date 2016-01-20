// EurostralPricer.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "EurostralPricer.h"
#include "EurostralMutualFundParameters.hpp"
#include "BlackScholesModelParameters.hpp"

// This is the constructor of a class that has been exported.
// see EurostralPricer.h for the class definition
EurostralPricer::EurostralPricer(double *vol, double correlation, int sample_number)
{
	PnlVect * volatilities = pnl_vect_create_from_ptr(underlying_number, vol);
	const EurostralMutualFundInputParameters fund_parameters;
	fund = new products::EurostralMutualFund(fund_parameters);
	random_generator = new generators::PnlRandomGeneration();
	const BlackScholesModelParameters model_parameters(volatilities, correlation);
	model = new models::BlackScholesModelRiskNeutral(model_parameters, *random_generator);
	pricer = new MonteCarloPricing(*model, *fund, sample_number);
	hedger = new MonteCarloHedging(*model, *fund, sample_number, 0.1);
	pnl_vect_free(&volatilities);
}

EurostralPricer::~EurostralPricer() {
	delete random_generator;
	delete fund;
	delete model;
	delete pricer;
	delete hedger;
}

void EurostralPricer::price(double * spots, double &price, double &confidence_interval) {
	PnlVect * spots_pnl = pnl_vect_create_from_ptr(underlying_number, spots);
	pricer->price(spots_pnl, price, confidence_interval);
	pnl_vect_free(&spots_pnl);
}

void EurostralPricer::price_at(const double time, int number_of_rows_past, double * past, double &price, double &confidence_interval) {
	PnlMat * past_pnl = pnl_mat_create_from_ptr(number_of_rows_past, underlying_number, past);
	pricer->price_at(time, past_pnl, price, confidence_interval);
	pnl_mat_free(&past_pnl);
}

void EurostralPricer::hedge(double * spots, double * hedging_results) {
	PnlVect * spots_pnl = pnl_vect_create_from_ptr(underlying_number, spots);
	PnlVect * hedging_results_pnl = pnl_vect_create_from_zero(underlying_number);
	hedger->hedge(spots_pnl, hedging_results_pnl);
	memcpy(hedging_results, hedging_results_pnl->array, underlying_number);
	pnl_vect_free(&spots_pnl);
	pnl_vect_free(&hedging_results_pnl);
}

void EurostralPricer::hedge_at(const double time, int number_of_rows_past, double * past, double * hedging_results) {
	PnlMat * past_pnl = pnl_mat_create_from_ptr(number_of_rows_past, underlying_number, past);
	PnlVect * hedging_results_pnl = pnl_vect_create_from_zero(underlying_number);
	hedger->hedge_at(time, past_pnl, hedging_results_pnl);
	memcpy(hedging_results, hedging_results_pnl->array, underlying_number);
	pnl_mat_free(&past_pnl);
	pnl_vect_free(&hedging_results_pnl);
}
