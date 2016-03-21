// This is the main DLL file.

// This is the main DLL file.

#include "stdafx.h"

#include "Wrapper.h"

namespace Wrapper_Pricer {

	void PricerWrapper::compute_price(array<System::Double>^ spots) {
		double price, ic;
		cli::pin_ptr<double> spots_pointer = &spots[0];
		pricer->price(spots_pointer, price, ic);
		this->price = price;
		this->ic = ic;
	}

	void PricerWrapper::compute_price_at(double time, array<System::Double>^ past, array<System::Double>^ spots, int number_of_observation_dates) {
		double price, ic;
		cli::pin_ptr<double> past_pointer = &past[0];
		cli::pin_ptr<double> spots_pointer = &spots[0];
		pricer->price_at(time, number_of_observation_dates, past_pointer, spots_pointer, price, ic);
		this->price = price;
		this->ic = ic;
	}

	void PricerWrapper::compute_deltas(array<System::Double>^ spots) {
		array<System::Double>^ hedging_results = gcnew array<System::Double>(3);
		cli::pin_ptr<double> hedging_results_pointer = &hedging_results[0];
		cli::pin_ptr<double> spots_pointer = &spots[0];
		pricer->hedge(spots_pointer, hedging_results_pointer);
		this->deltas = hedging_results;
	}

	void PricerWrapper::compute_deltas_at(double time, array<System::Double>^past, array<System::Double>^ spots, int number_of_observation_dates) {
		array<System::Double>^ hedging_results = gcnew array<System::Double>(3);
		cli::pin_ptr<double> hedging_results_pointer = &hedging_results[0];
		cli::pin_ptr<double> past_pointer = &past[0];
		cli::pin_ptr<double> spots_pointer = &spots[0];
		pricer->hedge_at(time, number_of_observation_dates, past_pointer, spots_pointer, hedging_results_pointer);
		this->deltas = hedging_results;
	}
	double PricerWrapper::get_price() {
		return this->price;
	}

	double PricerWrapper::get_price_ic() {
		return this->ic;
	}

	array<System::Double>^ PricerWrapper::get_deltas() {
		return this->deltas;
	}
}

