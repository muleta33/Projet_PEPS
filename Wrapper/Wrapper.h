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
		PricerWrapper()
		{
			double vol[3] = { 0.1, 0.3, 0.15 };
			double correlation_matrix[3 * 3] = { 1, 0.3, 0.3, 0.3, 1, 0.3, 0.3, 0.3, 1 };
			pricer = new EurostralPricer(vol, correlation_matrix, 10000);
		}

		PricerWrapper(array<System::Double>^ volatilities, array<System::Double>^ correlation_matrix)
		{
			cli::pin_ptr<double> volatilities_pointer = &volatilities[0];
			cli::pin_ptr<double> correlation_matrix_pointer = &correlation_matrix[0];
			pricer = new EurostralPricer(volatilities_pointer, correlation_matrix_pointer, 10000);
		}

		void compute_price(array<System::Double>^ spots);
		void compute_price_at(double time, array<System::Double>^ past, int number_of_observation_dates);
		void compute_deltas(array<System::Double>^ spots);
		void compute_deltas_at(double time, array<System::Double>^past, int number_of_observation_dates);

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

