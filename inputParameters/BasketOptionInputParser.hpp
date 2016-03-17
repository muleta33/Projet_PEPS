#pragma once
#include "BasketOptionInputParameters.hpp"
#include "Parser.hpp"

namespace input_parameters
{
	class BasketOptionInputParser : public BasketOptionInputParameters
	{
	private:
		const Parser & parser;
		int underlying_number;
	public:
		double get_maturity() const;
		int get_underlying_number() const;
		PnlVect * get_underlying_coefficients() const;
		double get_strike() const;
		PnlVect * get_currencies() const;

		BasketOptionInputParser(const Parser & p) : parser(p)
		{
			parser.extract("option size", underlying_number);
		}

		virtual ~BasketOptionInputParser() {};
	};
}