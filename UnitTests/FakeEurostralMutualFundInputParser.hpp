#pragma once
#include "EurostralMutualFundInputParser.hpp"

namespace UnitTests {
	class FakeEurostralMutualFundInputParser : public input_parsers::EurostralMutualFundInputParser
	{
	private:
		PnlVect * underlying_coefficients;

	public:
		FakeEurostralMutualFundInputParser()
		{
			underlying_coefficients = pnl_vect_create(3);
			LET(underlying_coefficients, 0) = 0.5;
			LET(underlying_coefficients, 1) = 0.3;
			LET(underlying_coefficients, 2) = 0.2;
		}

		~FakeEurostralMutualFundInputParser()
		{
			// Ligne suivante à commenter pour lancement des tests sans mode DEBUG : explications ?
			pnl_vect_free(&underlying_coefficients);
			underlying_coefficients = nullptr;
		}

		double get_maturity() const { return 0; };
		int get_underlying_number() const { return 3; };
		double get_guaranteed_capital_percentage() const { return 0.9; };
		double get_indexes_return_percentage() const { return 0.95; };
		double get_initial_capital() const { return 100; };
		PnlVect * get_underlying_coefficients() const { return underlying_coefficients; };
		virtual PnlVect * get_currencies() const { PnlVect * res = pnl_vect_create_from_double(6, 1); LET(res, 5) = 0; return res; }

	};
}