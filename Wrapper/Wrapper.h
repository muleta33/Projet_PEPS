// Wrapper.h

#pragma once
#include "EurostralPricer.h"

using namespace System;

namespace Wrapper_Pricer {

	public ref class PricerWrapper
	{
	private:
		EurostralPricer *pricer;
		double price = 0;
		double ic = 0;
		array<System::Double>^ deltas;

	public:
		PricerWrapper(array<System::Double>^ past_data, int number_of_past_dates, int number_of_samples)
		{
			cli::pin_ptr<double> past_data_pointer = &past_data[0];
			pricer = new EurostralPricer(past_data_pointer, number_of_past_dates, number_of_samples);
		}

		void compute_price(array<System::Double>^ spots);
		void compute_price_at(double time, array<System::Double>^ past, array<System::Double>^ spots, int number_of_observation_dates);
		void compute_deltas(array<System::Double>^ spots);
		void compute_deltas_at(double time, array<System::Double>^past, array<System::Double>^ spots, int number_of_observation_dates);

		double get_price();
		double get_price_ic();
		array<System::Double>^ get_deltas();

		~PricerWrapper()
		{
			delete pricer;
			pricer = 0;
		}
	};
}

