// This is the main DLL file.

// This is the main DLL file.

#include "stdafx.h"

#include "Wrapper.h"

namespace Wrapper_Pricer {

	void PricerWrapper::get_price_product(double spot1, double spot2, double spot3) {
		double price, ic;
		double spots[3] = { spot1, spot2, spot3 };
		pricer->price(spots, price, ic);
		this->price = price;
		this->ic = ic;

	}

	double PricerWrapper::get_price() {
		return this->price;
	}
}

