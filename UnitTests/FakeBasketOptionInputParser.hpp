#pragma once
#include "BasketOptionInputParser.hpp"

namespace UnitTests {
	class FakeBasketOptionInputParser : public input_parsers::BasketOptionInputParser
	{
	private:
		PnlVect * underlying_coefficients;

	public:
		FakeBasketOptionInputParser()
		{
			underlying_coefficients = pnl_vect_create(1);
			LET(underlying_coefficients, 0) = 1.0;
		}

		~FakeBasketOptionInputParser()
		{
			// Ligne suivante à commenter pour lancement des tests sans mode DEBUG : explications ?
			//pnl_vect_free(&underlying_coefficients);
			underlying_coefficients = nullptr;
		}

		double get_maturity() const { return 0; };
		int get_underlying_number() const { return 1; };
		double get_strike() const { return 90; }
		PnlVect * get_underlying_coefficients() const { return underlying_coefficients; };
	};
}