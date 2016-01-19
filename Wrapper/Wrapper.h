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
	public:
		PricerWrapper()
		{
			double vol[3] = { 0.1, 0.3, 0.15 };
			pricer = new EurostralPricer(vol, 0.3, 10000);
		}

		void get_price_product(double spot1, double spot2, double spot3);

		double get_price();

		~PricerWrapper()
		{
			delete pricer;
			pricer = 0;
		}
	};
}

