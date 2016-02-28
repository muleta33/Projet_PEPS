#pragma once
#include "pnl/pnl_vector.h"

namespace input_parsers
{
	class BlackScholesModelInputParser
	{
	public:
		virtual int get_underlying_number() const = 0;
		virtual int get_monitoring_times() const = 0;
		virtual PnlMat * get_correlation_matrix() const = 0;
		virtual double get_interest_rate() const = 0;
		virtual double get_maturity() const = 0;
		virtual PnlVect * get_volatility() const = 0;
		virtual PnlVect * get_foreign_interest_rates() const = 0;

		virtual ~BlackScholesModelInputParser() {};
	};
}