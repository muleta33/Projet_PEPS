#pragma once
#include "pnl/pnl_vector.h"

namespace input_parsers
{
	class BasketOptionInputParser
	{
	public:
		virtual double get_maturity() const = 0;
		virtual int get_underlying_number() const = 0;
		virtual PnlVect * get_underlying_coefficients() const = 0;
		virtual double get_strike() const = 0;
		virtual PnlVect * get_currencies() const = 0;

		virtual ~BasketOptionInputParser() {};
	};
}