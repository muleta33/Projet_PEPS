#pragma once
#include <pnl/pnl_matrix.h>

namespace products {

	class Product
	{
	protected:
		double maturity_;
		int underlying_number_;
		PnlVect * currencies_;

	public:
		Product() :
			maturity_(0),
			underlying_number_(0),
			currencies_(nullptr)
		{};

		Product(double maturity, int underlying_number, PnlVect * currencies) :
			maturity_(maturity),
			underlying_number_(underlying_number)
			{ currencies_ = pnl_vect_copy(currencies); };

		double get_maturity() const { return maturity_; };
		int get_underlying_number() const { return underlying_number_; };
		PnlVect * get_currencies() const { return currencies_; }

		virtual double get_payoff(const PnlMat * const underlying_paths) const = 0;
		virtual ~Product() { pnl_vect_free(&currencies_); };
	};
}