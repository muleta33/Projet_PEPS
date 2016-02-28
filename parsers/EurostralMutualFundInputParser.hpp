#pragma once
#include "pnl/pnl_vector.h"

namespace input_parsers
{
	class EurostralMutualFundInputParser
	{
	public:
		virtual double get_maturity() const = 0;
		virtual int get_underlying_number() const = 0;
		virtual double get_guaranteed_capital_percentage() const = 0;
		virtual double get_indexes_return_percentage() const = 0;
		virtual double get_initial_capital() const = 0;
		virtual PnlVect * get_underlying_coefficients() const = 0;
		virtual PnlVect * get_currencies() const  = 0;

		virtual ~EurostralMutualFundInputParser() {};
	};
}