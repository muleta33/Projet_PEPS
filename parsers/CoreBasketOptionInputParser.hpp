#pragma once
#include "BasketOptionInputParser.hpp"
#include "Parser.hpp"

namespace input_parsers
{
	class CoreBasketOptionInputParser : public BasketOptionInputParser
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

		CoreBasketOptionInputParser(const Parser & p) : parser(p)
		{
			parser.extract("option size", underlying_number);
		}

		virtual ~CoreBasketOptionInputParser() {};
	};
}