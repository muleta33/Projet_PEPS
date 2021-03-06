// EurostralPricer.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "EurostralPricer.h"
#include "EurostralMutualFundParameters.hpp"
#include "BlackScholesModelParameters.hpp"
#include "BlackScholesModelUtilities.hpp"

using namespace models;

// This is the constructor of a class that has been exported.
// see EurostralPricer.h for the class definition
EurostralPricer::EurostralPricer(double * past_data_array, int number_of_past_dates, int sample_number)
{
	PnlMat * past_data = pnl_mat_create_from_ptr(number_of_past_dates, 2 * underlying_number, past_data_array);
	const EurostralMutualFundParameters fund_parameters;
	fund = new products::EurostralMutualFund(fund_parameters);
	random_generator = new generators::PnlRandomGeneration();
	const BlackScholesModelParameters model_parameters(past_data);
	model = new models::BlackScholesModelRiskNeutral(model_parameters, *random_generator);
	pricer = new MonteCarloPricing(*model, *fund, sample_number);
	hedger = new MonteCarloHedging(*model, *fund, sample_number, 0.1);
}

EurostralPricer::~EurostralPricer() {
	delete random_generator;
	delete fund;
	delete model;
	delete pricer;
	delete hedger;
}

void EurostralPricer::price(double * spots, double &price, double &confidence_interval) {
	PnlVect * spots_pnl = pnl_vect_create_from_ptr(2 * underlying_number, spots);
	PnlVect * domestic_spots_pnl = pnl_vect_create(2 * underlying_number);
	foreign_to_domestic_spots(spots_pnl, domestic_spots_pnl);
	pricer->price(domestic_spots_pnl, price, confidence_interval);
	pnl_vect_free(&spots_pnl);
	pnl_vect_free(&domestic_spots_pnl);
}

void EurostralPricer::price_at(const double time, int number_of_rows_past, double * past, double * spots, double &price, double &confidence_interval) {
	PnlMat * past_pnl = pnl_mat_create_from_ptr(number_of_rows_past, underlying_number, past);
	PnlMat * domestic_past_pnl = pnl_mat_create(number_of_rows_past + 1, 2*underlying_number);
	PnlVect * spots_pnl = pnl_vect_create_from_ptr(2 * underlying_number, spots);
	historical_to_domestic_past_data(past_pnl, domestic_past_pnl, spots_pnl, model->foreign_interest_rates(), time, model->timestep());
	pricer->price_at(time, domestic_past_pnl, price, confidence_interval);
	pnl_mat_free(&past_pnl);
	pnl_mat_free(&domestic_past_pnl);
	pnl_vect_free(&spots_pnl);
}

void EurostralPricer::hedge(double * spots, double * hedging_results) {
	PnlVect * spots_pnl = pnl_vect_create_from_ptr(2 * underlying_number, spots);
	PnlVect * domestic_spots_pnl = pnl_vect_create(2 * underlying_number);
	PnlVect * hedging_results_pnl = pnl_vect_create_from_zero(2 * underlying_number);
	foreign_to_domestic_spots(spots_pnl, domestic_spots_pnl);
	hedger->hedge(domestic_spots_pnl, hedging_results_pnl);
	memcpy(hedging_results, hedging_results_pnl->array, 2*underlying_number*sizeof(double));
	pnl_vect_free(&spots_pnl);
	pnl_vect_free(&domestic_spots_pnl);
	pnl_vect_free(&hedging_results_pnl);
}

void EurostralPricer::hedge_at(const double time, int number_of_rows_past, double * past, double * spots, double * hedging_results) {
	PnlMat * past_pnl = pnl_mat_create_from_ptr(number_of_rows_past, underlying_number, past);
	PnlMat * domestic_past_pnl = pnl_mat_create(number_of_rows_past +1, 2 * underlying_number);
	PnlVect * spots_pnl = pnl_vect_create_from_ptr(2 * underlying_number, spots);
	PnlVect * hedging_results_pnl = pnl_vect_create_from_zero(2*underlying_number);
	historical_to_domestic_past_data(past_pnl, domestic_past_pnl, spots_pnl, model->foreign_interest_rates(), time,model->timestep());
	hedger->hedge_at(time, domestic_past_pnl, hedging_results_pnl);
	memcpy(hedging_results, hedging_results_pnl->array,2* underlying_number*sizeof(double));
	pnl_mat_free(&past_pnl);
	pnl_mat_free(&domestic_past_pnl);
	pnl_vect_free(&spots_pnl);
	pnl_vect_free(&hedging_results_pnl);
}
