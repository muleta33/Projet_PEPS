#pragma once
#include "EurostralMutualFundInputParser.hpp"
#include "pnl\pnl_vector.h"

class EurostralMutualFundInputParameters : public input_parsers::EurostralMutualFundInputParser
{
private:
	int const underlying_number = 3;
	double const guaranteed_capital_percentage = 90;
	double const indexes_return_percentage = 0.95;
	double const maturity = 8;
	double const initial_capital = 150;
	PnlVect *underlying_coefficients;
public:
	double get_maturity() const { return maturity; }
	int get_underlying_number() const { return underlying_number; }
	double get_guaranteed_capital_percentage() const { return guaranteed_capital_percentage; }
	double get_indexes_return_percentage() const { return indexes_return_percentage; }
	double get_initial_capital() const { return initial_capital; }
	PnlVect * get_underlying_coefficients() const { return underlying_coefficients; }

	EurostralMutualFundInputParameters()
	{
		pnl_vect_create(underlying_number);
		LET(underlying_coefficients, 0) = 0.5;
		LET(underlying_coefficients, 1) = 0.3;
		LET(underlying_coefficients, 2) = 0.2;
	}

	virtual ~EurostralMutualFundInputParameters() { pnl_vect_free(&underlying_coefficients); }
};