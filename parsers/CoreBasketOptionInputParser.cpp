#include "CoreBasketOptionInputParser.hpp"

using namespace input_parsers;

int CoreBasketOptionInputParser::get_underlying_number() const
{
	return underlying_number;
}

double CoreBasketOptionInputParser::get_maturity() const
{
	double result;
	parser.extract("maturity", result);
	return result;
}

PnlVect * CoreBasketOptionInputParser::get_underlying_coefficients() const
{
	PnlVect * result;
	parser.extract("underlying coefficients", result, underlying_number);
	return result;
}

double CoreBasketOptionInputParser::get_strike() const
{
	double result;
	parser.extract("strike", result);
	return result;
}