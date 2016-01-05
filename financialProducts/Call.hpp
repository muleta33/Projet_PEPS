#pragma once
#include "Product.hpp"

class Call : public products::Product
{
private:
	double strike;

public:
	Call() : Product(), strike(0) {};
	Call(double maturity, double strike) : Product(maturity, 1), strike(strike) {};

	double get_payoff(const PnlMat * const underlying_paths) const
	{
		double payoff = MGET(underlying_paths, underlying_paths->m - 1, 0) - strike;
		if (payoff > 0)
			return payoff;
		return 0;
	}

};